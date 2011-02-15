#ifndef BETAG4BigcalSD_HH
#define BETAG4BigcalSD_HH

#include "G4VSensitiveDetector.hh"
#include "BETAG4BigcalHit.hh"

class G4HCofThisEvent;
class G4TouchableHistory;
class G4Step;

class BETAG4BigcalSD : public G4VSensitiveDetector {

public:

  // Constructor
  BETAG4BigcalSD(const G4String& name);

  // Destructor
  virtual ~BETAG4BigcalSD();
  
  // Methods
  virtual void Initialize(G4HCofThisEvent* hitsCollectionOfThisEvent);
  
  virtual G4bool ProcessHits(G4Step* aStep,G4TouchableHistory* history);

  virtual void EndOfEvent(G4HCofThisEvent* hitsCollectionOfThisEvent);
  
private:
  
  // Data members
  BETAG4BigcalHitsCollection* fHitsCollection;
  G4int fHitsCollectionID;

};




#endif

