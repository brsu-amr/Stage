
#include <X11/keysym.h> 
#include <X11/keysymdef.h> 
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>

#include "stage_types.hh"
#include "truthserver.hh"
#include "xs.hh"


void CXGui::RenderObject( truth_t &truth )
  {
#ifdef DEBUG
    //    char buf[30]
    //GetObjectType( exp, buf, 29 );
    //cout << "Rendering " << exp->objectId << " " << buf <<  endl;

    //printf( "R(%d,%d,%d)\n", pid.port, pid.type, pid.index );
    //fflush( stdout );
#endif

    bool extended = false;

    SetDrawMode( GXxor );


    // check the object type and call the appropriate render function 
    switch( truth.stage_type )
      {
      case NullType: break; // do nothing 
      case RectRobotType: RenderPioneer( &truth, extended ); break; 
      case LaserTurretType: RenderLaserTurret( &truth, extended ); break;
	//case PLAYER_BEACON_CODE: RenderLaserBeacon( &truth, extended ); break;
      case BoxType: RenderBox( &truth, extended ); break;
      case PlayerType: RenderPlayer( &truth, extended ); break;
      case MiscType: RenderMisc( &truth , extended); break;
      case SonarType: RenderSonar( &truth, extended ); break;
      case LaserBeaconType: RenderLaserBeacon( &truth, extended ); break;
      case LBDType: RenderLaserBeaconDetector( &truth, extended ); break;
      case VisionType: RenderVision( &truth, extended ); break;
      case PtzType: RenderPTZ( &truth, extended ); break;  
      case VisionBeaconType: RenderVisionBeacon( &truth, extended ); break;  
      case BroadcastType: RenderBroadcast( &truth, extended ); break;  
      case BroadcastType: RenderBroadcast( &truth, extended ); break;  
      case BroadcastType: RenderBroadcast( &truth, extended ); break;  
	
	// deprecated
	//case PLAYER_TRUTH_CODE: RenderTruth(&truth,extended); break;  
	//case PLAYER_OCCUPANCY_CODE: RenderOccupancy(&truth,extended); break; 
	
      default: cout << "XGui: unknown object type " 
		    << truth.id.type << endl;
      }
    
    SetDrawMode( GXcopy );
    
  }

void CXGui::RenderObjectLabel( truth_t* exp, char* str, int len )
{
  if( str && strlen(str) > 0 )
    {
      //SetForeground( RGB(255,255,255) );
      //DrawString( exp->x + 0.6, exp->y - (8.0/ppm + exp->id.type * 11.0/ppm) , 
      //	  str, len );
    }
}

void CXGui::RenderLaserTurret( truth_t* exp, bool extended )
{ 
  SelectColor( exp,blue );  
  DrawNoseBox( exp->x, exp->y, exp->w/2.0, exp->h/2.0, exp->th );

  //if( extended && exp->data )
  //      {
  //        SetForeground( RGB(70,70,70) );
  //        ExportLaserData* ld = (ExportLaserData*) exp->data;
  //        DrawPolygon( ld->hitPts, ld->hitCount );
  //      }
}

void CXGui::RenderLaserBeaconDetector( truth_t* exp, bool extended )
{ 
  SelectColor( exp,blue );
  DrawCircle( exp->x, exp->y, exp->w/2.0 );
//    if( extended && exp->data )
//      {
//        DPoint pts[4];
//        char buf[20];
      
//        SetForeground( RGB( 255, 0, 255 ) );
      
//        ExportLaserBeaconDetectorData* p = 
//  	(ExportLaserBeaconDetectorData*)exp->data;
      
//        if( p->beaconCount > 0 ) for( int b=0; b < p->beaconCount; b ++ )
//  	{
//  	  GetRect( p->beacons[ b ].x, p->beacons[ b ].y, 
//  		   (4.0/ppm), 0.12 + (2.0/ppm), p->beacons[ b ].th, pts );
	  
//  	  // don't close the rectangle 
//  	  // so the open box visually indicates heading of the beacon
//  	  DrawLines( pts, 4 ); 
	  
//  	  //DrawLines( &(pts[1]), 2 ); 

//  	  // render the beacon id as text
//  	  sprintf( buf, "%d", p->beacons[ b ].id );
//  	  DrawString( p->beacons[ b ].x + 0.2 + (5.0/ppm), 
//  		      p->beacons[ b ].y - (4.0/ppm), 
//  		      buf, strlen( buf ) );
//  	}
//      }
}

void CXGui::RenderTruth( truth_t* exp, bool extended )
{ 
  SelectColor( exp,green );
  DrawString( exp->x, exp->y, "Truth", 5 );
}

void CXGui::RenderOccupancy( truth_t* exp, bool extended )
{ 
  SelectColor( exp,green );
  DrawString( exp->x, exp->y, "Occupancy", 9 ); 
}

void CXGui::RenderPTZ( truth_t* exp, bool extended )
{ 
  SelectColor( exp,magenta );    
  DrawNoseBox( exp->x, exp->y, exp->w/2.0, exp->h/2.0, exp->th );

  //GetRect( exp->x, exp->y, exp->w, exp->h, exp->th, pts );  
  //DrawPolygon( pts, 4 );

  //DrawCircle( exp->x, exp->y, exp->w/2 );
  
  //DrawArc( exp->x, exp->y, exp->w, exp->h, exp->th + M_PI_2, M_PI );

  
//    if( 0 )
//    //if( extended && exp->data ) 
//      {

//        ExportPtzData* p = (ExportPtzData*)exp->data;
      
//        //DPoint pts[4];
//        //GetRect( exp->x, exp->y, 0.4, 0.4, exp->th, pts );
//        //DrawPolygon( pts, 4 );
      
//        double len = 0.2; // meters
//        // have to figure out the zoom with Andrew
//        // for now i have a fixed width triangle
//        //double startAngle =  exp->th + p->pan - p->zoom/2.0;
//        //double stopAngle  =  startAngle + p->zoom;
//        double startAngle =  exp->th + p->pan - DTOR( 30 );
//        double stopAngle  =  startAngle + DTOR( 60 );
      
//        pts[0].x = exp->x;
//        pts[0].y = exp->y;
      
//        pts[1].x = exp->x + len * cos( startAngle );  
//        pts[1].y = exp->y + len * sin( startAngle );  
      
//        pts[2].x = exp->x + len * cos( stopAngle );  
//        pts[2].y = exp->y + len * sin( stopAngle );  
      
//        DrawPolygon( pts, 3 );
//      }
 }

void CXGui::RenderSonar( truth_t* exp, bool extended )
{ 
  SelectColor( exp,green );
  //    if( extended && exp->data )
  //      {
  //        ExportSonarData* p = (ExportSonarData*)exp->data;
  //        SetForeground( RGB(70,70,70) );
  //        DrawPolygon( p->hitPts, p->hitCount );
  //      }
  //  }

  //SetForeground( RGB(255,0,0) );
  //SetForeground( yellow );
  //DrawString( exp->x, exp->y, "Sonar", 1 );
}

void CXGui::RenderVision( truth_t* exp, bool extended )
{ 
  SelectColor( exp,magenta );
  DrawCircle( exp->x, exp->y, exp->w/2.0 );
}

void CXGui::RenderPlayer( truth_t* exp, bool extended )
{ 
  SelectColor( exp,white );

  DPoint pts[8];
  GetRect( exp->x, exp->y, exp->w/2.0, exp->h/2.0, exp->th, pts );  
  
  DrawPolygon( pts, 4 );

  // draw an X across the box
  DrawLine( pts[0], pts[2] );
  DrawLine( pts[1], pts[3] );
}

void CXGui::RenderMisc( truth_t* exp, bool extended )
{
  //    if( 0 ) // disable for now
//    //if( extended && exp->data )
//      {
  //        SetForeground( RGB(255,0,0) ); // red to match Pioneer base
      
  //  char buf[20];
      
//        player_misc_data_t* p = (player_misc_data_t*)exp->data;
      
        // support bumpers eventually
        //p->rearbumpers;
//        //p->frontbumpers;
      
  //float v = 11.9;//p->voltage / 10.0;
  //sprintf( buf, "%.1fV", v );
  //DrawString( exp->x + 1.0, exp->y + 1.0, buf, strlen( buf ) ); 
//      }
}


void CXGui::RenderPioneer( truth_t* exp, bool extended )
 { 
   SelectColor( exp,red );
   
   double sinth = sin( exp->th );
   double costh = cos( exp->th );
   double dx = exp->rotdx * costh + exp->rotdy * sinth;
   double dy = exp->rotdx * sinth - exp->rotdy * costh;
   
   DPoint pts[7];
   GetRect( exp->x + dx, 
	    exp->y + dy,
	    exp->w/2.0, exp->h/2.0, exp->th, pts );
   
   pts[4].x = pts[0].x;
   pts[4].y = pts[0].y;
   
   pts[5].x = exp->x;
    pts[5].y = exp->y;
    
    pts[6].x = pts[3].x;
    pts[6].y = pts[3].y;
    
    DrawLines( pts, 7 );
 }

void CXGui::RenderLaserBeacon( truth_t* exp, bool extended )
{ 
  SelectColor( exp,cyan );  
  DrawNoseBox( exp->x, exp->y, exp->w/2.0, exp->h/2.0, exp->th );
}

void CXGui::RenderVisionBeacon( truth_t* exp, bool extended )
{ 
  SelectColor( exp,yellow );
  DrawCircle( exp->x, exp->y, exp->w/2.0 );
}

void CXGui::RenderBroadcast( truth_t* exp, bool extended )
{ 
  SelectColor( exp,green );
  DrawCircle( exp->x, exp->y, exp->w/2.0 );
}

void CXGui::RenderBox( truth_t* exp, bool extended )
{ 
  SelectColor( exp,yellow );

  DPoint pts[4];
  GetRect( exp->x, exp->y, exp->w/2.0, exp->h/2.0, exp->th, pts );
  
  DrawPolygon( pts, 4 );
}

void CXGui::SelectColor( truth_t* exp, unsigned long def )
{
  // choose the channel color, or use default if channel == -1
  if( exp->channel != -1 )
    SetForeground( channel_colors[exp->channel] );
  else
    SetForeground( def ); // default color
}




