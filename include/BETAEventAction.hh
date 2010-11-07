#ifndef BETAEventAction_h
#define BETAEventAction_h 1

#include "BETAEventAction.hh"
#include "BETAEventActionMessenger.hh"
#include "BETAPMTHit.hh"
#include "BETAHodoscopePMTHit.hh"
#include "G4UserEventAction.hh"
#include "BETAFrontTrackerHit.hh"
#include "BETASimulationManager.hh"

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
class G4Event;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class BETAEventAction : public G4UserEventAction
{
  public:
    BETAEventAction();
    virtual ~BETAEventAction();

  public:
    virtual void BeginOfEventAction(const G4Event*);
    virtual void EndOfEventAction(const G4Event*);


  private:
    G4int PMTHCID;
    G4int pmt1Count;
    G4int pmt2Count;
    G4int pmt3Count;
    G4int pmt4Count;
    G4int pmt5Count;
    G4int pmt6Count;
    G4int pmt7Count;
    G4int pmt8Count;
    G4int hodoscopePMTHCID;
    G4int hodoscopePMTcount;

    G4double BCTE;
    G4int HHC1ID;
    G4int HHC2ID;
    G4int DHC1ID;
    G4int DHC2ID;
    G4int ECHCID;
    G4int HCHCID;
    G4int numberOfPrim;
    G4int BIGCALID;
    G4int BIGCALID2;
    G4int FTID;

    BETAEventActionMessenger* messenger;
    G4int verboseLevel;

    std::map<G4int, G4THitsMap<G4double>* > fMap;


BETASimulationManager* analysisManager;





  public:
    inline void SetVerbose(G4int val) { verboseLevel = val; }
    inline G4int GetVerbose() const { return verboseLevel; }
 

};
#endif // 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......



    
