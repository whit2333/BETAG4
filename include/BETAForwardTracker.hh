#ifndef BETAForwardTracker_HH
#define BETAForwardTracker_HH
#include "BETAForwardTracker.hh"
#include "G4VSensitiveDetector.hh"
#include "BETAForwardTrackerHit.hh"

class G4HCofThisEvent;
class G4TouchableHistory;
class G4Step;

/**
 *
 * \ingroup Detectors
 */
class BETAForwardTracker : public G4VSensitiveDetector {

public:

  // Constructor
  BETAForwardTracker( G4String name);

  // Destructor
  virtual ~BETAForwardTracker();
  
  // Methods
  virtual void Initialize(G4HCofThisEvent* hitsCollectionOfThisEvent);

/// We are counting the photons at both ends of the tracker...
  virtual G4bool ProcessHits(G4Step* aStep,G4TouchableHistory* history);

  virtual void EndOfEvent(G4HCofThisEvent* hitsCollectionOfThisEvent);
  
private:
  
  
  G4int HCID;
  BETAForwardTrackerHitsCollection* fHitsCollection;
  G4String  detname;
  G4int pmt;


};




#endif

