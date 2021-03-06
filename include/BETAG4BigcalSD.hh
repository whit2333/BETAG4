#ifndef BETAG4BigcalSD_HH
#define BETAG4BigcalSD_HH

#include "G4VSensitiveDetector.hh"

#include "CLHEP/Units/SystemOfUnits.h"
#include "CLHEP/Units/PhysicalConstants.h"
using namespace CLHEP;

#include "BETAG4BigcalHit.hh"
#include "BIGCALGeometryCalculator.h"

class G4HCofThisEvent;
class G4TouchableHistory;
class G4Step;

/**
 * \ingroup Detectors
 */
class BETAG4BigcalSD : public G4VSensitiveDetector {

   private:

      BETAG4BigcalHitsCollection  * fHitsCollection;
      G4int                         fHitsCollectionID;

      BETAG4BigcalHitsCollection  * fTimingHC;
      G4int                         fTimingHCID;

      BETAG4BigcalHitsCollection  * fTriggerHC;
      G4int                         fTriggerHCID;

      BIGCALGeometryCalculator    * fGeoCalc;

   public:

      BETAG4BigcalSD(const G4String& name);
      virtual ~BETAG4BigcalSD();

      virtual void Initialize(G4HCofThisEvent* hitsCollectionOfThisEvent);
      virtual G4bool ProcessHits(G4Step* aStep,G4TouchableHistory* history);
      virtual void EndOfEvent(G4HCofThisEvent* hitsCollectionOfThisEvent);


};




#endif

