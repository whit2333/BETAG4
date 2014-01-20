#include "BETAG4EventGenerator.hh"

BETAG4EventGenerator::BETAG4EventGenerator() {
   fNumberOfGeneratedParticles = 1;
   //fDiffXSec=0;
   //fEventSampler=0;
   //fPhaseSpace=0;
   fNeedsRefreshed = false;
   //varTheta=0;
   //varPhi=0;
   //varEnergy=0;
   //var=0;
   //fPrimaryPS=0;
   fIsInitialized = false;
   fBeamEnergy=5.9;

   //fCentralTheta = 10.0;
   //fCentralPhi = -20.0;
   //fCentralEnergy = 0.30;
   //fDeltaTheta = 1.0;
   //fDeltaPhi = 1.0;
   //fDeltaEnergy = 0.1;

   fInitialPosition = new G4ThreeVector(0,0,0);
   fInitialDirection = new G4ThreeVector(0,0,0);
   fMomentumVector = new G4ThreeVector(0,0,0);

   InSANEFunctionManager * fm = InSANEFunctionManager::GetInstance();
   fm->CreateSFs(6);
   fm->CreatePolSFs(6);

}
//________________________________________________________________________________
BETAG4EventGenerator::~BETAG4EventGenerator(){

}
//________________________________________________________________________________

void BETAG4EventGenerator::Initialize(){

   /// Create the cross section
   F1F209eInclusiveDiffXSec * fDiffXSec = new  F1F209eInclusiveDiffXSec();
   fDiffXSec->SetBeamEnergy(fBeamEnergy);
   /// Create the phase space
   fDiffXSec->InitializePhaseSpaceVariables();
   //      fPrimaryPS = fDiffXSec->GetPhaseSpace(); /// all the following cross sections share the same phase space. 
   //      fPrimaryPS->ListVariables();

   /// Create the sampler 
   InSANEPhaseSpaceSampler *  fF1F2EventSampler = new InSANEPhaseSpaceSampler(fDiffXSec);
   /// Add sampler to event generator
   AddSampler(fF1F2EventSampler);

   CalculateTotalCrossSection();

}
//________________________________________________________________________________

