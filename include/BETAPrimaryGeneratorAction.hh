#ifndef BETAPrimaryGeneratorAction_h
#define BETAPrimaryGeneratorAction_h 1

#include "BETASimulationManager.hh"
#include "InSANEPhaseSpaceSampler.h"
#include "InSANEPhaseSpace.h"
#include "InSANEInclusiveDiffXSec.h"
#include "G4VUserPrimaryGeneratorAction.hh"
#include "globals.hh"
#include "InSANEPhaseSpaceSampler.h"
#include "G4GeneralParticleSource.hh"
#include "SANEEventGenerators.hh"
#include "BETAG4MonteCarloEvent.h"
#include "TClonesArray.h"

class G4ParticleGun;
class G4Event;
class BETAPrimaryGeneratorMessenger;
class G4ParticleDefinition;
// class BETASimulationManager;

/** BETAPrimaryGeneratorAction is a mandatory concrete imp of G4VUserPrimaryGeneratorAction
 *
 *   Geant4 Mandatory Class 
 *
 *   \ingroup EventGen
 */
class BETAPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction {
public:
   bool fInitialized;
   BETAG4MonteCarloEvent * fMonteCarloEvent;
   TClonesArray * fThrownParticles;
   BETAG4EventGenerator * fBETAG4EventGen;

private:
   BETAG4MonteCarloThrownParticle * aThrownParticle;

public:
   /** C'tor :
    *   - defines some particles 
    *   - Creates particle gun
    *   - Creates the event generator
    */
    BETAPrimaryGeneratorAction();
   ~BETAPrimaryGeneratorAction();

  public:
    void SetMCEventAddress(BETAG4MonteCarloEvent * mcevent) {
       std::cout << "BETAPrimaryGeneratorAction::SetMCEventAddress()\n";
/*       if(mcevent) {*/
          fMonteCarloEvent = mcevent;
          fThrownParticles = fMonteCarloEvent->GetThrownParticlesArray();
          std::cout << " tracker " << fMonteCarloEvent->GetTrackerPlaneHitsArray() << "\n";
          std::cout << " bigcal " << fMonteCarloEvent->GetBigcalPlaneHitsArray() << "\n";
//        }
    }

    void GeneratePrimaries(G4Event*);

//     void SetOptPhotonPolar();
//     void SetOptPhotonPolar(G4double);
//     void SetIsotropic(G4int);
//     void SetSigmaMomentum(G4double);
//     void SetMomentum(G4double);
//     void SetElectronPionRatio(G4double);
//     void SetPiZeroRatio(G4double);
//     void SetPartTheta ( G4double P );
//     G4double GetPartTheta ();
//     void SetPartPhi ( G4double P );
//     G4double GetPartPhi ();

/// most recent values for event generation
    G4double fCurrentEnergy,fCurrentTheta,fCurrentPhi;

//     G4double theta_particle,phi_particle;
//     G4double momentum;
//     G4double sigmaMomentum;
//     G4double eventTheta;
//     G4double eventPhi;
//     G4double eventEnergy;
//     G4double eventMomentum;

   G4GeneralParticleSource  * fParticlesSource;
   G4ParticleGun  * fParticleGun;
private:
        Int_t fNumberOfParticles;
	bool background;
	bool goodElectron;
	bool backgroundAndElectron;

//     G4double sigmaTheta, sigmaPhi;

    BETAPrimaryGeneratorMessenger* gunMessenger;
    G4ParticleDefinition* electron;
    G4ParticleDefinition* pionminus;
    G4ParticleDefinition* pionplus;
    G4ParticleDefinition* pionzero;
    G4ParticleDefinition* kaon;
    G4ParticleDefinition* proton;

//     G4double sigmaAngle;
// 	G4double ElectronPionRatio;
// 	G4double Pi0Ratio;
//     G4bool randomizePrimary;
//      G4int iso; 


  public:

    G4GeneralParticleSource* Gun() {return fParticlesSource;}
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif /*BETAPrimaryGeneratorAction_h*/
