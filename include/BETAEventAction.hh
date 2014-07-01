#ifndef BETAEventAction_h
#define BETAEventAction_h 1

//#include "BETAEventActionMessenger.hh"
//#include "BETAPMTHit.hh"
//#include "BETAHodoscopePMTHit.hh"
#include "G4UserEventAction.hh"
#include "BETASimulationManager.hh"

#include "G4RunManager.hh"
#include "G4Event.hh"
#include "G4EventManager.hh"
#include "G4HCofThisEvent.hh"
#include "G4VHitsCollection.hh"
#include "G4TrajectoryContainer.hh"
#include "G4Trajectory.hh"
#include "G4VVisManager.hh"
#include "G4SDManager.hh"
#include "G4UImanager.hh"
#include "G4ios.hh"
#include "G4THitsMap.hh"

//#include "BETAProtvinoCalorimeterHit.hh"
//#include "BETARCSCalorimeterHit.hh"
//#include "BETAForwardTrackerHit.hh"
//#include "BETAPrimaryGeneratorAction.hh"
//#include "BETADetectorConstruction.hh"
//#include "BETAForwardTrackerHit.hh"
//#include "BETAG4BigcalSD.hh"
//#include "BETAG4BigcalHit.hh"
//#include "BETAHodoscopePMTHit.hh"
//#include "BETAPMTHit.hh"
//#include "BETAFakePlaneHit.hh"
//#include "BETAMirrorHit.hh"


class G4Event;

class BETAEventAction : public G4UserEventAction {

   private:

      //BETAHodoscopePMTHitsCollection * fHodoscopepmtHC;
      //BETAPMTHitsCollection          * fCherenkovHC;
      //BETAG4BigcalHitsCollection     * fBigcalHC;
      //G4int                            fBigcalHCID;
      //BETAProtvinoCalorimeterHitsCollection * BIGCALHC2;
      //BETAForwardTrackerHitsCollection      * FTHC;
      //BETAMirrorHitsCollection              * mirrorHC ;
      //BETAFakePlaneHitsCollection           * fakePlaneHC;

      //G4int PMTHCID;
      //G4int pmt1Count;
      //G4int pmt2Count;
      //G4int pmt3Count;
      //G4int pmt4Count;
      //G4int pmt5Count;
      //G4int pmt6Count;
      //G4int pmt7Count;
      //G4int pmt8Count;
      //G4int hodoscopePMTHCID;
      //G4int hodoscopePMTcount;

      //G4double BCTE;
      //G4int HHC1ID;
      //G4int HHC2ID;
      //G4int DHC1ID;
      //G4int DHC2ID;
      //G4int ECHCID;
      //G4int HCHCID;
      //G4int numberOfPrim;
      //G4int BIGCALID;
      //G4int BIGCALID2;
      //G4int FTID;

      //BETAEventActionMessenger* messenger;
      G4int verboseLevel;

      std::map<G4int, G4THitsMap<G4double>* > fMap;

   private:
      BETASimulationManager * fSimulationManager;
      G4RunManager * runManager;
      BETAPrimaryGeneratorAction * generator;
      BETADetectorConstruction * construction;


   public:
      BETAEventAction();
      virtual ~BETAEventAction();

   public:
      virtual void BeginOfEventAction(const G4Event*);
      virtual void EndOfEventAction(const G4Event*);

   public:
      inline void SetVerbose(G4int val) { verboseLevel = val; }
      inline G4int GetVerbose() const { return verboseLevel; }


};

#endif


