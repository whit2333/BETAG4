#ifndef BETASimulationManager_h
#define BETASimulationManager_h 1
#include "G4UserEventAction.hh"
#include "globals.hh"
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
#include "BETAG4EventGenerator.hh"
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
#include "SANEScalers.h"
#include "G4SDParticleFilter.hh"
#include "G4SDManager.hh"
#include "BETAG4EventGenerator.hh"
#include "SANEEventGenerators.hh"

//#include "BETADetectorConstruction.hh"
//#include "BETAPrimaryGeneratorAction.hh"

class BETADetectorConstruction;
class BETAPrimaryGeneratorAction;
class BETARunAction;
class G4Track;
class BETASimulationMessenger;
class BETARun;

/** The simulation manager singleton class.
 * The BETAG4 simulation manager class. The "Analysis" in the class name is a 
 * bit of an artifact. \todo{Rename BETASimulationManager. Not sure what the best name would be... BETAG4Manager?}
 * 
 * The Manager holds all configuration information such as run number, verbosity levels, primary vertex type, target configuration, geometries to construct. 
 *
 * \ingroup Simulation
 * \ingroup Manager
 */
class BETASimulationManager {
   private: 
      BETASimulationManager( );
      static BETASimulationManager* fgBETASimulationManager;

      friend class BETARunAction;
      friend class BETARun;

   protected: 
      BETADetectorConstruction    * fDetConstruction;
      BETAPrimaryGeneratorAction  * fPrimaryGenAction;

      InSANERun   * fInSANERun;
      int           instanceNumber;
      bool          fIsAppendMode;
      int           fRunNumber;
      int           fEventNumber;

      TFile       * fRootFile;
      TTree       * fDetectorTree;
      TTree       * fScalerTree;

      G4double      fBeamEnergy;  
      G4double      fTargetAngle;  

      int           fGasCherenkovVerbosity;
      int           fBigcalVerbosity;
      int           fLuciteHodoscopeVerbosity;
      int           fForwardTrackerVerbosity;

      G4SDParticleFilter      * protonFilter;
      G4SDParticleFilter      * electronFilter;
      G4SDParticleFilter      * opticalPhotonFilter;
      G4SDKineticEnergyFilter * bigcalEnergyFilter;
      G4SDChargedFilter       * chargeFilter;

      BETASimulationMessenger * fSimulationMessenger;

   public:
      BETASimulationMessenger * GetMessenger(){ return(fSimulationMessenger) ;}

      SANEScalers               * fSANEScalers;
      SANEEvents                * fEvents;
      bool                        fSimulateCherenkovOptics;
      bool                        fSimulateTrackerOptics;
      bool                        fSimulateHodoscopeOptics;
      bool                        fSimulateBigcalOptics;
      bool                        fSimulateTrigger;
      int                         fDebugLevel;

      G4MultiFunctionalDetector * fTrackerScoring;
      G4MultiFunctionalDetector * fCherenkovScoring;
      G4MultiFunctionalDetector * fBigcalScoring;
      G4MultiFunctionalDetector * fHodoscopeScoring;

      ForwardTrackerDetector    * fTrackerDetector;
      GasCherenkovDetector      * fCherenkovDetector;
      BigcalDetector            * fBigcalDetector;
      LuciteHodoscopeDetector   * fHodoscopeDetector;

      G4PSFlatSurfaceFlux       * protonSurfFlux;
      G4PSPassageCellCurrent    * electronSurfFlux;
      G4PSPassageCellCurrent    * photonSurfFlux;
      G4PSTrackLength           * electronTracklength;
      G4PSEnergyDeposit         * calEnergyDeposit;
      G4PSPassageCellCurrent    * chargeSurfFlux;

   public:
      ~BETASimulationManager();

      static BETASimulationManager* GetInstance();

      /**  dispose (delete?) */
      static void Dispose();

      /** Set detector's verbosity for debugging purposes. */
      void SetDetectorVerbosity( char * detName, int level);

      /** Get detector's verbosity for debugging purposes. */
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
       *  Only increments some counter in text file.
       */
      int InitializeNewRun();

      void UpdateRun(){
         if(fInSANERun){
            // Beam
            fInSANERun->SetBeamEnergy(  GetBeamEnergy() );
            fInSANERun->fBeamPolarization = 0.6;
            fInSANERun->fAverageBeamCurrent = 90.0;
            fInSANERun->fAverageBeamPolarization = 0.6;
            fInSANERun->fBCM1ChargeAsymmetry = 0.0;
            fInSANERun->fBCM2ChargeAsymmetry = 0.0;
            // Target
            fInSANERun->SetTargetAngle( GetTargetAngle() );
            fInSANERun->fTargetOfflinePolarization = 0.6;
            fInSANERun->fTargetOnlinePolarization = 0.6;
            fInSANERun->fAverageTargetPolarization = 0.6;
            fInSANERun->fTarget = InSANERun::kNH3;
            fInSANERun->fTargetPolarizationSign = InSANERun::kPOSITIVE;
            fInSANERun->fTargetCup = InSANERun::kTOP;
            fInSANERun->fTargetOrientation = InSANERun::kANTIPARALLEL;
            fInSANERun->fTargetField = 5.1;
         }
      }

      G4double GetBeamEnergy(){return(fBeamEnergy);}
      void     SetBeamEnergy(G4double en){fBeamEnergy=en;}
      G4double GetTargetAngle(){   return(fTargetAngle);}
      void     SetTargetAngle(G4double a){fTargetAngle=a;}

      int GetEventNumber() { return(fEventNumber); };

      BETADetectorConstruction   * GetDetectorConstruction(){ return fDetConstruction ; }
      BETAPrimaryGeneratorAction * GetPrimaryGeneratorAction(){ return fPrimaryGenAction ; }

      void SetDetectorConstruction(BETADetectorConstruction * detcon){ fDetConstruction = detcon; }
      void SetPrimaryGeneratorAction(BETAPrimaryGeneratorAction * gen){ fPrimaryGenAction = gen; }


};

#endif

