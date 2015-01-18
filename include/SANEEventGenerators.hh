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


/** Pions only.  INCOMPLETE
 *  Throws pi0, pi+ and pi- pions.
 *
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
      InclusiveElectronPionGenerator(InSANETarget * targ = 0);
      virtual ~InclusiveElectronPionGenerator();
      virtual void InitializeMaterialXSec(const Int_t i, const Double_t weight, const InSANETargetMaterial * mat, const InSANENucleus * targ);
      virtual void Initialize();

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
//______________________________________________________________________________


/** Uses A1,A2 models
 * 
 *   \ingroup EventGen
 */
class PolarizedInclusiveDISEventGenerator : public BETAG4EventGenerator  {
   public :
      PolarizedInclusiveDISEventGenerator(){}
      virtual ~PolarizedInclusiveDISEventGenerator() { }
};
//______________________________________________________________________________


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
//______________________________________________________________________________



#endif

