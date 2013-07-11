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

/** Base class for InSANE -> BETAG4.
 *  By default it assumes there is only one particle and thus 3 random variables
 *
 *   \ingroup EventGen
 */ 
class BETAG4EventGenerator : public InSANEEventGenerator {
   public :
      int   fNumberOfGeneratedParticles;
      bool  fIsInitialized;
      bool  fNeedsRefreshed;

      G4ThreeVector * fInitialPosition ;
      G4ThreeVector * fInitialDirection ;
      G4ThreeVector * fMomentumVector ;

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
      BETAG4EventGenerator();
      virtual ~BETAG4EventGenerator(); 

      /** Sets the cross section and creates the phase space sampler. 
       *  Must be called before using event generator
       *  and should first call InitializePhaseSpace() or take care of
       *  phase space definition.
       *  This is called from the constructor and each implementation
       *  requiring a different cross section should reimplement this method
       */
      virtual void Initialize();


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
         return(p->Energy());
      }

      void SetThetaMax(double val) { 
         fThetaMax = val;
         fNeedsRefreshed = true;
         TList * vars = GetPSVariables("theta");
         for(int i = 0; i< vars->GetEntries() ; i++){
            InSANEPhaseSpaceVariable * aVar = (InSANEPhaseSpaceVariable*) vars->At(i);
            aVar->SetMaximum(val);
         }
         if( vars->GetEntries() == 0) Error("SetThetaMax","\"theta\" variable does not exist.");
      }

      void SetThetaMin(double val) { 
         fThetaMin = val;
         fNeedsRefreshed = true;
         TList * vars = GetPSVariables("theta");
         for(int i = 0; i< vars->GetEntries() ; i++){
            InSANEPhaseSpaceVariable * aVar = (InSANEPhaseSpaceVariable*) vars->At(i);
            aVar->SetMinimum(val);
         }
         if( vars->GetEntries() == 0) Error("SetThetaMin","\"theta\" variable does not exist.");
      }

      void SetPhiMax(double val) { 
         fPhiMax = val;
         fNeedsRefreshed = true;
         TList * vars = GetPSVariables("phi");
         for(int i = 0; i< vars->GetEntries() ; i++){
            InSANEPhaseSpaceVariable * aVar = (InSANEPhaseSpaceVariable*) vars->At(i);
            aVar->SetMaximum(val);
         }
         if( vars->GetEntries() == 0) Error("SetPhiMax","\"phi\" variable does not exist.");
      }

      void SetPhiMin(double val) { 
         fPhiMin = val;
         fNeedsRefreshed = true;
         TList * vars = GetPSVariables("phi");
         for(int i = 0; i< vars->GetEntries() ; i++){
            InSANEPhaseSpaceVariable * aVar = (InSANEPhaseSpaceVariable*) vars->At(i);
            aVar->SetMinimum(val);
         }
         if( vars->GetEntries() == 0) Error("SetPhiMin","\"phi\" variable does not exist.");
      }

      void SetEnergyMax(double val) { 
         fEnergyMax = val;
         fNeedsRefreshed = true;
         TList * vars = GetPSVariables("energy");
         for(int i = 0; i< vars->GetEntries() ; i++){
            InSANEPhaseSpaceVariable * aVar = (InSANEPhaseSpaceVariable*) vars->At(i);
            aVar->SetMaximum(val);
         }
         if( vars->GetEntries() == 0) Error("SetEnergyMax","\"energy\" variable does not exist.");
      }

      void SetEnergyMin(double val) { 
         fEnergyMin = val;
         fNeedsRefreshed = true;
         TList * vars = GetPSVariables("energy");
         for(int i = 0; i< vars->GetEntries() ; i++){
            InSANEPhaseSpaceVariable * aVar = (InSANEPhaseSpaceVariable*) vars->At(i);
            aVar->SetMinimum(val);
         }
         if( vars->GetEntries() == 0) Error("SetEnergyMax","\"energy\" variable does not exist.");
      }

      void SetMomentumMax(double val) { 
         fMomentumMax = val;
         fNeedsRefreshed = true;
         TList * vars = GetPSVariables("momentum");
         for(int i = 0; i< vars->GetEntries() ; i++){
            InSANEPhaseSpaceVariable * aVar = (InSANEPhaseSpaceVariable*) vars->At(i);
            aVar->SetMaximum(val);
         }
         if( vars->GetEntries() == 0) Error("SetMomentumMax","\"momentum\" variable does not exist.");
      }

      void SetMomentumMin(double val) { 
         fMomentumMin = val;
         fNeedsRefreshed = true;
         TList * vars = GetPSVariables("momentum");
         for(int i = 0; i< vars->GetEntries() ; i++){
            InSANEPhaseSpaceVariable * aVar = (InSANEPhaseSpaceVariable*) vars->At(i);
            aVar->SetMinimum(val);
         }
         if( vars->GetEntries() == 0) Error("SetMomentumMax","\"momentum\" variable does not exist.");
      }

};


#endif
