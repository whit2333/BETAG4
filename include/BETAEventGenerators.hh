#ifndef BETAEventGenerators_HH
#define BETAEventGenerators_HH 1

#include "BETAG4EventGenerator.hh"
//#include "Randomize.hh"
//#include "G4ThreeVector.hh"
//#include "TMath.h"
//#include "InSANEPhaseSpace.h"
//#include "HallCBeam.h"

/** Basic event generators.
 *  These simple event generators do not use cross sections.
 *  For more event generators see SANEEventGenerators.h.
 * 
 */

//______________________________________________________________________________


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
//______________________________________________________________________________


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
//______________________________________________________________________________

#endif

