#ifndef BETAProtvinoCalorimeter_HH
#define BETAProtvinoCalorimeter_HH

#include "G4VSensitiveDetector.hh"
#include "BETAProtvinoCalorimeterHit.hh"

class G4HCofThisEvent;
class G4TouchableHistory;
class G4Step;

class BETAProtvinoCalorimeter : public G4VSensitiveDetector {

public:

  // Constructor
  BETAProtvinoCalorimeter(const G4String& name);

  // Destructor
  virtual ~BETAProtvinoCalorimeter();
  
  // Methods
  virtual void Initialize(G4HCofThisEvent* hitsCollectionOfThisEvent);
  
  virtual G4bool ProcessHits(G4Step* aStep,G4TouchableHistory* history);

  virtual void EndOfEvent(G4HCofThisEvent* hitsCollectionOfThisEvent);
  
private:
  
  // Data members
  BETAProtvinoCalorimeterHitsCollection* fHitsCollection;
  G4int fHitsCollectionID;

};




#endif

