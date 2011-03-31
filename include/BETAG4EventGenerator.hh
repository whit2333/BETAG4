#ifndef BETAG4EventGenerator_HH
#define BETAG4EventGenerator_HH 1
#include "Randomize.hh"
#include "InSANEPhaseSpaceSampler.h"
#include "InSANEInclusivePhaseSpace.h"
#include "InSANEInclusiveDiffXSec.h"
#include "G4ThreeVector.hh"
#include "TMath.h"

/**
 *   \ingroup EventGen
 */

/** Base class for InSANE -> BETAG4
 *  
 *  By default it assumes there is only one particle and thus 3 random variables
 */ 
class BETAG4EventGenerator   {
public:
   BETAG4EventGenerator();

   virtual ~BETAG4EventGenerator() {

   }

   double * GenerateEvent() {
      fEventVector = fEventSampler->GenerateEvent();
      return(fEventVector);
   }

/** Generates the randomly distributed initial position(s)
 *  \todo modify to be able to generate more than a single primary particle
 */ 
   virtual void ShootPositions(){ }

/**  Sets the phase space used and initializes
 *   each phase space variable
 */
   virtual void InitializePhaseSpace() {
      fEnergyMin = 0.80;
      fEnergyMax = 4.7;
      fThetaMin  = 30.0;
      fThetaMax  = 50.0;
      fPhiMin    =-90.0;
      fPhiMax    = 90.0;

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

/** Sets the cross section and creates the phase space sampler
 *
 *  Must be called before using event generator
 *  and should first call InitializePhaseSpace() or take care of
 *  phase space definition
 *
 *  This is called from the constructor and each implementation
 *  requiring a different cross section should reimplement this method
 */
   virtual void  Initialize() {
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

/** Re-initializes the event generator after modification
 */
   virtual void Refresh() {
      if(fDiffXSec) fDiffXSec->Refresh();
      if(fEventSampler)fEventSampler->Refresh(fDiffXSec);
   } 

   int fNumberOfGeneratedParticles;
   G4ThreeVector * fInitialPosition ;
   G4ThreeVector * fInitialDirection ;
   G4ThreeVector * fMomentumVector ;

   InSANEPhaseSpaceVariable * varTheta;
   InSANEPhaseSpaceVariable * varPhi;
   InSANEPhaseSpaceVariable * varEnergy;
   InSANEPhaseSpaceVariable * var;


   double fEnergyMax;
   double fEnergyMin;
   double fDeltaEnergy;
   double fCentralEnergy;

   double fThetaMax;
   double fThetaMin;
   double fDeltaTheta;
   double fCentralTheta;

   double fPhiMax;
   double fPhiMin;
   double fDeltaPhi;
   double fCentralPhi;

   double * fEventVector;

   InSANEInclusivePhaseSpace * fPhaseSpace;
   InSANEPhaseSpaceSampler * fEventSampler;
   InSANEInclusiveDiffXSec * fDiffXSec;

public:
   void Print() {
      std::cout << " ==== BETAG4EventGenerator ==== \n";
      if(fPhaseSpace)fPhaseSpace->Print();
      if(fDiffXSec)fDiffXSec->Print();
      if(fEventSampler)fEventSampler->Print();
      std::cout << " BETAG4EventGenerator member variables: ";
      std::cout << "   Energy: " << fEnergyMin << " - " << fEnergyMax << "\n";
      std::cout << "    Theta: " << fThetaMin << " - " << fThetaMax<< "\n";
      std::cout << "      Phi: " << fPhiMin << " - " << fPhiMax<< "\n";
   }

/** Returns the starting postiong of the primary particle
 */
   virtual  G4ThreeVector &  GetInitialPosition(){
      fInitialPosition->setX(0.0);
      fInitialPosition->setY(0.0);
      fInitialPosition->setZ(0.0);
      return(*fInitialPosition);
   }

/** Returns the starting direction of the primary particle
 */
   virtual  G4ThreeVector &  GetInitialDirection(){
      fInitialDirection->setX(TMath::Sin ( fEventVector[1]) *TMath::Cos ( fEventVector[2] ));
      fInitialDirection->setY(TMath::Sin ( fEventVector[1] ) *TMath::Sin ( fEventVector[2]));
      fInitialDirection->setZ(TMath::Cos (  fEventVector[1] ));
      return(*fInitialDirection);
   }

/// \todo assuming particle is electron (eg E=P))

/** Returns the starting postiong of the primary particle
 */
   virtual  G4ThreeVector &  GetMomentumVector(){
      fMomentumVector->setX(fInitialDirection->x() *fEventVector[0]*1000.0);
      fMomentumVector->setY(fInitialDirection->y()*fEventVector[0]*1000.0);
      fMomentumVector->setZ(fInitialDirection->z()*fEventVector[0]*1000.0);
      return(*fMomentumVector);
   }

   virtual double GetParticleEnergy() {
      return(fEventVector[0]);
   }

   void SetThetaMax(double val) { 
     fThetaMax = val;
     varTheta->SetVariableMaxima(val);
   }
   void SetThetaMin(double val) { 
     fThetaMin = val;
     varTheta->SetVariableMinima(val);
   }
   void SetPhiMax(double val) { 
     fPhiMax = val;
     varPhi->SetVariableMaxima(val);
   }
   void SetPhiMin(double val) { 
     fPhiMin = val;
     varPhi->SetVariableMinima(val);
   }
   void SetEnergyMax(double val) { 
     fEnergyMax = val;
     varEnergy->SetVariableMaxima(val);
   }
   void SetEnergyMin(double val) { 
     fEnergyMin = val;
     varEnergy->SetVariableMinima(val);
   }
};


#endif