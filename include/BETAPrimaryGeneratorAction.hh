#ifndef BETAPrimaryGeneratorAction_h
#define BETAPrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "globals.hh"

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
    void setIsotropic(G4int);
    void setSigmaMomentum(G4double);
    void setMomentum(G4double);
    void setElectronPionRatio(G4double);
    void setPiZeroRatio(G4double);

    void setPartTheta ( G4double P );
    G4double getPartTheta ();
    void setPartPhi ( G4double P );
    G4double getPartPhi ();
    G4double theta_particle,phi_particle;
    G4double momentum;
    G4double sigmaMomentum;
    G4double eventTheta;
    G4double eventPhi;
    G4double eventEnergy;
    G4double eventMomentum;

  private:

	bool background;
	bool goodElectron;
	bool backgroundAndElectron;

    G4double sigmaTheta, sigmaPhi;

    G4ParticleGun* particleGun;
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

    G4ParticleGun* Gun() {return particleGun;}
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif /*BETAPrimaryGeneratorAction_h*/
