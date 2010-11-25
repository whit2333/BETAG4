#ifndef BETAPrimaryGeneratorAction_h
#define BETAPrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "globals.hh"
#include "InSANEPhaseSpaceSampler.h"
#include "G4GeneralParticleSource.hh"

class G4ParticleGun;
class G4Event;
class BETAPrimaryGeneratorMessenger;
class G4ParticleDefinition;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class BETAPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
  public:
    BETAPrimaryGeneratorAction();
   ~BETAPrimaryGeneratorAction();

  public:
    void GeneratePrimaries(G4Event*);
    void SetOptPhotonPolar();
    void SetOptPhotonPolar(G4double);
    void SetIsotropic(G4int);
    void SetSigmaMomentum(G4double);
    void SetMomentum(G4double);
    void SetElectronPionRatio(G4double);
    void SetPiZeroRatio(G4double);

    void SetPartTheta ( G4double P );
    G4double GetPartTheta ();
    void SetPartPhi ( G4double P );
    G4double GetPartPhi ();

    G4double theta_particle,phi_particle;
    G4double momentum;
    G4double sigmaMomentum;
    G4double eventTheta;
    G4double eventPhi;
    G4double eventEnergy;
    G4double eventMomentum;
  
/**
 *  Used to control the phase space \todo{record at end of run xsec and phase space used}
 */
  InSANEPhaseSpaceSampler * fEventSampler;

/// 
  G4GeneralParticleSource  * fParticlesSource;
  G4ParticleGun  * fParticleGun;
  private:
        Int_t fNumberOfParticles;

	bool background;
	bool goodElectron;
	bool backgroundAndElectron;

    G4double sigmaTheta, sigmaPhi;

    BETAPrimaryGeneratorMessenger* gunMessenger;
    G4ParticleDefinition* electron;
    G4ParticleDefinition* pionminus;
    G4ParticleDefinition* pionplus;
    G4ParticleDefinition* pionzero;
    G4ParticleDefinition* kaon;
    G4ParticleDefinition* proton;

    G4double sigmaAngle;
	G4double ElectronPionRatio;
	G4double Pi0Ratio;
    G4bool randomizePrimary;
     G4int iso; 


  public:

    G4GeneralParticleSource* Gun() {return fParticlesSource;}
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif /*BETAPrimaryGeneratorAction_h*/
