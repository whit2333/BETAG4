#ifndef SANEEventGenerators_HH
#define SANEEventGenerators_HH 1

#include "Randomize.hh"
#include "BETAG4EventGenerator.hh"
#include "G4ThreeVector.hh"
#include "TMath.h"
#include "F1F209eInclusiveDiffXSec.h"

/**
 *   \ingroup EventGen
 */
/**  Flat Event generator centered on bigcal with small solid angle
 *   and near mono chromatic energies, hence a cone
 *
 */
class ConeEventGenerator : public BETAG4EventGenerator   {
public:
   ConeEventGenerator() {
      fCentralTheta = 40.0;
      fCentralPhi = 0.0;
      fCentralEnergy = 0.50;
      fDeltaTheta = 40.0;
      fDeltaPhi = 180.0;
      fDeltaEnergy = 0.400; //GeV
      fUpstreamPosition = 50.0;//cm
   }

   virtual ~ConeEventGenerator() {

   }
   double fUpstreamPosition;

   virtual  G4ThreeVector &  GetInitialPosition(){
      fInitialPosition->setX(2.*(G4UniformRand()-0.5)*1.0*cm);
      fInitialPosition->setY(2.*(G4UniformRand()-0.5)*1.0*cm);
      fInitialPosition->setZ(2.*(G4UniformRand()-0.5)*1.0*cm  - fUpstreamPosition*cm);
      return(*fInitialPosition);
   }
   virtual void InitializePhaseSpace() {
      fEnergyMin = fCentralEnergy-fDeltaEnergy;
      fEnergyMax = fCentralEnergy+fDeltaEnergy;

      fThetaMin  = fCentralTheta-fDeltaTheta;
      fThetaMax  = fCentralTheta+fDeltaTheta;

      fPhiMin    = fCentralPhi-fDeltaPhi;
      fPhiMax    = fCentralPhi+fDeltaPhi;


      fPhaseSpace = new InSANEInclusivePhaseSpace();
      // Add random variables ( 3 needed) to phase space
      varEnergy = new InSANEPhaseSpaceVariable();
      varEnergy->fVariableName="Energy"; 
      varEnergy->fVariable="E#prime"; // ROOT string latex
      varEnergy->SetVariableMinima(fEnergyMin); //GeV
      varEnergy->SetVariableMaxima(fEnergyMax); //GeV
      fPhaseSpace->AddVariable(varEnergy);

      varTheta = new InSANEPhaseSpaceVariable();
      varTheta->fVariableName="theta"; 
      varTheta->fVariable="#theta"; // ROOT string latex
      varTheta->SetVariableMinima(fThetaMin*TMath::Pi()/180.0); //
      varTheta->SetVariableMaxima(fThetaMax*TMath::Pi()/180.0); //
      fPhaseSpace->AddVariable(varTheta);

      varPhi = new InSANEPhaseSpaceVariable();
      varPhi->fVariableName="phi"; 
      varPhi->fVariable="#phi"; // ROOT string latex
      varPhi->SetVariableMinima(fPhiMin*TMath::Pi()/180.0); //
      varPhi->SetVariableMaxima(fPhiMax*TMath::Pi()/180.0); //
      fPhaseSpace->AddVariable(varPhi);
   }
};

/** Event generator for inclusive electron DIS
 *
 */
class DISEventGenerator : public BETAG4EventGenerator {
public:
   virtual void Initialize() {
      // InitializePhaseSpace() must be called
      InitializePhaseSpace();

      // Create the differential cross section to be used
      fDiffXSec = new F1F209eInclusiveDiffXSec();
      // Set the cross section's phase space
      fDiffXSec->SetPhaseSpace(fPhaseSpace);
      // Create event sampler
      fEventSampler = new InSANEPhaseSpaceSampler(fDiffXSec);
      // Set the seed 
      fEventSampler->fRandomNumberGenerator->SetSeed((int)(G4UniformRand()*999999));
   }
};


/**  Mott Cross section event generator
 *
 */
class MottEventGenerator : public BETAG4EventGenerator   {
public:

   virtual void Initialize() {
      InitializePhaseSpace();

      // Create the differential cross section to be used
      fDiffXSec = new InSANEInclusiveMottXSec();
      // Set the cross section's phase space
      fDiffXSec->SetPhaseSpace(fPhaseSpace);
      // Create event sampler
      fEventSampler = new InSANEPhaseSpaceSampler(fDiffXSec);
      // Set the seed 
      fEventSampler->fRandomNumberGenerator->SetSeed((int)(G4UniformRand()*999999));
   }
};

/**  Flat Event generator spread over bigcal
 *
 */
class BigcalSimpleEventGenerator : public BETAG4EventGenerator   {
public:
   BigcalSimpleEventGenerator(){
     
   };
   virtual ~BigcalSimpleEventGenerator() {
   }
   virtual void Initialize() {
      InitializePhaseSpace();

      // Create the differential cross section to be used
      fDiffXSec = new InSANEFlatInclusiveDiffXSec();
      // Set the cross section's phase space
      fDiffXSec->SetPhaseSpace(fPhaseSpace);
      // Create event sampler
      fEventSampler = new InSANEPhaseSpaceSampler(fDiffXSec);
      // Set the seed 
      fEventSampler->fRandomNumberGenerator->SetSeed((int)(G4UniformRand()*999999));
   }
};



/**  Flat Event generator centered on bigcal with small solid angle
 *   and near mono chromatic energies
 *
 */
class BigcalCenterEventGenerator : public BETAG4EventGenerator   {
public:
   BigcalCenterEventGenerator(){ };

   virtual ~BigcalCenterEventGenerator() {
   }


};

/** Beam on target event generator that simply puts an electron beam upstream towards the target.
 *
 *  The starting position is a few cm upstream of the target
 */
class BeamOnTargetEventGenerator : public BETAG4EventGenerator   {
public:
   BeamOnTargetEventGenerator(){
      fUpstreamPosition = 0.0;//cm
      fBeamEnergy=4.7;//GeV
   }
   virtual ~BeamOnTargetEventGenerator() {
   }

   double fUpstreamPosition;
   double fBeamEnergy;

   virtual  G4ThreeVector &  GetInitialPosition(){
      fInitialPosition->setX(2.*(G4UniformRand()-0.5)*1.0*cm);
      fInitialPosition->setY(2.*(G4UniformRand()-0.5)*1.0*cm);
      fInitialPosition->setZ(2.*(G4UniformRand()-0.5)*1.0*cm  - fUpstreamPosition*cm);
      return(*fInitialPosition);
   }

   virtual  G4ThreeVector &  GetInitialDirection(){
      fInitialDirection->setX(0.0);
      fInitialDirection->setY(0.0);
      fInitialDirection->setZ(1.0);
      return(*fInitialDirection);
   }
   virtual  G4ThreeVector &  GetMomentumVector(){
      fMomentumVector->setX(fInitialDirection->x() *fEventVector[0]*1000.0);
      fMomentumVector->setY(fInitialDirection->y()*fEventVector[0]*1000.0);
      fMomentumVector->setZ(fInitialDirection->z()*fEventVector[0]*1000.0);
      return(*fMomentumVector);
   }
   virtual double GetParticleEnergy() {
      return(fBeamEnergy);
   }
};

/**
 */
class SANEInclusiveDISEventGenerator : public BETAG4EventGenerator  {
public:
   SANEInclusiveDISEventGenerator(){}

   virtual ~SANEInclusiveDISEventGenerator() {
   }

};



#endif

