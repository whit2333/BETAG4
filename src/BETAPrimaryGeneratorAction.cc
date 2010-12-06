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

#include "InSANEPhaseSpaceSampler.h"
#include "InSANEInclusivePhaseSpace.h"
#include "InSANEInclusiveDiffXSec.h"

#include "G4GeneralParticleSource.hh"
#include "G4SingleParticleSource.hh"
#include "G4SPSPosDistribution.hh"
#include "G4SPSEneDistribution.hh"
#include "G4SPSAngDistribution.hh"

using namespace std;
double mottCrossSection(double p, double theta) ;

/**
 * \brief Define 
 * 
 **/
BETAPrimaryGeneratorAction::BETAPrimaryGeneratorAction() 
{

///\todo allow for phase space and xsec to be controlled a bit better???...
  fPhaseSpace = new InSANEInclusivePhaseSpace();
  fDiffXSec = new InSANEInclusiveDiffXSec();
  fDiffXSec->SetPhaseSpace(fPhaseSpace);
  fEventSampler = new InSANEPhaseSpaceSampler(fDiffXSec);

  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();

   G4String particleName;

   fParticleGun = new G4ParticleGun(1);
   //fParticlesSource  = new G4GeneralParticleSource ( );
   //fParticlesSource->SetNumberOfParticles(1);
   G4cout << "primary Generator constructor" << G4endl;

   //create a messenger for this class
   gunMessenger = new BETAPrimaryGeneratorMessenger ( this );

   electron = particleTable->FindParticle ( particleName="e-" );
   pionminus = particleTable->FindParticle ( particleName="pi-" );
   pionplus = particleTable->FindParticle ( particleName="pi+" );
   pionzero = particleTable->FindParticle ( particleName="pi0" );
   kaon = particleTable->FindParticle ( particleName="kaon+" );
   proton = particleTable->FindParticle ( particleName="proton" );

// Using  particle gun
  fParticleGun->SetParticleDefinition(electron);

// Using General Particle Source
//   fParticlesSource->SetParticleDefinition(electron);
//   fParticlesSource->SetCurrentSourceto(1);
//   fParticlesSource->GetCurrentSource()->GetEneDist()->SetEneDisType("Gauss");
//   fParticlesSource->GetCurrentSource()->GetAngDist()->SetAngDisType("iso");
//   fParticlesSource->GetCurrentSource()->GetPosDist()->SetEneDisType("Volume");

}
//________________________________________________________//

BETAPrimaryGeneratorAction::~BETAPrimaryGeneratorAction()
{
   delete fParticlesSource;
   delete gunMessenger;
}
//________________________________________________________//

void BETAPrimaryGeneratorAction::GeneratePrimaries ( G4Event* anEvent )
{
  double deltatheta, deltaphi,sigmaOverN, N,sigma,x,y,tempmom;
  Double_t *MCvect ;
  MCvect =fEventSampler->GenerateEvent();
  fCurrentEnergy=MCvect[0];
  fCurrentTheta=MCvect[1];
  fCurrentPhi=MCvect[2];

  fParticleGun->SetParticlePosition ( G4ThreeVector(
    2.*(G4UniformRand()-0.5)*1.0*cm,2.*(G4UniformRand()-0.5)*1.0*cm,
    2.*(G4UniformRand()-0.5)*1.0*cm ) );

  fParticleGun->SetParticleMomentumDirection (
         G4ThreeVector (
            std::sin ( fCurrentTheta) *std::cos ( fCurrentPhi ) *m,
            std::sin ( fCurrentTheta ) *std::sin ( fCurrentPhi ) *m ,
            std::cos ( fCurrentTheta ) *m ) );
  fParticleGun->SetParticleEnergy( (fCurrentEnergy)*1000.0*MeV );

  fParticleGun->GeneratePrimaryVertex ( anEvent );
//deltaphi   = 30.0*pi/180.0;
//    phi     =  2.*(G4UniformRand()-0.5)*deltaphi;
/*
   bool bigcal_background = true;
   if (bigcal_background) {
deltatheta = 10.0*pi/180.0;//deltatheta
deltaphi   = 20.0*pi/180.0;

theta   = 40.0*pi/180.0 + 2.*(G4UniformRand()-0.5)*deltatheta;//deltatheta
phi     =  2.*(G4UniformRand()-0.5)*deltaphi;

     particleGun->SetParticlePosition ( G4ThreeVector(
       2.*(G4UniformRand()-0.5)*1.0*cm,2.*(G4UniformRand()-0.5)*1.0*cm,
       2.*(G4UniformRand()-0.5)*1.0*cm ) );
     particleGun->SetParticleMomentumDirection (
         G4ThreeVector (
            std::sin ( theta) *std::cos ( phi ) *m,
            std::sin ( theta ) *std::sin ( phi ) *m ,
            std::cos ( theta ) *m ) );
      particleGun->SetParticleEnergy ( (2.4+sigmaMomentum*G4UniformRand())*1000.0*MeV );
   }
   else {*/
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
//  }
//     Double_t *MCvect 
//     =new Double_t[3]; // 2-dim vector generated in the MC run
//      fEventSampler->FoamX->MakeEvent();          // generate MC event
//      fEventSampler->FoamX->GetMCvect( MCvect);   // get generated vector (x,y)

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void BETAPrimaryGeneratorAction::SetOptPhotonPolar()
{
   G4double angle = G4UniformRand() * 360.0*deg;
   SetOptPhotonPolar ( angle );
}
//________________________________________________________//

void BETAPrimaryGeneratorAction::SetOptPhotonPolar ( G4double angle )
{
   if ( fParticleGun->GetParticleDefinition()->GetParticleName() != "opticalphoton" )
   {
      G4cout << "--> warning from PrimaryGeneratorAction::SetOptPhotonPolar() :"
      "the particleGun is not an opticalphoton" << G4endl;
      return;
   }

   G4ThreeVector normal ( 1., 0., 0. );
   G4ThreeVector kphoton = fParticleGun->GetParticleMomentumDirection();
   G4ThreeVector product = normal.cross ( kphoton );
   G4double modul2       = product*product;

   G4ThreeVector e_perpend ( 0., 0., 1. );
   if ( modul2 > 0. ) e_perpend = ( 1./std::sqrt ( modul2 ) ) *product;
   G4ThreeVector e_paralle    = e_perpend.cross ( kphoton );

   G4ThreeVector polar = std::cos ( angle ) *e_paralle + std::sin ( angle ) *e_perpend;
   fParticleGun->SetParticlePolarization ( polar );
}
//________________________________________________________//

void BETAPrimaryGeneratorAction::SetIsotropic ( G4int set )
{

   iso = set;
//else particleGun->SetParticleMomentumDirection( getBETASolidAngle() );
}
//________________________________________________________//

void BETAPrimaryGeneratorAction::SetMomentum ( G4double P )
{

   momentum = P;

}
//________________________________________________________//

void BETAPrimaryGeneratorAction::SetSigmaMomentum ( G4double P )
{

   sigmaMomentum = P;
}
//________________________________________________________//

void BETAPrimaryGeneratorAction::SetPartTheta ( G4double P )
{

   theta_particle = P;
}
//________________________________________________________//

G4double BETAPrimaryGeneratorAction::GetPartTheta (  )
{

   return( theta_particle );
}
//________________________________________________________//

void BETAPrimaryGeneratorAction::SetPartPhi ( G4double P )
{

   phi_particle = P;
}
//________________________________________________________//

G4double BETAPrimaryGeneratorAction::GetPartPhi (  )
{

   return( phi_particle );
}
//________________________________________________________//

void BETAPrimaryGeneratorAction::SetElectronPionRatio ( G4double Ratio )
{

   ElectronPionRatio = Ratio;
}
//________________________________________________________//

void BETAPrimaryGeneratorAction::SetPiZeroRatio ( G4double Ratio )
{

   Pi0Ratio = Ratio;
}
//________________________________________________________//

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
//________________________________________________________//
