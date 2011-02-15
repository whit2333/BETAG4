#ifndef BETARCSCalorimeter_HH
#define BETARCSCalorimeter_HH

#include "G4VSensitiveDetector.hh"
#include "BETARCSCalorimeterHit.hh"

class G4HCofThisEvent;
class G4TouchableHistory;
class G4Step;

class BETARCSCalorimeter : public G4VSensitiveDetector {

public:

  // Constructor
  BETARCSCalorimeter(const G4String& name);

  // Destructor
  virtual ~BETARCSCalorimeter();
  
  // Methods
  virtual void Initialize(G4HCofThisEvent* hitsCollectionOfThisEvent);
  
  virtual G4bool ProcessHits(G4Step* aStep,G4TouchableHistory* history);

  virtual void EndOfEvent(G4HCofThisEvent* hitsCollectionOfThisEvent);
  
private:
  
  // Data members
  BETARCSCalorimeterHitsCollection* fHitsCollection;
  G4int fHitsCollectionID;

};




#endif

