#ifndef BETAG4EventGenerator_HH
#define BETAG4EventGenerator_HH 1
#include "Randomize.hh"
#include "InSANEPhaseSpaceSampler.h"
#include "InSANEPhaseSpace.h"
#include "InSANEInclusiveDiffXSec.h"
#include "G4ThreeVector.hh"
#include "TMath.h"
#include "InSANETargetEventGenerator.h"
#include "InSANECompositeDiffXSec.h"
#include "TParticle.h"
#include "InSANEFunctionManager.h"


/** Base class for InSANE -> BETAG4.
 *  By default it assumes there is only one particle and thus 3 random variables
 *
 *   \ingroup EventGen
 */ 
class BETAG4EventGenerator : public BETAG4SavedEventGenerator {
   public:

      G4ThreeVector * fInitialPosition ;
      G4ThreeVector * fInitialDirection ;
      G4ThreeVector * fMomentumVector ;

   public:
      BETAG4EventGenerator();
      virtual ~BETAG4EventGenerator(); 

      BETAG4EventGenerator(const BETAG4EventGenerator & eg);

      /** Sets the cross section and creates the phase space sampler. 
       *  Must be called before using event generator
       *  and should first call InitializePhaseSpace() or take care of
       *  phase space definition.
       *  This is called from the constructor and each implementation
       *  requiring a different cross section should reimplement this method
       */
      virtual void Initialize();

      /** Override this instead of Initialize. 
       */
      virtual void InitializeMaterialXSec(const Int_t i, const Double_t weight, const InSANETargetMaterial * mat, const InSANENucleus * targ);

      virtual bool HasPolarizedTarget(){ return false; }
      double       GetThetaTarget(){ return fThetaTarget; }
      void         SetThetaTarget(double angle) { fThetaTarget = angle; }

   public:

      /** Returns the starting postiong of the primary particle. */
      virtual  G4ThreeVector&  GetInitialPosition(const TParticle * p);

      /** Returns the starting direction of the primary particle.
       * \deprecated
       */
      virtual  G4ThreeVector&  GetInitialDirection(const TParticle * p);

      /// \todo assuming particle is electron (eg E=P))

      /** Returns the starting postiong of the primary particle
       * \deprecated
       */
      virtual  G4ThreeVector &  GetMomentumVector(const TParticle * p);

      virtual double GetParticleEnergy(const TParticle * p);

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

