#ifndef SANEEventGenerators_HH
#define SANEEventGenerators_HH 1

#include "Randomize.hh"
#include "BETAG4EventGenerator.hh"
#include "G4ThreeVector.hh"
#include "TMath.h"
#include "InSANEInclusiveDiffXSec.h"
#include "F1F209eInclusiveDiffXSec.h"
#include "OARPionDiffXSec.h"
#include "InSANEXSections.h"
#include "InSANEPhaseSpace.h"
#include "WiserXSection.h"
#include "HallCBeam.h"
#include "PolarizedDISXSec.h"
#include "EPCVXSection.h"
#include "SANETargets.h"
#include "InSANERadiator.h"
#include "InSANEFunctionManager.h"
#include "InSANEElasticRadiativeTail.h"

/*! \page EventGeneratorHowTo How to make an event generator

  Implement the virtual method BETAG4EventGenerator::Initialize().
  The implementation should do the following:
    - Instantiate a concrete class deriving from InSANEDiffXSec.
    - Create the phase space and phase space variables (often easiest to use InSANEDiffXSec::InitializePhaseSpaceVariables() )
    - Set the beam energy.
    - Set the particle type (often easiest to use InSANEDiffXSec::InitializeFinalStateParticles() )
    - Add

  See class documentation for BETAG4EventGenerator and InSANEEventGenerator.

 */


/**  Flat Event generator centered on bigcal with small solid angle
 *   and near mono chromatic energies, hence a cone
 *
 *   \ingroup EventGen
 */
class ConeEventGenerator : public BETAG4EventGenerator   {
   public:
      ConeEventGenerator();
      virtual ~ConeEventGenerator();
      virtual  G4ThreeVector&  GetInitialPosition(const TParticle * ){
         fInitialPosition->setX(2.*(G4UniformRand()-0.5)*1.0*cm);
         fInitialPosition->setY(2.*(G4UniformRand()-0.5)*1.0*cm);
         fInitialPosition->setZ(2.*(G4UniformRand()-0.5)*1.0*cm  - fUpstreamPosition*cm);
         return(*fInitialPosition);
      }
      double fUpstreamPosition;
};


//______________________________________________________________________________
/** Event generator for inclusive electron DIS.   
 *   \ingroup EventGen
 */
class DISEventGenerator : public BETAG4EventGenerator {
   public:
      DISEventGenerator(){ }
      ~DISEventGenerator(){}
      virtual void Initialize();
};


//______________________________________________________________________________
/** Polarized DIS cross section (born).
 *   \ingroup EventGen
 */
class PolarizedDISEventGenerator : public BETAG4EventGenerator {
   public:
      PolarizedDISEventGenerator(){
      }
      virtual ~PolarizedDISEventGenerator(){}
      virtual void Initialize();

};


//______________________________________________________________________________
/** Throws pi0, pi+ and pi- pions.
 *   \ingroup EventGen
 */
class InclusivePionEventGenerator : public BETAG4EventGenerator  {
   public :
      InclusivePionEventGenerator(){ }
      virtual ~InclusivePionEventGenerator() { }
      virtual void Initialize();
};


//______________________________________________________________________________
/** Electrons and pi0s thrown.
 *   \ingroup EventGen
 */
class InclusiveElectronPionGenerator : public BETAG4EventGenerator  {
   public :
      InclusiveElectronPionGenerator(){ 
         fNH3PackingFraction = 0.6;
         UVAPolarizedAmmoniaTarget * targ = new UVAPolarizedAmmoniaTarget("UVaTarget","UVa Ammonia target",fNH3PackingFraction);
         SetTarget(targ);
         //InSANEFunctionManager::GetManager()->CreateSFs(1); // 1=CTEQ
         InSANEFunctionManager::GetManager()->CreateSFs(11); // 11=composite
         //InSANEFunctionManager::GetManager()->CreateSFs(2); // 2=NMC95
      }

      virtual ~InclusiveElectronPionGenerator() { }
      virtual void InitializeMaterialXSec(const Int_t i, const Double_t weight, const InSANETargetMaterial * mat, const InSANENucleus * targ){
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
         samp->SetFoamCells(200);
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
            samp->SetFoamCells(200);
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
         samp->SetFoamCells(200);
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
         samp->SetFoamCells(200);
         samp->SetWeight(weight);
         AddSampler(samp);


      }
      virtual void Initialize() {
         InSANETargetEventGenerator::Initialize();
      }

      Double_t fNH3PackingFraction;
};


//______________________________________________________________________________
/**  Mott Cross section event generator
 *
 *   \ingroup EventGen
 */
class MottEventGenerator : public BETAG4EventGenerator   {
   public:
      MottEventGenerator(){}
      virtual ~MottEventGenerator(){}
      virtual void Initialize();
};


//______________________________________________________________________________
/**  Wiser  Event generator spread over bigcal
 *
 *   \ingroup EventGen
 */
class WiserEventGenerator : public BETAG4EventGenerator   {
   public:
      WiserEventGenerator(){ }
      virtual ~WiserEventGenerator() { }
      virtual void Initialize();
};


//______________________________________________________________________________
/**  Bigcal Simple Event generator spread over bigcal
 *
 *   \ingroup EventGen
 */
class BigcalSimpleEventGenerator : public BETAG4EventGenerator   {
   public:
      BigcalSimpleEventGenerator(){ }
      virtual ~BigcalSimpleEventGenerator() { }
};


//______________________________________________________________________________
/**  Flat Event generator centered on bigcal with small solid angle
 *   and near mono chromatic energies
 *
 *   \ingroup EventGen
 */
class BigcalCenterEventGenerator : public BETAG4EventGenerator   {
   public:
      BigcalCenterEventGenerator(){ }
      virtual ~BigcalCenterEventGenerator(){ }
      virtual void Initialize();
};

/** Beam on target event generator that simply puts an electron beam upstream towards the target.
 *
 *  The starting position is a few cm upstream of the target
 *   \ingroup EventGen
 */
class BeamOnTargetEventGenerator : public BETAG4EventGenerator   {
   public:
      BeamOnTargetEventGenerator(){
         fUpstreamPosition = 150.0;//cm
         fYPosition = -1.0*(100.0-99.944)*100.0/*-0.43*/;//cm
         fBeamEnergy= 5.9;//GeV
         fChicaneBendAngle = 2.214*TMath::Pi()/180.0;
      }
      virtual ~BeamOnTargetEventGenerator() { }

      double fYPosition;
      double fChicaneBendAngle;
      double fUpstreamPosition;
      double fBeamEnergy;

      virtual  G4ThreeVector &  GetInitialPosition(const TParticle * ){
         fInitialPosition->setX(2.*(G4UniformRand()-0.5)*1.0*cm);
         fInitialPosition->setY(2.*(G4UniformRand()-0.5)*1.0*cm + fYPosition*cm);
         fInitialPosition->setZ(2.*(G4UniformRand()-0.5)*0.0*cm  - fUpstreamPosition*cm);
         return(*fInitialPosition);
      }

      virtual  G4ThreeVector &  GetInitialDirection(const TParticle * ){
         fInitialDirection->setX(0.0);
         fInitialDirection->setY(1.0*TMath::Sin(fChicaneBendAngle));
         fInitialDirection->setZ(1.0*TMath::Cos(fChicaneBendAngle));
         return(*fInitialDirection);
      }
      virtual  G4ThreeVector &  GetMomentumVector(const TParticle * ){
         fMomentumVector->setX(fInitialDirection->x() *fEventArray[0]*1000.0);
         fMomentumVector->setY(fInitialDirection->y()*fEventArray[0]*1000.0);
         fMomentumVector->setZ(fInitialDirection->z()*fEventArray[0]*1000.0);
         return(*fMomentumVector);
      }
      virtual double GetParticleEnergy(const TParticle *) { return(fBeamEnergy); }
};

/** Uses QFS
 * 
 *   \ingroup EventGen
 */
class SANEInclusiveDISEventGenerator : public BETAG4EventGenerator  {
   public :
      SANEInclusiveDISEventGenerator(){
         fNH3PackingFraction = 0.6;
         UVAPolarizedAmmoniaTarget * targ = new UVAPolarizedAmmoniaTarget("UVaTarget","UVa Ammonia target",fNH3PackingFraction);
         SetTarget(targ);
      }
      virtual ~SANEInclusiveDISEventGenerator() { }


      virtual void InitializeMaterialXSec(const Int_t i, const Double_t weight, const InSANETargetMaterial * mat, const InSANENucleus * targ){
         InSANEPhaseSpaceSampler * samp = 0;
         //F1F209eInclusiveDiffXSec * xsec = new F1F209eInclusiveDiffXSec();
         InSANEInclusiveDISXSec * xsec = new InSANEInclusiveDISXSec();
         //xsec->Dump();
         xsec->SetTargetMaterial(*mat);
         xsec->SetTargetMaterialIndex(i);
         xsec->SetBeamEnergy(GetBeamEnergy());
         xsec->SetTargetNucleus(*targ);
         xsec->InitializePhaseSpaceVariables();
         xsec->InitializeFinalStateParticles();
         samp = new InSANEPhaseSpaceSampler(xsec);
         samp->SetFoamCells(100);
         samp->SetWeight(weight);
         AddSampler(samp);
      }
      virtual void Initialize() {
         InSANETargetEventGenerator::Initialize();
      }

      Double_t fNH3PackingFraction;

};

/** Uses A1,A2 models
 * 
 *   \ingroup EventGen
 */
class PolarizedInclusiveDISEventGenerator : public BETAG4EventGenerator  {
   public :
      PolarizedInclusiveDISEventGenerator(){}
      virtual ~PolarizedInclusiveDISEventGenerator() { }
};

/** NH3 Event Generator for getting the dilution factor
 *
 *  Emulates the UVA Polarized NH3 target 
 *
 *  Note the various paramteres for configuration
 *
 *   \ingroup EventGen
 */
class NH3TargetEventGenerator : public BETAG4EventGenerator  {
   public :
      NH3TargetEventGenerator(){
         fNH3PackingFraction = 0.6;
         UVAPolarizedAmmoniaTarget * targ = new UVAPolarizedAmmoniaTarget("UVaTarget","UVa Ammonia target",fNH3PackingFraction);
         SetTarget(targ);
      }

      virtual ~NH3TargetEventGenerator() { }

      Double_t fNH3PackingFraction;

      virtual void Initialize() {
         InSANETargetEventGenerator::Initialize();
      }

};



#endif

