#ifndef BETAFakePlane_HH
#define BETAFakePlane_HH
#include "fstream"
#include <gsl/gsl_interp.h>
#include <gsl/gsl_errno.h>
#include <gsl/gsl_spline.h>

#include "G4VSensitiveDetector.hh"
#include "BETAFakePlaneHit.hh"

class G4HCofThisEvent;
class G4TouchableHistory;
class G4Step;

class BETAFakePlane : public G4VSensitiveDetector {

public:

  // Constructor
  BETAFakePlane( G4String name);

  // Destructor
  virtual ~BETAFakePlane();
  
  // Methods
  virtual void Initialize(G4HCofThisEvent* hitsCollectionOfThisEvent);
  
  virtual G4bool ProcessHits(G4Step* aStep,G4TouchableHistory* history);

  virtual void EndOfEvent(G4HCofThisEvent* hitsCollectionOfThisEvent);
  
private:
  
  // Data members
  G4int HCID;
  BETAFakePlaneHitsCollection* fHitsCollection;
  G4String  detname;
  G4int pid; // 1 electron, 2 positron
  G4int energy;

};




#endif

