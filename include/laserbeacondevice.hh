///////////////////////////////////////////////////////////////////////////
//
// File: laserbeacondevice.hh
// Author: Andrew Howard
// Date: 12 Jan 2000
// Desc: Simulates the laser-based beacon detector
//
// CVS info:
//  $Source: /home/tcollett/stagecvs/playerstage-cvs/code/stage/include/laserbeacondevice.hh,v $
//  $Author: vaughan $
//  $Revision: 1.8 $
//
// Usage:
//  (empty)
//
// Theory of operation:
//  (empty)
//
// Known bugs:
//  (empty)
//
// Possible enhancements:
//  (empty)
//
///////////////////////////////////////////////////////////////////////////

// LBD = Laser Beacon Detector!

#ifndef LBDDEVICE_HH
#define LBDDEVICE_HH

#include "entity.hh"
#include "laserdevice.hh"
#include "guiexport.hh"

class CLBDDevice : public CEntity
{
    // Default constructor
    //
    public: CLBDDevice(CWorld *world, CLaserDevice *parent );

    // Update the device
    //
    public: virtual void Update( double sim_time );

    // Pointer to laser used as souce of data
    //
    private: CLaserDevice *m_laser;

    // Time of last update
    //
    private: uint32_t m_time_sec, m_time_usec;

    // Load the object from an argument list
    //
    public: virtual bool Load(int argc, char **argv);

    // Save the object to an argument list
    //
    public: virtual bool Save(int &argc, char **argv);

    // Detection parameters
    //
    private: double m_max_anon_range;
    private: double m_max_id_range;
    
    private:  ExportLaserBeaconDetectorData expBeacon; 

#ifdef INCLUDE_RTK
    
    // Process GUI update messages
    //
    public: virtual void OnUiUpdate(RtkUiDrawData *pData);

    // Draw the beacon data
    //
    public: void DrawData(RtkUiDrawData *event);

#endif
};

#endif






