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
#include "BigcalDetector.h"
#include "GasCherenkovDetector.h"
#include "LuciteHodoscopeDetector.h"
#include "ForwardTrackerDetector.h"
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

/**
 *  Get the simulation manager singleton
 */
  static BETASimulationManager* GetInstance();

/**
 *  dispose (delete?)
 */
  static void Dispose();


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

/** Creates the InSANEDetector Base classes and sets event addresses
 *  used in pedestal and noise simulation
 */
  int AddDetectors(int runNumber = 0) ;


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

/// Pointer to the BETADetectorConstruction 
  BETADetectorConstruction * fConstruction;

/// Pointer to the BETASimulationMessenger 
  BETASimulationMessenger * fSimulationMessenger;

  bool fSimulateCherenkovOptics;
  bool fSimulateTrackerOptics;
  bool fSimulateHodoscopeOptics;

  int  fDebugLevel;

  G4MultiFunctionalDetector* fTrackerScoring;
  G4MultiFunctionalDetector* fCherenkovScoring;
  G4MultiFunctionalDetector* fBigcalScoring;
  G4MultiFunctionalDetector* fHodoscopeScoring;

  ForwardTrackerDetector * fTrackerDetector;
  GasCherenkovDetector * fCherenkovDetector;
  BigcalDetector* fBigcalDetector;
  LuciteHodoscopeDetector* fHodoscopeDetector;

  G4PSFlatSurfaceFlux * protonSurfFlux;
  G4PSPassageCellCurrent * electronSurfFlux;
  G4PSPassageCellCurrent * photonSurfFlux;
  G4PSTrackLength* electronTracklength;
  G4PSEnergyDeposit * calEnergyDeposit;
  G4PSPassageCellCurrent * chargeSurfFlux;


  G4int plotterVisible() {return plotVis; }

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

public:

/// pointer for all output stuff
  SANEEvents * fEvents;

  int GetEventNumber() { return(fEventNumber); };

private: 

  G4int plotVis;
  BETASimulationManager( );
  static BETASimulationManager* fgBETASimulationManager;
  int instanceNumber;
  bool fIsAppendMode;
  BETAG4SimulationRun * fInSANERun;
  int fRunNumber;
  int fEventNumber;

  TFile * fRootFile;
  TTree * fDetectorTree;

  friend class BETARunAction;
  friend class BETARun;

};

#endif

