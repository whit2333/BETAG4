#ifndef BETAMirror_HH
#define BETAMirror_HH
#include "fstream"
//#include <gsl/gsl_interp.h>
//#include <gsl/gsl_errno.h>
//#include <gsl/gsl_spline.h>

#include "G4VSensitiveDetector.hh"
#include "BETAMirrorHit.hh"

class G4HCofThisEvent;
class G4TouchableHistory;
class G4Step;

/**
 *
 * \ingroup Detectors
 */
class BETAMirror : public G4VSensitiveDetector {

public:

  // Constructor
  BETAMirror( G4String name);

  // Destructor
  virtual ~BETAMirror();
  
  // Methods
  virtual void Initialize(G4HCofThisEvent* hitsCollectionOfThisEvent);
  
  virtual G4bool ProcessHits(G4Step* aStep,G4TouchableHistory* history);

  virtual void EndOfEvent(G4HCofThisEvent* hitsCollectionOfThisEvent);
  
private:
  
  // Data members
  G4int HCID;
  BETAMirrorHitsCollection* fHitsCollection;
  G4String  detname;
  G4int mirror;
  G4int pid; // 1 electron, 2 positron
  G4int energy;

};




#endif

