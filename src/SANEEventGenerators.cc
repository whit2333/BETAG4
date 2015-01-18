#include "SANEEventGenerators.hh"
#include "QuasiElasticInclusiveDiffXSec.h"
#include "InSANERadiator.h"

//____________________________________________________________________
void DISEventGenerator::Initialize(){
   F1F209eInclusiveDiffXSec * fDiffXSec = new  F1F209eInclusiveDiffXSec();
   //std::cout << " Beam Energy: " << fBeamEnergy << std::endl;
   fDiffXSec->SetBeamEnergy(fBeamEnergy);
   fDiffXSec->InitializePhaseSpaceVariables();
   fDiffXSec->InitializeFinalStateParticles();
   InSANEPhaseSpaceSampler *  fF1F2EventSampler = new InSANEPhaseSpaceSampler(fDiffXSec);
   AddSampler(fF1F2EventSampler);

   SetBeamEnergy(fBeamEnergy);
   
   //InSANEEventGenerator::Initialize();

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

   InSANEPhaseSpaceVariable * varEnergy = new InSANEPhaseSpaceVariable("energy","E#prime"); 
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
   fPolXSec->SetChargeAsymmetry(-0.0001);
   fPolXSec->SetBeamPolarization(0.6);
   fPolXSec->SetTargetPolarization(0.6);
   fPolXSec->Refresh();


   InSANEPhaseSpaceSampler *  sampler = new InSANEPhaseSpaceSampler(fPolXSec);
   sampler->SetFoamSample(10);
   AddSampler(sampler);
   SetBeamEnergy(fBeamEnergy);

   //InSANEEventGenerator::Initialize();
   CalculateTotalCrossSection();

   //fDiffXSec->Print();
   //fDiffXSec1->Print();
   //fDiffXSec2->Print();
   //fPolXSec->Print();
   fIsInitialized = true;
}
//____________________________________________________________________

//____________________________________________________________________
//void InclusiveElectronPionGenerator::Initialize(){
//   //std::cout << " InclusiveElectronPionGenerator::Initialize() \n";
//   /// Neutral Pion
//   //InSANEInclusiveWiserXSec * fDiffXSec1 = new InSANEInclusiveWiserXSec();
//   PhotoOARPionDiffXSec * fDiffXSec1 = new PhotoOARPionDiffXSec();
//   fDiffXSec1->SetBeamEnergy(fBeamEnergy);
//   //fDiffXSec1->SetProductionParticleType(111);
//   fDiffXSec1->InitializePhaseSpaceVariables();
//   fDiffXSec1->InitializeFinalStateParticles();
//   //fDiffXSec1->SetRadiationLength(8.0);
//   InSANEPhaseSpaceSampler *  pi0EventSampler = new InSANEPhaseSpaceSampler(fDiffXSec1);
//   AddSampler(pi0EventSampler);
//
//   ///// Positive Pion
//   //InSANEInclusiveWiserXSec * fDiffXSec2 = new InSANEInclusiveWiserXSec();
//   //fDiffXSec2->SetBeamEnergy(fBeamEnergy);
//   //fDiffXSec2->SetProductionParticleType(211);
//   //fDiffXSec2->InitializePhaseSpaceVariables();
//   //fDiffXSec2->InitializeFinalStateParticles();
//   //fDiffXSec2->SetRadiationLength(0.05);
//   //InSANEPhaseSpaceSampler *  pi0EventSampler2 = new InSANEPhaseSpaceSampler(fDiffXSec2);
//   //AddSampler(pi0EventSampler2);
//
//   ///// Negative Pion
//   //InSANEInclusiveWiserXSec * fDiffXSec3 = new InSANEInclusiveWiserXSec();
//   //fDiffXSec3->SetBeamEnergy(fBeamEnergy);
//   //fDiffXSec3->SetProductionParticleType(-211);
//   //fDiffXSec3->InitializePhaseSpaceVariables();
//   //fDiffXSec3->InitializeFinalStateParticles();
//   //fDiffXSec3->SetRadiationLength(0.05);
//   //InSANEPhaseSpaceSampler *  pi0EventSampler3 = new InSANEPhaseSpaceSampler(fDiffXSec3);
//   //AddSampler(pi0EventSampler3);
//
//   /// DIS
//   F1F209eInclusiveDiffXSec * fDiffXSec = new  F1F209eInclusiveDiffXSec();
//   fDiffXSec->SetBeamEnergy(fBeamEnergy);
//   fDiffXSec->InitializePhaseSpaceVariables();
//   fDiffXSec->GetPhaseSpace()->GetVariableWithName("energy")->SetMinimum(0.5);
//   fDiffXSec->GetPhaseSpace()->GetVariableWithName("energy")->SetMaximum(4.0);
//   fDiffXSec->InitializeFinalStateParticles();
//   //      InSANEPhaseSpace *ps = fDiffXSec->GetPhaseSpace(); /// all the following cross sections share the same phase space. 
//   //     ps->ListVariables();
//   InSANEPhaseSpaceSampler *  fF1F2EventSampler = new InSANEPhaseSpaceSampler(fDiffXSec);
//   AddSampler(fF1F2EventSampler);
//
//   SetBeamEnergy(fBeamEnergy);
//   InSANEEventGenerator::Initialize();
//
//   CalculateTotalCrossSection();
//   fIsInitialized = true;
//}
//____________________________________________________________________

//____________________________________________________________________
void InclusivePionEventGenerator::Initialize(){
   //std::cout << " InclusivePionEventGenerator::Initialize() \n";
   /// Neutral Pion
   InSANEInclusiveWiserXSec * fDiffXSec1 = new InSANEInclusiveWiserXSec();
   //InSANEInclusiveEPCVXSec * fDiffXSec1 = new InSANEInclusiveEPCVXSec();
   fDiffXSec1->SetBeamEnergy(fBeamEnergy);
   fDiffXSec1->SetProductionParticleType(111);
   fDiffXSec1->InitializePhaseSpaceVariables();
   fDiffXSec1->InitializeFinalStateParticles();
   fDiffXSec1->SetRadiationLength(8.0);
   InSANEPhaseSpaceSampler *  pi0EventSampler = new InSANEPhaseSpaceSampler(fDiffXSec1);
   AddSampler(pi0EventSampler);

   /// Positive Pion
   InSANEInclusiveWiserXSec * fDiffXSec2 = new InSANEInclusiveWiserXSec();
   //InSANEInclusiveEPCVXSec * fDiffXSec2 = new InSANEInclusiveEPCVXSec();
   fDiffXSec2->SetBeamEnergy(fBeamEnergy);
   fDiffXSec2->SetProductionParticleType(211);
   fDiffXSec2->InitializePhaseSpaceVariables();
   fDiffXSec2->InitializeFinalStateParticles();
   fDiffXSec2->SetRadiationLength(8.0);
   InSANEPhaseSpaceSampler *  pi0EventSampler2 = new InSANEPhaseSpaceSampler(fDiffXSec2);
   AddSampler(pi0EventSampler2);

   /// Negative Pion
   InSANEInclusiveWiserXSec * fDiffXSec3 = new InSANEInclusiveWiserXSec();
   //InSANEInclusiveEPCVXSec * fDiffXSec3 = new InSANEInclusiveEPCVXSec();
   fDiffXSec3->SetBeamEnergy(fBeamEnergy);
   fDiffXSec3->SetProductionParticleType(-211);
   fDiffXSec3->InitializePhaseSpaceVariables();
   fDiffXSec3->InitializeFinalStateParticles();
   fDiffXSec3->SetRadiationLength(8.0);
   InSANEPhaseSpaceSampler *  pi0EventSampler3 = new InSANEPhaseSpaceSampler(fDiffXSec3);
   AddSampler(pi0EventSampler3);

   SetBeamEnergy(fBeamEnergy);

   //InSANEEventGenerator::Initialize();

   CalculateTotalCrossSection();
   fIsInitialized = true;
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
   fIsInitialized = true;
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
   CalculateTotalCrossSection();
   //ps->Print();
   fIsInitialized = true;
}
//____________________________________________________________________



//______________________________________________________________________________
InclusiveElectronPionGenerator::InclusiveElectronPionGenerator(InSANETarget * targ){ 

   if(!targ) {
    targ = new UVAPolarizedAmmoniaTarget("UVaTarget","UVa Ammonia target",0.6);
   }
   SetTarget(targ);
   //InSANEFunctionManager::GetManager()->CreateSFs(1); // 1=CTEQ
   InSANEFunctionManager::GetManager()->CreateSFs(11); // 11=composite
   //InSANEFunctionManager::GetManager()->CreateSFs(2); // 2=NMC95
}
//______________________________________________________________________________
InclusiveElectronPionGenerator::~InclusiveElectronPionGenerator(){
}
//______________________________________________________________________________
void InclusiveElectronPionGenerator::InitializeMaterialXSec(const Int_t i, const Double_t weight, const InSANETargetMaterial * mat, const InSANENucleus * targ){
   InSANEPhaseSpaceSampler * samp = 0;

   //InSANERadiator<InSANEInclusiveDiffXSec> * xsec = new InSANERadiator<InSANEInclusiveDiffXSec>();
   //InSANERadiator<F1F209eInclusiveDiffXSec> * xsec = new InSANERadiator<F1F209eInclusiveDiffXSec>();
   //F1F209eInclusiveDiffXSec * xsec = new F1F209eInclusiveDiffXSec();

   InSANERadiator<InSANEInclusiveBornDISXSec> * xsec = new InSANERadiator<InSANEInclusiveBornDISXSec>();
   xsec->SetRadiationLength(mat->GetNumberOfRadiationLengths());
   //xsec->SetInternalOnly(true);// external is taken care of by GEANT4

   //InSANEInclusiveDISXSec * xsec = new InSANEInclusiveDISXSec();
   //xsec->SetTargetThickness(mat->GetNumberOfRadiationLengths());
   //xsec->Dump();
   //std::cout << "X/X0 = " << mat->GetNumberOfRadiationLengths() << std::endl;

   xsec->SetTargetMaterial(*mat);
   xsec->SetTargetMaterialIndex(i);
   xsec->SetBeamEnergy(GetBeamEnergy());
   xsec->SetTargetNucleus(*targ);
   xsec->InitializePhaseSpaceVariables();
   xsec->InitializeFinalStateParticles();
   xsec->GetPhaseSpace()->GetVariableWithName("energy")->SetMinimum(0.5);
   xsec->GetPhaseSpace()->GetVariableWithName("energy")->SetMaximum(4.5);
   xsec->GetPhaseSpace()->GetVariableWithName("theta")->SetMinimum(25.0*degree);
   samp = new InSANEPhaseSpaceSampler(xsec);
   samp->SetFoamCells(10);
   samp->SetFoamSample(10);
   //samp->SetFoamCells(100);
   samp->SetWeight(weight);
   AddSampler(samp);


   InSANERadiator<QuasiElasticInclusiveDiffXSec> * QE_xsec = new InSANERadiator<QuasiElasticInclusiveDiffXSec>();
   QE_xsec->SetTargetMaterial(*mat);
   QE_xsec->SetTargetMaterialIndex(i);
   QE_xsec->SetBeamEnergy(GetBeamEnergy());
   QE_xsec->SetTargetNucleus(*targ);
   QE_xsec->InitializePhaseSpaceVariables();
   QE_xsec->InitializeFinalStateParticles();
   QE_xsec->GetPhaseSpace()->GetVariableWithName("energy")->SetMinimum(0.5);
   QE_xsec->GetPhaseSpace()->GetVariableWithName("energy")->SetMaximum(4.5);
   QE_xsec->GetPhaseSpace()->GetVariableWithName("theta")->SetMinimum(25.0*degree);
   samp = new InSANEPhaseSpaceSampler(QE_xsec);
   samp->SetFoamCells(10);
   samp->SetFoamSample(10);
   //samp->SetFoamCells(100);
   samp->SetWeight(weight);
   AddSampler(samp);


   if( i==0 ) {
      // Add elastic radiative tail for proton 
      InSANEElasticRadiativeTail * xsec_tail = new  InSANEElasticRadiativeTail();
      xsec_tail->SetPolarizations(0.0,0.0);
      xsec_tail->SetTargetMaterial(*mat);
      xsec_tail->SetTargetMaterialIndex(i);
      xsec_tail->SetBeamEnergy(GetBeamEnergy());
      //xsec_tail->SetTargetNucleus(InSANENucleus::Proton());
      xsec_tail->SetTargetNucleus(*targ);
      xsec_tail->InitializePhaseSpaceVariables();
      xsec_tail->InitializeFinalStateParticles();
      xsec_tail->GetPhaseSpace()->GetVariableWithName("energy")->SetMinimum(0.5);
      xsec_tail->GetPhaseSpace()->GetVariableWithName("energy")->SetMaximum(4.5);
      xsec_tail->GetPhaseSpace()->GetVariableWithName("theta")->SetMinimum(25.0*degree);
      samp = new InSANEPhaseSpaceSampler(xsec_tail);
   samp->SetFoamCells(10);
   samp->SetFoamSample(10);
      //samp->SetFoamCells(100);
      samp->SetWeight(weight);
      AddSampler(samp);
   }

   PhotoOARPionDiffXSec * xsec1 = new PhotoOARPionDiffXSec();
   //xsec->Dump();
   xsec1->SetTargetMaterial(*mat);
   xsec1->SetTargetMaterialIndex(i);
   xsec1->SetBeamEnergy(GetBeamEnergy());
   xsec1->SetTargetNucleus(*targ);
   xsec1->InitializePhaseSpaceVariables();
   xsec1->InitializeFinalStateParticles();
   samp = new InSANEPhaseSpaceSampler(xsec1);
   samp->SetFoamCells(10);
   samp->SetFoamSample(10);
   //samp->SetFoamCells(100);
   samp->SetWeight(weight);
   AddSampler(samp);

   ElectroOARPionDiffXSec *xsec2 = new ElectroOARPionDiffXSec();
   //OARPionElectroDiffXSec *xsec2 = new OARPionElectroDiffXSec();
   //xsec->Dump();
   xsec2->SetTargetMaterial(*mat);
   xsec2->SetTargetMaterialIndex(i);
   xsec2->SetBeamEnergy(GetBeamEnergy());
   xsec2->SetTargetNucleus(*targ);
   xsec2->InitializePhaseSpaceVariables();
   xsec2->InitializeFinalStateParticles();
   samp = new InSANEPhaseSpaceSampler(xsec2);
   samp->SetFoamCells(10);
   samp->SetFoamSample(10);
   //samp->SetFoamCells(100);
   samp->SetWeight(weight);
   AddSampler(samp);


}
//______________________________________________________________________________
void InclusiveElectronPionGenerator::Initialize() {
   InSANETargetEventGenerator::Initialize();
}
//______________________________________________________________________________

