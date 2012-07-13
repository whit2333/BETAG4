#ifndef SANEEventGenerators_HH
#define SANEEventGenerators_HH 1

#include "Randomize.hh"
#include "BETAG4EventGenerator.hh"
#include "G4ThreeVector.hh"
#include "TMath.h"
#include "F1F209eInclusiveDiffXSec.h"
#include "InSANEInclusiveDiffXSec.h"
#include "InSANEXSections.h"
#include "WiserXSection.h"

/**  Flat Event generator centered on bigcal with small solid angle
 *   and near mono chromatic energies, hence a cone
 *
 *   \ingroup EventGen
 */
class ConeEventGenerator : public BETAG4EventGenerator   {
public:
   ConeEventGenerator();
   virtual ~ConeEventGenerator();

   virtual  G4ThreeVector &  GetInitialPosition(){
      fInitialPosition->setX(2.*(G4UniformRand()-0.5)*1.0*cm);
      fInitialPosition->setY(2.*(G4UniformRand()-0.5)*1.0*cm);
      fInitialPosition->setZ(2.*(G4UniformRand()-0.5)*1.0*cm  - fUpstreamPosition*cm);
      return(*fInitialPosition);
   }

   double fUpstreamPosition;
};

/** Event generator for inclusive electron DIS.  Uses F1F2 Code. 
 *
 *   \ingroup EventGen
 */
class DISEventGenerator : public BETAG4EventGenerator {
public:
   DISEventGenerator(){}
   ~DISEventGenerator(){}

   /** */
   virtual void Initialize(){
     fBeamEnergy=5.9;
     F1F209eInclusiveDiffXSec * fDiffXSec = new  F1F209eInclusiveDiffXSec();
     fDiffXSec->SetBeamEnergy(fBeamEnergy);
     fDiffXSec->InitializePhaseSpaceVariables();
     fPrimaryPS = fDiffXSec->GetPhaseSpace(); 
     fPrimaryPS->ListVariables();
     InSANEPhaseSpaceSampler *  fF1F2EventSampler = new InSANEPhaseSpaceSampler(fDiffXSec);
     AddSampler(fF1F2EventSampler);
     CalculateTotalCrossSection();

   }

};


/** Electrons and pi0s thrown 
 *   \ingroup EventGen
 */
class InclusiveElectronPionGenerator : public BETAG4EventGenerator  {
public :
   InclusiveElectronPionGenerator(){
   }

   virtual ~InclusiveElectronPionGenerator() { }

   virtual void Initialize(){
      fBeamEnergy=5.9;

      /// Neutral Pion
      InSANEInclusiveWiserXSec * fDiffXSec1 = new InSANEInclusiveWiserXSec();
      fDiffXSec1->SetBeamEnergy(fBeamEnergy);
      fDiffXSec1->SetProductionParticleType(111);
      fDiffXSec1->InitPhaseSpace();
      fDiffXSec1->InitializeFinalStateParticles();
      InSANEPhaseSpaceSampler *  pi0EventSampler = new InSANEPhaseSpaceSampler(fDiffXSec1);
      AddSampler(pi0EventSampler);

      /// Positive Pion
      InSANEInclusiveWiserXSec * fDiffXSec2 = new InSANEInclusiveWiserXSec();
      fDiffXSec2->SetBeamEnergy(fBeamEnergy);
      fDiffXSec2->SetProductionParticleType(211);
      fDiffXSec2->InitPhaseSpace();
      fDiffXSec2->InitializeFinalStateParticles();
      InSANEPhaseSpaceSampler *  pi0EventSampler2 = new InSANEPhaseSpaceSampler(fDiffXSec2);
      AddSampler(pi0EventSampler2);

      /// Negative Pion
      InSANEInclusiveWiserXSec * fDiffXSec3 = new InSANEInclusiveWiserXSec();
      fDiffXSec3->SetBeamEnergy(fBeamEnergy);
      fDiffXSec3->SetProductionParticleType(-211);
      fDiffXSec3->InitPhaseSpace();
      fDiffXSec3->InitializeFinalStateParticles();
      InSANEPhaseSpaceSampler *  pi0EventSampler3 = new InSANEPhaseSpaceSampler(fDiffXSec3);
      AddSampler(pi0EventSampler3);


      F1F209eInclusiveDiffXSec * fDiffXSec = new  F1F209eInclusiveDiffXSec();
      fDiffXSec->SetBeamEnergy(fBeamEnergy);
      fDiffXSec->InitializePhaseSpaceVariables();
//      InSANEPhaseSpace *ps = fDiffXSec->GetPhaseSpace(); /// all the following cross sections share the same phase space. 
//     ps->ListVariables();
      InSANEPhaseSpaceSampler *  fF1F2EventSampler = new InSANEPhaseSpaceSampler(fDiffXSec);
      AddSampler(fF1F2EventSampler);

      CalculateTotalCrossSection();
   }

};



/**  Mott Cross section event generator
 *
 *   \ingroup EventGen
 */
class MottEventGenerator : public BETAG4EventGenerator   {
public:
   MottEventGenerator(){}
   ~MottEventGenerator(){}
   /** */
   virtual void Initialize(){
     fBeamEnergy=5.9;
     F1F209eInclusiveDiffXSec * fDiffXSec = new  F1F209eInclusiveDiffXSec();
     fDiffXSec->SetBeamEnergy(fBeamEnergy);
     fDiffXSec->InitializePhaseSpaceVariables();
     InSANEPhaseSpace *ps = fDiffXSec->GetPhaseSpace(); /// all the following cross sections share the same phase space. 
     ps->ListVariables();
     InSANEPhaseSpaceSampler *  fF1F2EventSampler = new InSANEPhaseSpaceSampler(fDiffXSec);
     AddSampler(fF1F2EventSampler);
     CalculateTotalCrossSection();
   }
};

/**  Flat Event generator spread over bigcal
 *
 *   \ingroup EventGen
 */
class WiserEventGenerator : public BETAG4EventGenerator   {
public:
   WiserEventGenerator(){ }
   virtual ~WiserEventGenerator() { }

   virtual void Initialize() {
      fBeamEnergy=5.9;
      InSANEInclusiveWiserXSec * fDiffXSec = new InSANEInclusiveWiserXSec();
      fDiffXSec->SetBeamEnergy(fBeamEnergy);
      fDiffXSec->SetProductionParticleType(111);
      fDiffXSec->InitPhaseSpace();
      fDiffXSec->InitializeFinalStateParticles();
      fPrimaryPS = fDiffXSec->GetPhaseSpace(); 
      InSANEPhaseSpaceSampler *  fEventSampler = new InSANEPhaseSpaceSampler(fDiffXSec);
      AddSampler(fEventSampler);
      CalculateTotalCrossSection();
      fPrimaryPS->Print();

   }
};


/**  Flat Event generator spread over bigcal
 *
 *   \ingroup EventGen
 */
class BigcalSimpleEventGenerator : public BETAG4EventGenerator   {
public:
   BigcalSimpleEventGenerator(){
     
   };
   virtual ~BigcalSimpleEventGenerator() {
   }
//    virtual void Initialize() {
//       InitializePhaseSpace();
// 
//       // Create the differential cross section to be used
//       fDiffXSec = new InSANEFlatInclusiveDiffXSec();
//       // Set the cross section's phase space
//       fDiffXSec->SetPhaseSpace(fPhaseSpace);
//       // Create event sampler
//       fEventSampler = new InSANEPhaseSpaceSampler(fDiffXSec);
//       // Set the seed 
//       fEventSampler->fRandomNumberGenerator->SetSeed((int)(G4UniformRand()*999999));
//    }
};



/**  Flat Event generator centered on bigcal with small solid angle
 *   and near mono chromatic energies
 *
 *   \ingroup EventGen
 */
class BigcalCenterEventGenerator : public BETAG4EventGenerator   {
public:
   BigcalCenterEventGenerator(){ };

   virtual ~BigcalCenterEventGenerator() {
   }

   virtual void Initialize() {
      fBeamEnergy=5.9;
      InSANEFlatInclusiveDiffXSec * fDiffXSec = new InSANEFlatInclusiveDiffXSec();
      fDiffXSec->SetBeamEnergy(fBeamEnergy);
/*      fDiffXSec->SetProductionParticleType(111);*/
      fDiffXSec->InitPhaseSpace();
      fDiffXSec->InitializeFinalStateParticles();
      fPrimaryPS = fDiffXSec->GetPhaseSpace(); 
      InSANEPhaseSpaceSampler *  fEventSampler = new InSANEPhaseSpaceSampler(fDiffXSec);
      AddSampler(fEventSampler);
      CalculateTotalCrossSection();
     fPrimaryPS->Print();

   }

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
   virtual ~BeamOnTargetEventGenerator() {
   }

   double fYPosition;
   double fChicaneBendAngle;
   double fUpstreamPosition;
   double fBeamEnergy;

   virtual  G4ThreeVector &  GetInitialPosition(){
      fInitialPosition->setX(2.*(G4UniformRand()-0.5)*1.0*cm);
      fInitialPosition->setY(2.*(G4UniformRand()-0.5)*1.0*cm + fYPosition*cm);
      fInitialPosition->setZ(2.*(G4UniformRand()-0.5)*0.0*cm  - fUpstreamPosition*cm);
      return(*fInitialPosition);
   }

   virtual  G4ThreeVector &  GetInitialDirection(){
      fInitialDirection->setX(0.0);
      fInitialDirection->setY(1.0*TMath::Sin(fChicaneBendAngle));
      fInitialDirection->setZ(1.0*TMath::Cos(fChicaneBendAngle));
      return(*fInitialDirection);
   }
   virtual  G4ThreeVector &  GetMomentumVector(){
      fMomentumVector->setX(fInitialDirection->x() *fEventArray[0]*1000.0);
      fMomentumVector->setY(fInitialDirection->y()*fEventArray[0]*1000.0);
      fMomentumVector->setZ(fInitialDirection->z()*fEventArray[0]*1000.0);
      return(*fMomentumVector);
   }
   virtual double GetParticleEnergy() {
      return(fBeamEnergy);
   }
};

/** Uses QFS
 * 
 *   \ingroup EventGen
 */
class SANEInclusiveDISEventGenerator : public BETAG4EventGenerator  {
public :
   SANEInclusiveDISEventGenerator(){}

   virtual ~SANEInclusiveDISEventGenerator() { }

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

/** Event generator for inclusive electron DIS
 *
 *   \ingroup EventGen
 */
class PolarizedDISEventGenerator : public BETAG4EventGenerator {
public:
   PolarizedDISEventGenerator() {
      fVarHelicity=0;
   }

   /**  Add the helicity variable to the normal 
    *   BETAG4EventGenerator::InitializePhaseSpace()
    */
//    virtual void InitializePhaseSpace() {
//       BETAG4EventGenerator::InitializePhaseSpace();
//       fVarHelicity = new InSANEDiscretePhaseSpaceVariable();
//       fVarHelicity->fVariableName="helicity"; 
//       fVarHelicity->fVariable="#lambda";
//       ((InSANEDiscretePhaseSpaceVariable*)fVarHelicity)->SetNumberOfValues(3); // ROOT string latex
// /*      varHelicity->SetVariableMaxima(1.0); //*/
//       fPhaseSpace->AddVariable(fVarHelicity);
//    }


   /** Initialize the event generator
    */
//    virtual void Initialize() {
//       // InitializePhaseSpace() must be called
//       InitializePhaseSpace();
// 
//       // Create the differential cross section to be used
//       fDiffXSec = new F1F209eInclusiveDiffXSec();
//       // Set the cross section's phase space
//       fDiffXSec->SetPhaseSpace(fPhaseSpace);
//       // Create event sampler
//       fEventSampler = new InSANEPhaseSpaceSampler(fDiffXSec);
//       // Set the seed 
//       fEventSampler->fRandomNumberGenerator->SetSeed((int)(G4UniformRand()*999999));
//    }

   InSANEPhaseSpaceVariable * fVarHelicity;

   Int_t fHelicity;

};

/** NH3 Event Generator for getting the dilution factor
 *
 *  Emulates the UVA Polarized NH3 target 
 *
 *  Note the various paramteres for configuration
 *
 *   \ingroup EventGen
 */
class NH3TargetEventGenerator : public PolarizedDISEventGenerator  {
public :
   NH3TargetEventGenerator(){
      fNH3PackingFraction = 0.5;
      fN14Density;
      fH3Density;
   }

   virtual ~NH3TargetEventGenerator() { }

   Double_t fNH3PackingFraction;
   Double_t fN14Density;
   Double_t fH3Density;

};



#endif

