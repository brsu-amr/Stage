
#include <stdlib.h>
#include <assert.h>

//#define DEBUG

#define STG_TOKEN_MAX 64

#define STG_DEFAULT_RESOLUTION    0.02  // 2cm pixels
#define STG_DEFAULT_INTERVAL_REAL 100   // msec between updates
#define STG_DEFAULT_INTERVAL_SIM  100 
  
#include "stage_internal.h"



extern int _stg_quit; // quit flag is returned by stg_world_update()

stg_world_t* stg_world_create( stg_id_t id, 
			       char* token, 
			       int sim_interval, 
			       int real_interval,
			       double ppm_high,
			       double ppm_med,
			       double ppm_low )
{
  PRINT_DEBUG2( "alternate world creator %d (%s)", id, token );
  
  stg_world_t* world = calloc( sizeof(stg_world_t),1 );
  
  world->id = id;
  world->token = strdup( token );
  world->models = g_hash_table_new_full( g_int_hash, g_int_equal,
					 NULL, model_destroy_cb );
  world->models_by_name = g_hash_table_new_full( g_str_hash, g_str_equal,
						 NULL, NULL );  
  world->sim_time = 0.0;
  world->sim_interval = sim_interval;
  world->wall_interval = real_interval;
  world->wall_last_update = 0;
  
  world->matrix = stg_matrix_create( ppm_high, ppm_med, ppm_low ); 
  
  world->ppm = ppm_high; // this is the finest resolution of the matrix
  
  world->paused = TRUE; // start paused.
  
  world->destroy = FALSE;
  
  world->win = gui_world_create( world );

  return world;
}

/// calculate the bounding rectangle of everything in the world
void stg_world_dimensions( stg_world_t* world, 
			   double* min_x, double * min_y,
			   double* max_x, double * max_y )
{
  *min_x = *min_y =  MILLION;
  *max_x = *max_y = -MILLION;
}


void stg_world_destroy( stg_world_t* world )
{
  assert( world );
		   
  PRINT_DEBUG1( "destroying world %d", world->id );
  
  
  stg_matrix_destroy( world->matrix );

  free( world->token );
  g_hash_table_destroy( world->models );

  gui_world_destroy( world );

  free( world );
}

void world_destroy_cb( gpointer world )
{
  stg_world_destroy( (stg_world_t*)world );
}


int stg_world_update( stg_world_t* world, int sleepflag )
{
  //PRINT_WARN( "World update" );

  //PRINT_WARN( "World update - not paused" );
 


#if 0 //DEBUG
  struct timeval tv1;
  gettimeofday( &tv1, NULL );
#endif
  
  if( world->win )
    {
      gui_poll();
      
      // if the window was closed, we request a quit
      if( gui_world_update( world ) )
	stg_quit_request();
    }

#if 0// DEBUG
  struct timeval tv2;
  gettimeofday( &tv2, NULL );
  
  double guitime = (tv2.tv_sec + tv2.tv_usec / 1e6) - 
    (tv1.tv_sec + tv1.tv_usec / 1e6);
  
  printf( " guitime %.4f\n", guitime );
#endif

  //putchar( '.' );
  //fflush( stdout );

  if( world->paused ) // only update if we're not paused
    return _stg_quit;

  
  stg_msec_t timenow = stg_timenow();
   
  //PRINT_DEBUG5( "timenow %lu last update %lu interval %lu diff %lu sim_time %lu", 
  //	timenow, world->wall_last_update, world->wall_interval,  
  //	timenow - world->wall_last_update, world->sim_time  );
  
  // if it's time for an update, update all the models
  stg_msec_t elapsed =  timenow - world->wall_last_update;

  if( world->wall_interval < elapsed )
    {
      stg_msec_t real_interval = timenow - world->wall_last_update;

#if 0      
      printf( " [%d %lu] sim:%lu real:%lu  ratio:%.2f\n",
	      world->id, 
	      world->sim_time,
	      world->sim_interval,
	      real_interval,
	      (double)world->sim_interval / (double)real_interval  );
      
      fflush(stdout);
#endif
      
      world->real_interval_measured = real_interval;
      
      g_hash_table_foreach( world->models, model_update_cb, world );
      
      
      world->wall_last_update = timenow;
      
      world->sim_time += world->sim_interval;
      
    }
  else
    if( sleepflag )
      {
	//puts( "sleeping" );
	usleep( 10000 ); // sleep a little
      }

  return _stg_quit; // may have been set TRUE by the GUI or someone else
}

//void world_update_cb( gpointer key, gpointer value, gpointer user )
//{
// stg_world_update( (stg_world_t*)value );
//}

stg_model_t* stg_world_get_model( stg_world_t* world, stg_id_t mid )
{
  return( world ? g_hash_table_lookup( (gpointer)world->models, &mid ) : NULL );
}

void stg_world_add_model( stg_world_t* world, 
			  stg_model_t* mod  )
{
  g_hash_table_replace( world->models, &mod->id, mod );
  g_hash_table_replace( world->models_by_name, mod->token, mod );
}

int stg_world_model_destroy( stg_world_t* world, stg_id_t model )
{
  // delete the model
  g_hash_table_remove( world->models, &model );
  
  return 0; // ok
}


void stg_world_print( stg_world_t* world )
{
  printf( " world %d:%s (%d models)\n", 
	  world->id, 
	  world->token,
	  g_hash_table_size( world->models ) );
  
   g_hash_table_foreach( world->models, model_print_cb, NULL );
}

void world_print_cb( gpointer key, gpointer value, gpointer user )
{
  stg_world_print( (stg_world_t*)value );
}

stg_model_t* stg_world_model_name_lookup( stg_world_t* world, const char* name )
{
  return (stg_model_t*)g_hash_table_lookup( world->models_by_name, name );
}


void stg_model_save_cb( gpointer key, gpointer data, gpointer user )
{
  stg_model_save( (stg_model_t*)data );
}

void stg_world_save( stg_world_t* world )
{
  // ask every model to save itself
  g_hash_table_foreach( world->models, stg_model_save_cb, NULL );

  gui_save( world->win );

  wf_save();
}



/** @defgroup world The World

Stage simulates a 'world' composed of models, defined in a 'world
file'. 

<h2>Worldfile properties</h2>

@par Summary and default values

@verbatim
world
(
   name            "[filename of worldfile]"
   interval_real   100
   interval_sim    100
   resolution      0.01
   resolution_med  0.01
   resolution_low  0.01
)
@endverbatim

@par Details
- name [string]
  - the name of the world, as displayed in the window title bar. Defaults to the worldfile file name.
- interval_sim [milliseconds]
  - the length of each simulation update cycle in milliseconds.
- interval_real [milliseconds]
  - the amount of real-world (wall-clock) time the siulator will attempt to spend on each simulation cycle.
- resolution [meters]
  - specifies the resolution of the underlying bitmap model. Larger values speed up raytracing at the expense of fidelity in collision detection and sensing. 

@par More examples

The Stage source distribution contains several example world files in
<tt>(stage src)/worlds</tt> along with the worldfile properties
described on the manual page for each model type.

*/

/* UNDOCUMENTED - don't want to confuse people.
- resolution_med [meters]
  - resolution of the medium-level raytrace bitmap.
- resolution_med [meters]
  - resolution of the top-level raytrace bitmap.
*/


// create a world containing a passel of Stage models based on the
// worldfile

stg_world_t* stg_world_create_from_file( char* worldfile_path )
{
  wf_load( worldfile_path );
  
  int section = 0;
  
  char* world_name =
    wf_read_string( section, "name", worldfile_path );
  
  stg_msec_t interval_real = 
    wf_read_int( section, "interval_real", STG_DEFAULT_INTERVAL_REAL );

  stg_msec_t interval_sim = 
    wf_read_int( section, "interval_sim", STG_DEFAULT_INTERVAL_SIM );
      
  double ppm_high = 
    1.0 / wf_read_float( section, "resolution", STG_DEFAULT_RESOLUTION ); 

  double ppm_med = 
    1.0 / wf_read_float( section, "resolution_med", 0.2 ); 
  
  double ppm_low = 
    1.0 / wf_read_float( section, "resolution_low", 1.0 ); 
  
  // create a single world
  stg_world_t* world = 
    stg_world_create( 0, 
		      world_name, 
		      interval_sim, 
		      interval_real,
		      ppm_high,
		      ppm_med,
		      ppm_low );

  if( world == NULL )
    return NULL; // failure
  
  // configure the GUI
  

  int section_count = wf_section_count();
  
  // Iterate through sections and create client-side models
  for( section = 1; section < section_count; section++ )
    {
      if( strcmp( wf_get_section_type(section), "window") == 0 )
	{
	  gui_load( world->win, section ); 
	}
      else
	{
	  char *typestr = (char*)wf_get_section_type(section);      
	  
	  int parent_section = wf_get_parent_section( section );
	  
	  PRINT_DEBUG2( "section %d parent section %d\n", 
			section, parent_section );
	  
	  stg_model_t* parent = NULL;
	  
	  parent = (stg_model_t*)
	    g_hash_table_lookup( world->models, &parent_section );
	  
	  // select model type based on the worldfile token
	  stg_model_type_t type;
	  
	  if( strcmp( typestr, "model" ) == 0 ) // basic model
	    type = STG_MODEL_BASIC;
	  else if( strcmp( typestr, "test" ) == 0 ) // specialized models
	    type = STG_MODEL_TEST;
	  else if( strcmp( typestr, "laser" ) == 0 )
	    type = STG_MODEL_LASER;
	  else if( strcmp( typestr, "ranger" ) == 0 )
	    type = STG_MODEL_RANGER;
	  else if( strcmp( typestr, "position" ) == 0 )
	    type = STG_MODEL_POSITION;
	  else if( strcmp( typestr, "blobfinder" ) == 0 )
	    type = STG_MODEL_BLOB;
	  else if( strcmp( typestr, "fiducialfinder" ) == 0 )
	    type = STG_MODEL_FIDUCIAL;
	  else 
	    {
	      PRINT_ERR1( "unknown model type \"%s\". Model has not been created.",
			  typestr ); 
	      continue;
	    }
	  
	  //PRINT_WARN3( "creating model token %s type %d instance %d", 
	  //	    typestr, 
	  //	    type,
	  //	    parent ? parent->child_type_count[type] : world->child_type_count[type] );
	  
	  // generate a name and count this type in its parent (or world,
	  // if it's a top-level object)
	  char namebuf[STG_TOKEN_MAX];  
	  if( parent == NULL )
	    snprintf( namebuf, STG_TOKEN_MAX, "%s:%d", 
		      typestr, 
		      world->child_type_count[type]++);
	  else
	    snprintf( namebuf, STG_TOKEN_MAX, "%s.%s:%d", 
		      parent->token,
		      typestr, 
		      parent->child_type_count[type]++ );
	  
	  //PRINT_WARN1( "generated name %s", namebuf );
	  
	  // having done all that, allow the user to specify a name instead
	  char *namestr = (char*)wf_read_string(section, "name", namebuf );
	  
	  //PRINT_WARN2( "loading model name %s for type %s", namebuf, typestr );
	  
	  
	  PRINT_DEBUG2( "creating model from section %d parent section %d",
			section, parent_section );
	  
	  stg_model_t* mod = NULL;
	  stg_model_t* parent_mod = stg_world_get_model( world, parent_section );
	  
	  switch( type )
	    {
	    case STG_MODEL_BASIC:
	      mod = stg_model_create( world, parent_mod, section, STG_MODEL_BASIC, namestr );
	      break;
	      
	    case STG_MODEL_BLOB:
	      mod = stg_blobfinder_create( world, parent_mod, section, namestr );
	      break;
	      	      
	    case STG_MODEL_LASER:
	      mod = stg_laser_create( world,  parent_mod, section, namestr );
	      break;
	      
	    case STG_MODEL_RANGER:
	      mod = stg_ranger_create( world,  parent_mod, section, namestr );
	      break;
	      
	    case STG_MODEL_FIDUCIAL:
	      mod = stg_fiducial_create( world,  parent_mod, section, namestr );
	      break;
	      
	    case STG_MODEL_POSITION:
	      mod = stg_position_create( world,  parent_mod, section, namestr );
	      break;

	    default:
	      PRINT_ERR1( "don't know how to configure type %d", type );
	    }

	  assert( mod );
	  
	  // configure the model with properties from the world file
	  stg_model_load( mod );
	  
	  // add the new model to the world
	  stg_world_add_model( world, mod );
	}
    }
  return world;
}



