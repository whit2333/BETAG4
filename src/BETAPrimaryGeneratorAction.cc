#include "BETAPrimaryGeneratorAction.hh"
#include "BETAPrimaryGeneratorMessenger.hh"
#include "G4Gamma.hh"
#include "Randomize.hh"
#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4RandomDirection.hh"
#include "fstream"

using namespace std;
double mottCrossSection(double p, double theta) ;

/**
 * \brief Define a Paritlce gun
 * 
 **/
BETAPrimaryGeneratorAction::BETAPrimaryGeneratorAction() : iso ( 1 ),momentum ( 2.0 ),sigmaMomentum ( 1.0),ElectronPionRatio ( 1.0 ),Pi0Ratio ( 0.5 ),theta_particle(40),phi_particle(0)
{
   G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
   G4String particleName;
   particleGun  = new G4ParticleGun ( 1 );

//   ofstream file;
//   file.open("mott_test.out");
//   int ww = 0;
/*   double p;
   for (ww=0;ww<10;ww++) {
      p =(double)ww*0.1;*/
//      file << p << " " << mottCrossSection(p,0.1)  << G4endl;
//    }
if(1) {

   pionzero = particleTable->FindParticle ( particleName="pi0" );
   particleGun->SetParticleDefinition ( pionzero );

} else {

/// Pick primary events for study...
   background = false;
   ElectronPionRatio = 0.50; // Ratio of electrons to pions (charged and neutral) (1.0 is only electrons)
   Pi0Ratio = 0.5; // Ratio of pi0's to charged pions (1.0 is only pi0's)
   goodElectron = false;
   backgroundAndElectron = false;


   G4cout << "primary Generator constructor" << G4endl;

   //create a messenger for this class
   gunMessenger = new BETAPrimaryGeneratorMessenger ( this );

   electron = particleTable->FindParticle ( particleName="e-" );
   pionminus = particleTable->FindParticle ( particleName="pi-" );
   pionplus = particleTable->FindParticle ( particleName="pi+" );
   pionzero = particleTable->FindParticle ( particleName="pi0" );
   kaon = particleTable->FindParticle ( particleName="kaon+" );
   proton = particleTable->FindParticle ( particleName="proton" );

/// TODO: Sample position from some volume to account for target thickness
   particleGun->SetParticlePosition ( G4ThreeVector ( 0.,0.,0.0*cm ) );
// Default particle is electron of 1 GeV
   particleGun->SetParticleDefinition ( electron );
// particleGun->SetParticleEnergy ( 1000*MeV );

}
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
BETAPrimaryGeneratorAction::~BETAPrimaryGeneratorAction()
{
   delete particleGun;
   delete gunMessenger;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void BETAPrimaryGeneratorAction::GeneratePrimaries ( G4Event* anEvent )
{
      double E,deltatheta, theta,deltaphi,phi,sigmaOverN, N,sigma,x,y,tempmom;

   bool bigcal_background = true;
   if (bigcal_background) {
deltatheta = 15.0*pi/180.0;//deltatheta
deltaphi   = pi/3.0;

theta   = 40.0*pi/180.0 + 2.*(G4UniformRand()-0.5)*deltatheta;//deltatheta
phi     = pi/2.0 + 2.*(G4UniformRand()-0.5)*deltaphi;

     particleGun->SetParticlePosition ( G4ThreeVector(
       2.*(G4UniformRand()-0.5)*1.0*cm,2.*(G4UniformRand()-0.5)*1.0*cm,
       2.*(G4UniformRand()-0.5)*1.0*cm ) );
     particleGun->SetParticleMomentumDirection (
         G4ThreeVector (
            std::sin ( theta) *std::cos ( phi ) *m,
            std::sin ( theta ) *std::sin ( phi ) *m ,
            std::cos ( theta ) *m ) );
      particleGun->SetParticleEnergy ( 0.4+sigmaMomentum*G4UniformRand()*1000.0*MeV );
   }
   else {
/*      bool straightOnDetector = true;
      double E, theta,phi,sigmaOverN, N,sigma,x,y,tempmom;
      background=false;
/// WARNING HARD CODING OVERTOP stupid me i know
//theta_particle = 40;
      phi_particle = 90.0; //
//particleGun->SetParticleDefinition ( G4Gamma::GammaDefinition() );
      theta = ( theta_particle + 20.*2.0* ( G4UniformRand()-0.5 ) ) *pi/180. ;
      phi = ( 35.0* 2.0* ( G4UniformRand()-0.5 ) +phi_particle) *pi/180. ;
//  theta = -1.0* ( theta_particle  ) *pi/180.;
//  phi = ( phi_particle ) *pi/180.;
      particleGun->SetParticleMomentumDirection (
         G4ThreeVector (
            std::sin ( theta ) *std::cos ( phi ) *m,
            std::sin ( theta ) *std::sin ( phi ) *m ,
            std::cos ( theta ) *m ) );
      double tempmom= (momentum+(G4UniformRand()-0.5)*2.0*sigmaMomentum)*GeV;
      eventTheta = theta;
      eventPhi=phi;
      particleGun->SetParticleEnergy ( tempmom );
      eventMomentum=tempmom;
      eventEnergy = std::sqrt(tempmom*tempmom+0.000511*0.000511*GeV*GeV);
      particleGun->GeneratePrimaryVertex ( anEvent );
*/
  }
      particleGun->GeneratePrimaryVertex ( anEvent );

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void BETAPrimaryGeneratorAction::SetOptPhotonPolar()
{
   G4double angle = G4UniformRand() * 360.0*deg;
   SetOptPhotonPolar ( angle );
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void BETAPrimaryGeneratorAction::SetOptPhotonPolar ( G4double angle )
{
   if ( particleGun->GetParticleDefinition()->GetParticleName() != "opticalphoton" )
   {
      G4cout << "--> warning from PrimaryGeneratorAction::SetOptPhotonPolar() :"
      "the particleGun is not an opticalphoton" << G4endl;
      return;
   }

   G4ThreeVector normal ( 1., 0., 0. );
   G4ThreeVector kphoton = particleGun->GetParticleMomentumDirection();
   G4ThreeVector product = normal.cross ( kphoton );
   G4double modul2       = product*product;

   G4ThreeVector e_perpend ( 0., 0., 1. );
   if ( modul2 > 0. ) e_perpend = ( 1./std::sqrt ( modul2 ) ) *product;
   G4ThreeVector e_paralle    = e_perpend.cross ( kphoton );

   G4ThreeVector polar = std::cos ( angle ) *e_paralle + std::sin ( angle ) *e_perpend;
   particleGun->SetParticlePolarization ( polar );
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void BETAPrimaryGeneratorAction::setIsotropic ( G4int set )
{

   iso = set;
//else particleGun->SetParticleMomentumDirection( getBETASolidAngle() );
}

void BETAPrimaryGeneratorAction::setMomentum ( G4double P )
{

   momentum = P;

}

void BETAPrimaryGeneratorAction::setSigmaMomentum ( G4double P )
{

   sigmaMomentum = P;
}

void BETAPrimaryGeneratorAction::setPartTheta ( G4double P )
{

   theta_particle = P;
}


G4double BETAPrimaryGeneratorAction::getPartTheta (  )
{

   return( theta_particle );
}


void BETAPrimaryGeneratorAction::setPartPhi ( G4double P )
{

   phi_particle = P;
}


G4double BETAPrimaryGeneratorAction::getPartPhi (  )
{

   return( phi_particle );
}

void BETAPrimaryGeneratorAction::setElectronPionRatio ( G4double Ratio )
{

   ElectronPionRatio = Ratio;
}

void BETAPrimaryGeneratorAction::setPiZeroRatio ( G4double Ratio )
{

   Pi0Ratio = Ratio;
}


double mottCrossSection(double p, double theta) {
// MeV and degrees
   double alphaSquared =(1.0/137.0)*(1.0/137.0);
   double hbarSquared = 6.5822e-22*6.5822e-22;
   double mass = 0.938;
   double E = std::sqrt(p*p+mass*mass);

   double res = (alphaSquared*E*E/(4.0*p*p*p*p*std::pow(std::sin(theta/2.0),4)) )*(1.0-std::pow(p*std::sin(theta/2.0)/E,2));
//double res1 = alphaSquared *hbarSquared /((2* p*p*std::sin(theta*pi/360.)*std::sin(theta*pi/360.))*(2* p*p* std::sin(theta*pi/360.)*std::sin(theta*pi/360.))) ;
//double res2 = 938.28*938.28+ p*p*std::cos(theta*pi/360.)*std::cos(theta*pi/360.);

   return(res);
}
