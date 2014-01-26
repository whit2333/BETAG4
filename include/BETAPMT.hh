#ifndef BETAPMT_HH
#define BETAPMT_HH
#include "fstream"
//#include <gsl/gsl_interp.h>
//#include <gsl/gsl_errno.h>
//#include <gsl/gsl_spline.h>

#include "G4VSensitiveDetector.hh"
#include "BETAPMTHit.hh"

class G4HCofThisEvent;
class G4TouchableHistory;
class G4Step;

class BETAPMT : public G4VSensitiveDetector {

public:

  // Constructor
  BETAPMT( G4String name);

  // Destructor
  virtual ~BETAPMT();
  
  // Methods
  virtual void Initialize(G4HCofThisEvent* hitsCollectionOfThisEvent);
  
  virtual G4bool ProcessHits(G4Step* aStep,G4TouchableHistory* history);

  virtual void EndOfEvent(G4HCofThisEvent* hitsCollectionOfThisEvent);
  
  bool countAllPhotons;
  bool savePhotonPositions;


private:
  
  G4double QE(G4double photonEnergy);
  G4double lambda[15];
  G4double sensitivity[15];
  // Data members
  G4int HCID;
  BETAPMTHitsCollection* fHitsCollection;
  G4String  detname;
  G4int pmt;
//gsl_interp * alloc;

};




#endif

