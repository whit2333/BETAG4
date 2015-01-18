#include "BETAEventGenerators.hh"

//____________________________________________________________________
ConeEventGenerator::ConeEventGenerator() {
   fCentralTheta     = 40.0;
   fCentralPhi       = 0.0;
   fCentralEnergy    = 2.00;
   fDeltaTheta       = 2.0;
   fDeltaPhi         = 20.0;
   fDeltaEnergy      = 0.400; //GeV
   fUpstreamPosition = 0.0;//cm
}
//____________________________________________________________________
ConeEventGenerator::~ConeEventGenerator() {
}
//____________________________________________________________________



//____________________________________________________________________
void BigcalCenterEventGenerator::Initialize() {
   InSANEFlatInclusiveDiffXSec * fDiffXSec = new InSANEFlatInclusiveDiffXSec();
   fDiffXSec->SetBeamEnergy(fBeamEnergy);
   fDiffXSec->SetParticleType(11);
   fDiffXSec->InitializePhaseSpaceVariables();
   InSANEPhaseSpace * ps = fDiffXSec->GetPhaseSpace(); 
   InSANEPhaseSpaceSampler *  fEventSampler = new InSANEPhaseSpaceSampler(fDiffXSec);
   AddSampler(fEventSampler);
   SetBeamEnergy(fBeamEnergy);
   //CalculateTotalCrossSection();
   Refresh();
   //ps->Print();
   fIsInitialized = true;

}
//____________________________________________________________________

