#ifndef BETASimulationManager_h
#define BETASimulationManager_h 1

#include "G4UserEventAction.hh"
#include "globals.hh"
#include "BETASimulationMessenger.hh"
#include <fstream>
#include "G4PSFlatSurfaceFlux.hh"
#include "G4PSPassageCellCurrent.hh"
#include "G4PSTrackLength.hh"
#include "G4PSTrackLength.hh"
#include "G4PSPassageTrackLength.hh"
#include "G4PSEnergyDeposit.hh"

#include "G4SDKineticEnergyFilter.hh"
#include "G4SDChargedFilter.hh"

#include "HallCBeamEvent.h"
#include "HMSEvent.h"
#include "BETAEvent.h"
#include "BETAG4MonteCarloEvent.h"
#include "GasCherenkovEvent.h"
#include "BigcalEvent.h"
#include "GasCherenkovHit.h"
#include "BETAG4SimulationRun.h"
#include "G4ScoringManager.hh"
#include "SANEEvents.h"
class BETARunAction;
class G4Track;
class BETASimulationMessenger;
class BETARun;
/**
 * \ingroup Simulation
 */
/**
 * \ingroup Manager
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

  static BETASimulationManager* GetInstance();
  static void Dispose();
  BETADetectorConstruction * fConstruction;
  BETASimulationMessenger * fSimulationMessenger;

bool fSimulateCherenkovOptics;
bool fSimulateTrackerOptics;
bool fSimulateHodoscopeOptics;

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

/**
 * Allocate Event and Hit memory
 */
 int AllocateTreeMemory();

/**
 * Free Event and Hit memory
 */
 int Reset();




/**
 * Sets up detector's scoring
 * This defines what should be detected but not which volume it is associated with
 * this should be done within the det const class
 */
  int InitScoring();

/**
 *  Defines the scoring filters
 *  This must come during detector construction...??
 */ 
  int DefineScoringFilters();

/**
 *  
 */
  int InitializeNewRun();



public:
G4MultiFunctionalDetector* fTrackerDetector;
G4MultiFunctionalDetector* fCherenkovDetector;
G4MultiFunctionalDetector* fBigcalDetector;
G4MultiFunctionalDetector* fHodoscopeDetector;

  G4PSFlatSurfaceFlux * protonSurfFlux;
  G4PSPassageCellCurrent * electronSurfFlux;
  G4PSPassageCellCurrent * photonSurfFlux;
  G4PSTrackLength* electronTracklength;
  G4PSEnergyDeposit * calEnergyDeposit;
  G4PSPassageCellCurrent * chargeSurfFlux;


inline G4int plotterVisible() {return plotVis; }

private: 
  int fGasCherenkovVerbosity;
  int fBigcalVerbosity;
  int fLuciteHodoscopeVerbosity;
  int fForwardTrackerVerbosity;

  G4SDParticleFilter* protonFilter;
  G4SDParticleFilter* electronFilter;
  G4SDParticleFilter* opticalPhotonFilter;
  G4SDKineticEnergyFilter* bigcalEnergyFilter;
  G4SDChargedFilter* chargeFilter;

/// Sets/creates the tree branches for append mode or new file mode
int SetTreeBranches();

/// Creates the standard trees
int CreateTrees();

SANEEvents * fEvents;
/// Contains all BETA detector's data
  BETAEvent * betaEvent;
/// Contains all the HMS data
  HMSEvent* hmsEvent;
/// Contains all the Hall C beam data
  HallCBeamEvent * beamEvent;
/// Contains all the Thrown and unrealistically obtained Montecarlo data
  BETAG4MonteCarloEvent * mcEvent;
/// Simulation Run object
  BETAG4SimulationRun * fInSANERun;



  G4int plotVis;
  BETASimulationManager( );
  static BETASimulationManager* fgBETASimulationManager;
  int instanceNumber;
  int fRunNumber;
  bool fIsAppendMode;

  TFile * fRootFile;
  TTree * fDetectorTree;

  friend class BETARunAction;
  friend class BETARun;

};

#endif

