#ifndef BETAG4EventGenerator_HH
#define BETAG4EventGenerator_HH 1
#include "Randomize.hh"
#include "InSANEPhaseSpaceSampler.h"
#include "InSANEPhaseSpace.h"
#include "InSANEInclusiveDiffXSec.h"
#include "G4ThreeVector.hh"
#include "TMath.h"
#include "InSANETargetEventGenerator.h"
#include "F1F209eInclusiveDiffXSec.h"
#include "TParticle.h"
#include "InSANEFunctionManager.h"

/** Base class for InSANE -> BETAG4.
 *  By default it assumes there is only one particle and thus 3 random variables
 *
 *   \ingroup EventGen
 */ 
class BETAG4EventGenerator : public InSANETargetEventGenerator {
   protected:
      double fThetaTarget;

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

      virtual bool HasPolarizedTarget(){ return false; }
      double       GetThetaTarget(){ return fThetaTarget; }
      void         SetThetaTarget(double angle) { fThetaTarget = angle; }

   public:

      /** Returns the starting postiong of the primary particle. */
      virtual  G4ThreeVector &  GetInitialPosition(TParticle * p);

      /** Returns the starting direction of the primary particle.
       * \deprecated
       */
      virtual  G4ThreeVector &  GetInitialDirection(TParticle * p);

      /// \todo assuming particle is electron (eg E=P))

      /** Returns the starting postiong of the primary particle
       * \deprecated
       */
      virtual  G4ThreeVector &  GetMomentumVector(TParticle * p);

      virtual double GetParticleEnergy(TParticle * p);

      void SetThetaMax    ( double val);
      void SetThetaMin    ( double val);
      void SetPhiMax      ( double val);
      void SetPhiMin      ( double val);
      void SetEnergyMax   ( double val);
      void SetEnergyMin   ( double val);
      void SetMomentumMax ( double val);
      void SetMomentumMin ( double val);

};


#endif
