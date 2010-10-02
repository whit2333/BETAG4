#ifndef BETAFrontTracker_HH
#define BETAFrontTracker_HH
#include "BETAFrontTracker.hh"
#include "G4VSensitiveDetector.hh"
#include "BETAFrontTrackerHit.hh"

class G4HCofThisEvent;
class G4TouchableHistory;
class G4Step;

class BETAFrontTracker : public G4VSensitiveDetector {

public:

  // Constructor
  BETAFrontTracker( G4String name);

  // Destructor
  virtual ~BETAFrontTracker();
  
  // Methods
  virtual void Initialize(G4HCofThisEvent* hitsCollectionOfThisEvent);
  
  virtual G4bool ProcessHits(G4Step* aStep,G4TouchableHistory* history);

  virtual void EndOfEvent(G4HCofThisEvent* hitsCollectionOfThisEvent);
  
private:
  
  
  G4int HCID;
  BETAFrontTrackerHitsCollection* fHitsCollection;
  G4String  detname;
  G4int pmt;


};




#endif

