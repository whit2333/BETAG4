#ifndef BETAFakePlane_HH
#define BETAFakePlane_HH
#include "fstream"
#include <gsl/gsl_interp.h>
#include <gsl/gsl_errno.h>
#include <gsl/gsl_spline.h>

#include "G4VSensitiveDetector.hh"
#include "BETAFakePlaneHit.hh"
#include "G4VPhysicalVolume.hh"

class G4HCofThisEvent;
class G4TouchableHistory;
class G4Step;

/**  Fake plane Sensitive Detector
 *  
 * \ingroup Detectors
 */
class BETAFakePlane : public G4VSensitiveDetector {
public:
   BETAFakePlane( G4String name);
   virtual ~BETAFakePlane();

   virtual void Initialize(G4HCofThisEvent* hitsCollectionOfThisEvent);

   virtual G4bool ProcessHits(G4Step* aStep,G4TouchableHistory* history);

   virtual void EndOfEvent(G4HCofThisEvent* hitsCollectionOfThisEvent);

   G4VPhysicalVolume * GetSensitiveVolume(){ return fSensitiveVolume; }
   void                SetSensitiveVolume(G4VPhysicalVolume * vol){ fSensitiveVolume = vol; }

protected:
   G4VPhysicalVolume * fSensitiveVolume;

private:
  G4int fPID; 
  G4int HCID;
  BETAFakePlaneHitsCollection* fHitsCollection;
  G4String  detname;
  G4int pid; 
  G4int energy;

};




#endif

