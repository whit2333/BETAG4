#include "BETAG4EventGenerator.hh"


BETAG4EventGenerator::BETAG4EventGenerator() {
   fNumberOfGeneratedParticles = 1;
//       fDiffXSec=0;
//       fEventSampler=0;
//       fPhaseSpace=0;
   fNeedsRefreshed = false;
     	varTheta=0;
      varPhi=0;
      varEnergy=0;
      var=0;
      fPrimaryPS=0;
   fIsInitialized = false;
     fBeamEnergy=5.9;

//       fCentralTheta = 10.0;
//       fCentralPhi = -20.0;
//       fCentralEnergy = 0.30;
//       fDeltaTheta = 1.0;
//       fDeltaPhi = 1.0;
//       fDeltaEnergy = 0.1;

      fInitialPosition = new G4ThreeVector(0,0,0);
      fInitialDirection = new G4ThreeVector(0,0,0);
      fMomentumVector = new G4ThreeVector(0,0,0);


}


