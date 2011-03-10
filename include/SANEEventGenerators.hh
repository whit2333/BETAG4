#ifndef SANEEventGenerators_HH
#define SANEEventGenerators_HH 1
#include "Randomize.hh"
#include "InSANEPhaseSpaceSampler.h"
#include "InSANEInclusivePhaseSpace.h"
#include "InSANEInclusiveDiffXSec.h"

class BigcalCenterEventGenerator   {
public:
   double fThetaMax;
   double fEnergyMax;
   double fPhiMax;
   double fEnergyMin;
   double fThetaMin;
   double fPhiMin;
   InSANEInclusivePhaseSpace * fPhaseSpace;
   InSANEPhaseSpaceSampler * fEventSampler;
   InSANEInclusiveDiffXSec * fDiffXSec;

public:
/** c'tor */
   BigcalCenterEventGenerator() {
      fEnergyMin = 0.80;
      fEnergyMax = 5.7;
      fThetaMin  = 39.0;
      fThetaMax  = 41.0;
      fPhiMin    =-5.0;
      fPhiMax    = 5.0;
      fPhaseSpace = new InSANEInclusivePhaseSpace();

      // Add random variables ( 3 needed) to phase space
      InSANEPhaseSpaceVariable * var;
      var = new InSANEPhaseSpaceVariable();
      var->fVariableName="Energy"; 
      var->fVariable="E#prime"; // ROOT string latex
      var->fVariableMinima = fEnergyMin; //GeV
      var->fVariableMaxima = fEnergyMax; //GeV
      fPhaseSpace->AddVariable(var);

      var = new InSANEPhaseSpaceVariable();
      var->fVariableName="theta"; 
      var->fVariable="#theta"; // ROOT string latex
      var->fVariableMinima = fThetaMin*TMath::Pi()/180.0; //
      var->fVariableMaxima = fThetaMax*TMath::Pi()/180.0; //
      fPhaseSpace->AddVariable(var);

      var = new InSANEPhaseSpaceVariable();
      var->fVariableName="phi"; 
      var->fVariable="#phi"; // ROOT string latex
      var->fVariableMinima= fPhiMin*TMath::Pi()/180.0; //
      var->fVariableMaxima= fPhiMax*TMath::Pi()/180.0; //
      fPhaseSpace->AddVariable(var);

      // Create the differential cross section to be used
      fDiffXSec = new InSANEFlatInclusiveDiffXSec();
      // Set the cross section's phase space
      fDiffXSec->SetPhaseSpace(fPhaseSpace);
      // Create event sampler
      fEventSampler = new InSANEPhaseSpaceSampler(fDiffXSec);
      // Set the seed 
      fEventSampler->fRandomNumberGenerator->SetSeed((int)(G4UniformRand()*999999));
   }


   ~BigcalCenterEventGenerator() {
      if(fPhaseSpace) delete fPhaseSpace;

   }


   double * GenerateEvent() { return(fEventSampler->GenerateEvent());} 



};



#endif

