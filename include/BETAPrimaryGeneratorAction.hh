#ifndef BETAPrimaryGeneratorAction_h
#define BETAPrimaryGeneratorAction_h 1

#include "globals.hh"
#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4GeneralParticleSource.hh"
#include "G4ParticleGun.hh"

#include "BETAEventGenerators.hh"
#include "SANEEventGenerators.hh"
#include "BETASimulationManager.hh"

#include "InSANEPhaseSpaceSampler.h"
#include "InSANEPhaseSpaceSampler.h"
#include "InSANEPhaseSpace.h"
#include "InSANEInclusiveDiffXSec.h"
#include "BETAG4MonteCarloEvent.h"
#include "TClonesArray.h"

class G4Event;
class G4ParticleDefinition;

/** BETAPrimaryGeneratorAction is a mandatory concrete imp of G4VUserPrimaryGeneratorAction
 *
 *   Geant4 Mandatory Class 
 *
 *   \ingroup EventGen
 */
class BETAPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction {

   private:
      Int_t   fNumberOfParticles;
      //bool    background;
      //bool    goodElectron;
      //bool    backgroundAndElectron;

      //BETAPrimaryGeneratorMessenger * gunMessenger;
      G4ParticleDefinition          * electron;
      G4ParticleDefinition          * positron;
      G4ParticleDefinition          * pionminus;
      G4ParticleDefinition          * pionplus;
      G4ParticleDefinition          * pionzero;
      G4ParticleDefinition          * kaon;
      G4ParticleDefinition          * proton;
      G4ParticleDefinition          * gamma;
      InSANEParticle                * aThrownParticle;

   protected:
      BETAG4EventGenerator  * fBETAG4EventGen;
      bool                    fInitialized;      ///< flag telling event-generator to be initialized
      BETAG4MonteCarloEvent * fMonteCarloEvent;
      TClonesArray          * fThrownParticles;
      G4ParticleGun         * fParticleGun;

   public:

      BETAPrimaryGeneratorAction();
      virtual ~BETAPrimaryGeneratorAction();

      void GeneratePrimaries(G4Event*);

      BETAG4EventGenerator  * GetEventGenerator() const { return fBETAG4EventGen; }
      void                    SetEventGenerator(BETAG4EventGenerator * gen){
         if(fBETAG4EventGen) delete fBETAG4EventGen;
         fBETAG4EventGen = gen;
      }

      void SetMCEventAddress(BETAG4MonteCarloEvent * mcevent) {
         //std::cout << "BETAPrimaryGeneratorAction::SetMCEventAddress()\n";
         /*       if(mcevent) {*/
         fMonteCarloEvent = mcevent;
         fThrownParticles = fMonteCarloEvent->GetThrownParticlesArray();
         //std::cout << " tracker " << fMonteCarloEvent->GetTrackerPlaneHitsArray() << "\n";
         //std::cout << " bigcal " << fMonteCarloEvent->GetBigcalPlaneHitsArray() << "\n";
         //        }
      }

      TTree * fOutputTree;
      void InitOutput(){
         SANERunManager::GetRunManager()->GetCurrentFile()->cd();
         fOutputTree = new TTree("thrownEvents","Thrown MC Events");
         fOutputTree->Branch("fThrownParticles",&(fMonteCarloEvent->fThrownParticles));

      }

      //G4GeneralParticleSource* Gun() {return fParticlesSource;}
      G4ParticleGun * GetParticleGun() const { return  fParticleGun; }

};


#endif 

