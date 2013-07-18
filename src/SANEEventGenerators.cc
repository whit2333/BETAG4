#include "SANEEventGenerators.hh"

//____________________________________________________________________
ConeEventGenerator::ConeEventGenerator() {
   fCentralTheta = 40.0;
   fCentralPhi = 0.0;
   fCentralEnergy = 2.00;
   fDeltaTheta = 2.0;
   fDeltaPhi = 20.0;
   fDeltaEnergy = 0.400; //GeV
   fUpstreamPosition = 0.0;//cm
}
//____________________________________________________________________
ConeEventGenerator::~ConeEventGenerator() {
}
//____________________________________________________________________

//____________________________________________________________________
void DISEventGenerator::Initialize(){
   F1F209eInclusiveDiffXSec * fDiffXSec = new  F1F209eInclusiveDiffXSec();
   std::cout << " Beam Energy: " << fBeamEnergy << std::endl;
   fDiffXSec->SetBeamEnergy(fBeamEnergy);
   fDiffXSec->InitializePhaseSpaceVariables();
   fDiffXSec->InitializeFinalStateParticles();
   InSANEPhaseSpaceSampler *  fF1F2EventSampler = new InSANEPhaseSpaceSampler(fDiffXSec);
   AddSampler(fF1F2EventSampler);

   SetBeamEnergy(fBeamEnergy);
   InSANEEventGenerator::Initialize();

   CalculateTotalCrossSection();
}
//____________________________________________________________________

//____________________________________________________________________
void PolarizedDISEventGenerator::Initialize(){
   double Emin = 0.5;
   double Emax = 3.5;
   //----------------------------------
   // Electron Phase Space
   //----------------------------------
   InSANEPhaseSpace * fPolarizedPhaseSpace = new InSANEPhaseSpace();
   InSANEPhaseSpace * fPhaseSpace          = new InSANEPhaseSpace();

   InSANEPhaseSpaceVariable * varEnergy = new InSANEPhaseSpaceVariable("Energy","E#prime"); 
   varEnergy->SetMinimum(Emin);         //GeV
   varEnergy->SetMaximum(Emax); //GeV
   varEnergy->SetVariableUnits("GeV");        //GeV
   fPolarizedPhaseSpace->AddVariable(varEnergy);
   fPhaseSpace->AddVariable(varEnergy);

   InSANEPhaseSpaceVariable *   varTheta = new InSANEPhaseSpaceVariable("theta","#theta");
   varTheta->SetMinimum(10.0*TMath::Pi()/180.0); //
   varTheta->SetMaximum(50.0*TMath::Pi()/180.0); //
   varTheta->SetVariableUnits("rads"); //
   fPolarizedPhaseSpace->AddVariable(varTheta);
   fPhaseSpace->AddVariable(varTheta);

   InSANEPhaseSpaceVariable *   varPhi = new InSANEPhaseSpaceVariable("phi","#phi");
   varPhi->SetMinimum(-90.0*TMath::Pi()/180.0); //
   varPhi->SetMaximum(90.0*TMath::Pi()/180.0); //
   varPhi->SetVariableUnits("rads"); //
   fPolarizedPhaseSpace->AddVariable(varPhi);
   fPhaseSpace->AddVariable(varPhi);

   InSANEDiscretePhaseSpaceVariable *   varHelicity = new InSANEDiscretePhaseSpaceVariable("helicity","#lambda");
   varHelicity->SetNumberOfValues(3); 
   fPolarizedPhaseSpace->AddVariable(varHelicity);

   //---------------------
   // Cross-sections
   //---------------------

   // - Build up electron DIS x-sec
   //  - unpolarized 
   F1F209eInclusiveDiffXSec * fDiffXSec = new  F1F209eInclusiveDiffXSec();
   fDiffXSec->SetBeamEnergy(fBeamEnergy);
   fDiffXSec->SetA(1);
   fDiffXSec->SetZ(1);
   fDiffXSec->SetPhaseSpace(fPhaseSpace);
   fDiffXSec->Refresh();
   //  - polarized 
   PolarizedDISXSecParallelHelicity * fDiffXSec1 = new  PolarizedDISXSecParallelHelicity();
   fDiffXSec1->SetBeamEnergy(fBeamEnergy);
   fDiffXSec1->SetPhaseSpace(fPolarizedPhaseSpace);
   fDiffXSec1->Refresh();
   PolarizedDISXSecAntiParallelHelicity * fDiffXSec2 = new  PolarizedDISXSecAntiParallelHelicity();
   fDiffXSec2->SetBeamEnergy(fBeamEnergy);
   fDiffXSec2->SetPhaseSpace(fPolarizedPhaseSpace);
   fDiffXSec2->Refresh();
   //  - Add cross sections to total cross section
   InSANEPolarizedDiffXSec * fPolXSec = new InSANEPolarizedDiffXSec();
   fPolXSec->SetUnpolarizedCrossSection(fDiffXSec);
   fPolXSec->SetPolarizedPositiveCrossSection(fDiffXSec1);
   fPolXSec->SetPolarizedNegativeCrossSection(fDiffXSec2);
   fPolXSec->SetPhaseSpace(fPolarizedPhaseSpace);
   fPolXSec->SetBeamEnergy(fBeamEnergy);
   fPolXSec->SetChargeAsymmetry(-0.002);
   fPolXSec->SetBeamPolarization(0.66);
   fPolXSec->SetTargetPolarization(0.6);
   fPolXSec->Refresh();


   InSANEPhaseSpaceSampler *  sampler = new InSANEPhaseSpaceSampler(fPolXSec);
   sampler->SetFoamSample(10);
   AddSampler(sampler);
   SetBeamEnergy(fBeamEnergy);

   //InSANEEventGenerator::Initialize();
   CalculateTotalCrossSection();

   fDiffXSec->Print();
   fDiffXSec1->Print();
   fDiffXSec2->Print();
   fPolXSec->Print();
}
//____________________________________________________________________

//____________________________________________________________________
void InclusiveElectronPionGenerator::Initialize(){
   std::cout << " InclusiveElectronPionGenerator::Initialize() \n";
   /// Neutral Pion
   InSANEInclusiveWiserXSec * fDiffXSec1 = new InSANEInclusiveWiserXSec();
   fDiffXSec1->SetBeamEnergy(fBeamEnergy);
   fDiffXSec1->SetProductionParticleType(111);
   fDiffXSec1->InitializePhaseSpaceVariables();
   fDiffXSec1->InitializeFinalStateParticles();
   fDiffXSec1->SetRadiationLength(8.0);
   InSANEPhaseSpaceSampler *  pi0EventSampler = new InSANEPhaseSpaceSampler(fDiffXSec1);
   AddSampler(pi0EventSampler);

   /// Positive Pion
   InSANEInclusiveWiserXSec * fDiffXSec2 = new InSANEInclusiveWiserXSec();
   fDiffXSec2->SetBeamEnergy(fBeamEnergy);
   fDiffXSec2->SetProductionParticleType(211);
   fDiffXSec2->InitializePhaseSpaceVariables();
   fDiffXSec2->InitializeFinalStateParticles();
   fDiffXSec2->SetRadiationLength(8.0);
   InSANEPhaseSpaceSampler *  pi0EventSampler2 = new InSANEPhaseSpaceSampler(fDiffXSec2);
   AddSampler(pi0EventSampler2);

   /// Negative Pion
   InSANEInclusiveWiserXSec * fDiffXSec3 = new InSANEInclusiveWiserXSec();
   fDiffXSec3->SetBeamEnergy(fBeamEnergy);
   fDiffXSec3->SetProductionParticleType(-211);
   fDiffXSec3->InitializePhaseSpaceVariables();
   fDiffXSec3->InitializeFinalStateParticles();
   fDiffXSec3->SetRadiationLength(8.0);
   InSANEPhaseSpaceSampler *  pi0EventSampler3 = new InSANEPhaseSpaceSampler(fDiffXSec3);
   AddSampler(pi0EventSampler3);

   /// DIS
   F1F209eInclusiveDiffXSec * fDiffXSec = new  F1F209eInclusiveDiffXSec();
   fDiffXSec->SetBeamEnergy(fBeamEnergy);
   fDiffXSec->InitializePhaseSpaceVariables();
   fDiffXSec->GetPhaseSpace()->GetVariableWithName("energy")->SetMinimum(0.5);
   fDiffXSec->GetPhaseSpace()->GetVariableWithName("energy")->SetMaximum(4.0);
   fDiffXSec->InitializeFinalStateParticles();
   //      InSANEPhaseSpace *ps = fDiffXSec->GetPhaseSpace(); /// all the following cross sections share the same phase space. 
   //     ps->ListVariables();
   InSANEPhaseSpaceSampler *  fF1F2EventSampler = new InSANEPhaseSpaceSampler(fDiffXSec);
   AddSampler(fF1F2EventSampler);

   SetBeamEnergy(fBeamEnergy);
   InSANEEventGenerator::Initialize();

   CalculateTotalCrossSection();
}
//____________________________________________________________________

//____________________________________________________________________
void MottEventGenerator::Initialize(){
   F1F209eInclusiveDiffXSec * fDiffXSec = new  F1F209eInclusiveDiffXSec();
   fDiffXSec->SetBeamEnergy(fBeamEnergy);
   fDiffXSec->InitializePhaseSpaceVariables();

   InSANEPhaseSpace *ps = fDiffXSec->GetPhaseSpace(); /// all the following cross sections share the same phase space. 
   ps->ListVariables();
   InSANEPhaseSpaceSampler *  fF1F2EventSampler = new InSANEPhaseSpaceSampler(fDiffXSec);
   AddSampler(fF1F2EventSampler);
   CalculateTotalCrossSection();
}
//____________________________________________________________________

//____________________________________________________________________
void WiserEventGenerator::Initialize() {
   InSANEInclusiveWiserXSec * fDiffXSec = new InSANEInclusiveWiserXSec();
   fDiffXSec->SetBeamEnergy(fBeamEnergy);
   fDiffXSec->SetProductionParticleType(111);
   fDiffXSec->InitializeFinalStateParticles();
   InSANEPhaseSpace * ps = fDiffXSec->GetPhaseSpace(); 
   InSANEPhaseSpaceSampler *  fEventSampler = new InSANEPhaseSpaceSampler(fDiffXSec);
   AddSampler(fEventSampler);
   SetBeamEnergy(fBeamEnergy);
   InSANEEventGenerator::Initialize();
   CalculateTotalCrossSection();
   ps->Print();
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
      InSANEEventGenerator::Initialize();
      CalculateTotalCrossSection();
      ps->Print();

   }
//____________________________________________________________________

//____________________________________________________________________
