#ifndef BETASimulationManager_h
#define BETASimulationManager_h 1

#include "G4UserEventAction.hh"
#include "globals.hh"
#include "BETASimulationMessenger.hh"
#include <fstream>

#include "HallCBeamEvent.h"
#include "HMSEvent.h"
#include "BETAEvent.h"
#include "BETAG4MonteCarloEvent.h"
#include "GasCherenkovEvent.h"
#include "BigcalEvent.h"
#include "GasCherenkovHit.h"
#include "InSANERun.h"

class BETARunAction;
class G4Track;
class BETASimulationMessenger;

/**
 * \ingroup Runs
 */

/**
 * \brief The simulation manager singleton class
 */
/**
 * The BETAG4 simulation manager class. The "Analysis" in the class name is a 
 * bit of an artifact. \todo{Rename BETASimulationManager. Not sure what the best name would be... BETAG4Manager?}
 * 
 * The Manager holds all configuration information such as run number, verbosity levels, primary vertex type, target configuration, geometries to construct. 
 */

class BETASimulationManager {
public:
  ~BETASimulationManager();

  static BETASimulationManager* getInstance();
  static void dispose();


/**
 * Set detector's verbosity for debugging purposes
 */
  void SetDetectorVerbosity( char * detName, int level);

/**
 * Get detector's verbosity for debugging purposes
 */
  int GetDetectorVerbosity( char * detName);

/**
 * Sets whether plots are shown or not.
 * Not yet implemented.
 */
  void showPlot(int );

/**
 * Not yet implemented.
 */
  void write();

/**
 * Prints a general statement about the current run configuration
 * Not yet implemented.
 */ 
 void GetRunInfo(){ ; };

/**
 * If true, data is to be appeneded to the root file with current run number, which is not incremented
 * If false, the run number is incremented and a new root file is created.
 */ 
 void SetAppendMode(bool mode){ fIsAppendMode=mode; };

/**
 * If true, data is to be appeneded to the root file with current run number, which is not incremented
 * If false, the run number is incremented and a new root file is created.
 */ 
 bool IsAppendMode(){ return fIsAppendMode; };

/**
 * Returns the run number 
 * \todo Make the source of run number a database, not a text file. 
 */ 
 int GetRunNumber(){ return fRunNumber; };

protected: 
/**
 * Increments the run number in memory and in file/database
 * returns the run number
 * \todo Make the source of run number a database, not a text file. 
 */ 

 int IncrementRunNumber();


/**
 * Gets the run number from a file/database
 * \todo Make the source of run number a database, not a text file. 
 */ 
 int RetrieveRunNumber();

public:

  BETASimulationMessenger * fSimulationMessenger;

inline G4int plotterVisible() {return plotVis; }

  int fGasCherenkovVerbosity;
  int fBigcalVerbosity;
  int fLuciteHodoscopeVerbosity;
  int fForwardTrackerVerbosity;

private:


/// Contains all BETA detector's data
  BETAEvent * betaEvent;
/// Contains all the HMS data
  HMSEvent* hmsEvent;
/// Contains all the Hall C beam data
  HallCBeamEvent * beamEvent;
/// Contains all the Thrown and unrealistically obtained Montecarlo data
  BETAG4MonteCarloEvent * mcEvent;
/// Simulation Run object
  InSANERun * simulationRun;



  G4int plotVis;
  BETASimulationManager( );
  static BETASimulationManager* fgBETASimulationManager;
  int instanceNumber;
  int fRunNumber;
  bool fIsAppendMode;
  friend class BETARunAction;

};

#endif

