#ifndef BETAHodoscopePMT_HH
#define BETAHodoscopePMT_HH
#include "fstream"
//#include <gsl/gsl_interp.h>
//#include <gsl/gsl_errno.h>
//#include <gsl/gsl_spline.h>

#include "G4VSensitiveDetector.hh"
#include "BETAHodoscopePMTHit.hh"

class G4HCofThisEvent;
class G4TouchableHistory;
class G4Step;

/**
 * \ingroup Detectors
 */
class BETAHodoscopePMT : public G4VSensitiveDetector {
   
   public:
      G4int                            fDiscriminatorThreshold;
      G4double                         lambda[15];
      G4double                         sensitivity[15];
      G4int                            HCID;
      BETAHodoscopePMTHitsCollection*  fHitsCollection;
      G4String                         detname;
      G4int                            pmtNumber;
      G4int                            barNumber;

   public:

      BETAHodoscopePMT( G4String name);

      virtual ~BETAHodoscopePMT();

      virtual void Initialize(G4HCofThisEvent* hitsCollectionOfThisEvent);

      virtual G4bool ProcessHits(G4Step* aStep,G4TouchableHistory* history);

      virtual void EndOfEvent(G4HCofThisEvent* hitsCollectionOfThisEvent);



};




#endif

