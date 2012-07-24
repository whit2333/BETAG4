#ifndef BETAG4EventGenerator_HH
#define BETAG4EventGenerator_HH 1
#include "Randomize.hh"
#include "InSANEPhaseSpaceSampler.h"
#include "InSANEPhaseSpace.h"
#include "InSANEInclusiveDiffXSec.h"
#include "G4ThreeVector.hh"
#include "TMath.h"
#include "InSANEEventGenerator.h"
#include "F1F209eInclusiveDiffXSec.h"
#include "TParticle.h"

/** Base class for InSANE -> BETAG4
 *  
 *  By default it assumes there is only one particle and thus 3 random variables
 *
 *   \ingroup EventGen
 */ 
class BETAG4EventGenerator : public InSANEEventGenerator {
public:
   BETAG4EventGenerator();
   virtual ~BETAG4EventGenerator() { }

   /** Generates the randomly distributed initial position(s)
    *  \todo modify to be able to generate more than a single primary particle
    */ 
   virtual void ShootPositions(){ }

   /** Sets the cross section and creates the phase space sampler
    *
    *  Must be called before using event generator
    *  and should first call InitializePhaseSpace() or take care of
    *  phase space definition 
    *
    *  This is called from the constructor and each implementation
    *  requiring a different cross section should reimplement this method
    */
   virtual void Initialize(){
     F1F209eInclusiveDiffXSec * fDiffXSec = new  F1F209eInclusiveDiffXSec();
     fDiffXSec->SetBeamEnergy(fBeamEnergy);
     fPrimaryPS = fDiffXSec->GetPhaseSpace(); /// all the following cross sections share the same phase space. 
     fPrimaryPS->ListVariables();
     InSANEPhaseSpaceSampler *  fF1F2EventSampler = new InSANEPhaseSpaceSampler(fDiffXSec);
     AddSampler(fF1F2EventSampler);
     CalculateTotalCrossSection();
   }

protected :
   InSANEPhaseSpace * fPrimaryPS;

public :
   int fNumberOfGeneratedParticles;
   bool fIsInitialized;
   bool fNeedsRefreshed;


   G4ThreeVector * fInitialPosition ;
   G4ThreeVector * fInitialDirection ;
   G4ThreeVector * fMomentumVector ;

   InSANEPhaseSpaceVariable * varTheta;
   InSANEPhaseSpaceVariable * varPhi;
   InSANEPhaseSpaceVariable * varEnergy;
   InSANEPhaseSpaceVariable * var;
   InSANEPhaseSpaceVariable * varMomentum;

   double fBeamEnergy;
   double fEnergyMax;
   double fEnergyMin;
   double fMomentumMax;
   double fMomentumMin;
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

   double * fEventArray;


public:

/** Returns the starting postiong of the primary particle
 */
   virtual  G4ThreeVector &  GetInitialPosition(TParticle * p){
      fInitialPosition->setX(p->Vx());
      fInitialPosition->setY(p->Vy());
      fInitialPosition->setZ(p->Vz());
      return(*fInitialPosition);
   }

/** Returns the starting direction of the primary particle
 * \deprecated
 */
   virtual  G4ThreeVector &  GetInitialDirection(TParticle * p){
      fInitialDirection->setX(p->Px());
      fInitialDirection->setY(p->Py());
      fInitialDirection->setZ(p->Pz());
      return(*fInitialDirection);
   }

/// \todo assuming particle is electron (eg E=P))

   /** Returns the starting postiong of the primary particle
    * \deprecated
    */
   virtual  G4ThreeVector &  GetMomentumVector(TParticle * p){
      fInitialDirection->setX(p->Px());
      fInitialDirection->setY(p->Py());
      fInitialDirection->setZ(p->Pz());
      return(*fMomentumVector);
   }

   virtual double GetParticleEnergy(TParticle * p) {
      return(p->Pt());
   }

   void SetThetaMax(double val) { 
     fThetaMax = val;
     fNeedsRefreshed = true;
     if(varTheta) varTheta->SetVariableMaxima(val);
     else if (fPrimaryPS) {
        varTheta = fPrimaryPS->GetVariable("theta");
        if(varTheta) varTheta->SetVariableMaxima(val);
        else printf("No theta variable found !!!!!!!!!!!!!!!!!!!\n");
     }
   }
   void SetThetaMin(double val) { 
     fThetaMin = val;
     fNeedsRefreshed = true;
     if(varTheta) varTheta->SetVariableMinima(val);
     else if (fPrimaryPS) {
        varTheta = fPrimaryPS->GetVariable("theta");
        if(varTheta) varTheta->SetVariableMinima(val);
     }
   }
   void SetPhiMax(double val) { 
     fPhiMax = val;
     fNeedsRefreshed = true;
     if(varPhi) varPhi->SetVariableMaxima(val);
     else if (fPrimaryPS) {
        varPhi = fPrimaryPS->GetVariable("phi");
        if(varPhi) varPhi->SetVariableMaxima(val);
     }
   }
   void SetPhiMin(double val) { 
     fPhiMin = val;
     fNeedsRefreshed = true;
     if(varPhi) varPhi->SetVariableMinima(val);
     else if (fPrimaryPS) {
        varPhi = fPrimaryPS->GetVariable("phi");
        if(varPhi) varPhi->SetVariableMinima(val);
     }
   }

   void SetEnergyMax(double val) { 
     fEnergyMax = val;
     fNeedsRefreshed = true;
     if(varEnergy) varEnergy->SetVariableMaxima(val);
     else if (fPrimaryPS) {
        varEnergy = fPrimaryPS->GetVariable("energy");
        if(varEnergy) varEnergy->SetVariableMaxima(val);
     }
   }
   void SetEnergyMin(double val) { 
     fEnergyMin = val;
     fNeedsRefreshed = true;
     if(varEnergy) varEnergy->SetVariableMinima(val);
     else if (fPrimaryPS) {
        varEnergy = fPrimaryPS->GetVariable("energy");
        if(varEnergy) varEnergy->SetVariableMinima(val);
     }
   }
   void SetMomentumMax(double val) { 
     fMomentumMax = val;
     fNeedsRefreshed = true;
     if(varMomentum) varMomentum->SetVariableMaxima(val);
     else if (fPrimaryPS) {
        varMomentum = fPrimaryPS->GetVariable("momentum");
        if(varMomentum) varMomentum->SetVariableMaxima(val);
     }
   }
   void SetMomentumMin(double val) { 
     fMomentumMin = val;
     fNeedsRefreshed = true;
     if(varMomentum) varMomentum->SetVariableMinima(val);
     else if (fPrimaryPS) {
        varMomentum = fPrimaryPS->GetVariable("momentum");
        if(varMomentum) varMomentum->SetVariableMinima(val);
     }
   }

};


#endif
