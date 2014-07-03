#include "BETAG4EventGenerator.hh"

//______________________________________________________________________________
BETAG4EventGenerator::BETAG4EventGenerator() {
   fNumberOfGeneratedParticles = 1;
   //fNeedsRefreshed             = false;
   fIsInitialized              = false;
   fBeamEnergy                 = 5.9;
   fInitialPosition            = new G4ThreeVector(0,0,0);
   fInitialDirection           = new G4ThreeVector(0,0,0);
   fMomentumVector             = new G4ThreeVector(0,0,0);

   // Hard coded use of fast structure functions
   InSANEFunctionManager * fm = InSANEFunctionManager::GetInstance();
   fm->CreateSFs(6);
   fm->CreatePolSFs(6);

   InSANESimpleTarget * target = new InSANESimpleTarget("LH2Target","Simple LH2 Target");
   SetTarget(target);
}
//________________________________________________________________________________
BETAG4EventGenerator::~BETAG4EventGenerator(){
   delete fInitialPosition;
   delete fInitialDirection;
   delete fMomentumVector;
}
//________________________________________________________________________________
void BETAG4EventGenerator::Initialize(){
   InSANETargetEventGenerator::Initialize();
   // Initialize is likely to be overloaded later.
   // here we provide a simple initialization

   // Create the cross section
   //F1F209eInclusiveDiffXSec * fDiffXSec = new  F1F209eInclusiveDiffXSec();
   //InSANEInclusiveDISXSec * fDiffXSec = new  InSANEInclusiveDISXSec();
   //fDiffXSec->SetBeamEnergy(fBeamEnergy);
   //fDiffXSec->InitializePhaseSpaceVariables();
   ////fPrimaryPS = fDiffXSec->GetPhaseSpace(); /// all the following cross sections share the same phase space. 
   ////fPrimaryPS->ListVariables();

   //// Create the sampler 
   //InSANEPhaseSpaceSampler *  disEventSampler = new InSANEPhaseSpaceSampler(fDiffXSec);
   //// Add sampler to event generator
   //AddSampler(disEventSampler);

   //CalculateTotalCrossSection();

}
//________________________________________________________________________________
void BETAG4EventGenerator::InitializeMaterialXSec(const Int_t i, const Double_t weight, 
      const InSANETargetMaterial * mat, const InSANENucleus * targ){
   // This is the one where you set all the cross sections etc... 
   InSANETargetEventGenerator::InitializeMaterialXSec(i,weight,mat,targ);
}
//________________________________________________________________________________
G4ThreeVector&  BETAG4EventGenerator::GetInitialPosition(const TParticle * p){
   fInitialPosition->setX(p->Vx());
   fInitialPosition->setY(p->Vy());
   fInitialPosition->setZ(p->Vz());
   return(*fInitialPosition);
}
//________________________________________________________________________________
G4ThreeVector&  BETAG4EventGenerator::GetInitialDirection(const TParticle * p){
   fInitialDirection->setX(p->Px());
   fInitialDirection->setY(p->Py());
   fInitialDirection->setZ(p->Pz());
   return(*fInitialDirection);
}
//________________________________________________________________________________

/// \todo assuming particle is electron (eg E=P))
G4ThreeVector&  BETAG4EventGenerator::GetMomentumVector(const TParticle * p){
   // \deprecated
   fInitialDirection->setX(p->Px());
   fInitialDirection->setY(p->Py());
   fInitialDirection->setZ(p->Pz());
   return(*fMomentumVector);
}
//________________________________________________________________________________
double BETAG4EventGenerator::GetParticleEnergy(const TParticle * p) {
   return(p->Energy());
}
//________________________________________________________________________________
void BETAG4EventGenerator::SetThetaMax(double val) { 
   fThetaMax = val;
   SetModified(true);
   TList * vars = GetPSVariables("theta");
   for(int i = 0; i< vars->GetEntries() ; i++){
      InSANEPhaseSpaceVariable * aVar = (InSANEPhaseSpaceVariable*) vars->At(i);
      aVar->SetMaximum(val);
   }
   if( vars->GetEntries() == 0) Error("SetThetaMax","\"theta\" variable does not exist.");
}
//________________________________________________________________________________
void BETAG4EventGenerator::SetThetaMin(double val) { 
   fThetaMin = val;
   SetModified(true);
   TList * vars = GetPSVariables("theta");
   for(int i = 0; i< vars->GetEntries() ; i++){
      InSANEPhaseSpaceVariable * aVar = (InSANEPhaseSpaceVariable*) vars->At(i);
      aVar->SetMinimum(val);
   }
   if( vars->GetEntries() == 0) Error("SetThetaMin","\"theta\" variable does not exist.");
}
//________________________________________________________________________________
void BETAG4EventGenerator::SetPhiMax(double val) { 
   fPhiMax = val;
   SetModified(true);
   TList * vars = GetPSVariables("phi");
   for(int i = 0; i< vars->GetEntries() ; i++){
      InSANEPhaseSpaceVariable * aVar = (InSANEPhaseSpaceVariable*) vars->At(i);
      aVar->SetMaximum(val);
   }
   if( vars->GetEntries() == 0) Error("SetPhiMax","\"phi\" variable does not exist.");
}
//________________________________________________________________________________
void BETAG4EventGenerator::SetPhiMin(double val) { 
   fPhiMin = val;
   SetModified(true);
   TList * vars = GetPSVariables("phi");
   for(int i = 0; i< vars->GetEntries() ; i++){
      InSANEPhaseSpaceVariable * aVar = (InSANEPhaseSpaceVariable*) vars->At(i);
      aVar->SetMinimum(val);
   }
   if( vars->GetEntries() == 0) Error("SetPhiMin","\"phi\" variable does not exist.");
}
//________________________________________________________________________________
void BETAG4EventGenerator::SetEnergyMax(double val) { 
   fEnergyMax = val;
   SetModified(true);
   TList * vars = GetPSVariables("energy");
   for(int i = 0; i< vars->GetEntries() ; i++){
      InSANEPhaseSpaceVariable * aVar = (InSANEPhaseSpaceVariable*) vars->At(i);
      aVar->SetMaximum(val);
   }
   if( vars->GetEntries() == 0) Error("SetEnergyMax","\"energy\" variable does not exist.");
}
//________________________________________________________________________________
void BETAG4EventGenerator::SetEnergyMin(double val) { 
   fEnergyMin = val;
   SetModified(true);
   TList * vars = GetPSVariables("energy");
   for(int i = 0; i< vars->GetEntries() ; i++){
      InSANEPhaseSpaceVariable * aVar = (InSANEPhaseSpaceVariable*) vars->At(i);
      aVar->SetMinimum(val);
   }
   if( vars->GetEntries() == 0) Error("SetEnergyMax","\"energy\" variable does not exist.");
}
//________________________________________________________________________________
void BETAG4EventGenerator::SetMomentumMax(double val) { 
   fMomentumMax = val;
   SetModified(true);
   TList * vars = GetPSVariables("momentum");
   for(int i = 0; i< vars->GetEntries() ; i++){
      InSANEPhaseSpaceVariable * aVar = (InSANEPhaseSpaceVariable*) vars->At(i);
      aVar->SetMaximum(val);
   }
   if( vars->GetEntries() == 0) Error("SetMomentumMax","\"momentum\" variable does not exist.");
}
//________________________________________________________________________________
void BETAG4EventGenerator::SetMomentumMin(double val) { 
   fMomentumMin = val;
   SetModified(true);
   TList * vars = GetPSVariables("momentum");
   for(int i = 0; i< vars->GetEntries() ; i++){
      InSANEPhaseSpaceVariable * aVar = (InSANEPhaseSpaceVariable*) vars->At(i);
      aVar->SetMinimum(val);
   }
   if( vars->GetEntries() == 0) Error("SetMomentumMax","\"momentum\" variable does not exist.");
}
//________________________________________________________________________________


