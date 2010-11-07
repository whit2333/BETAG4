#ifndef BETASimulationManager_h
#define BETASimulationManager_h 1

#include "G4UserEventAction.hh"
#include "globals.hh"
#include "BETASimulationMessenger.hh"

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
  static BETASimulationManager* getInstance(const int);
  static void dispose();

/**
 * Set detector's verbosity for debugging purposes
 */
  void SetDetectorVerbosity( char * detName, int level);
  int GetDetectorVerbosity( char * detName);

  void showPlot(int );
  void write();

/**
 * Prints a general statement about the current run configuration
 */ 
 void GetRunInfo(){ ; };


  BETASimulationMessenger * messenger;

inline G4int plotterVisible() {return plotVis; }

  int fGasCherenkovVerbosity;
  int fBigcalVerbosity;
  int fLuciteHodoscopeVerbosity;
  int fForwardTrackerVerbosity;

private:



  G4int plotVis;
  BETASimulationManager(const int );
  static BETASimulationManager* fgBETASimulationManager;
  int instanceNumber;



};

#endif

