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

double mottCrossSection(double p, double theta) ;

BETAPrimaryGeneratorAction::BETAPrimaryGeneratorAction() 
{
   G4cout << "BETAPrimaryGeneratorAction constructor" << G4endl;

   fMonteCarloEvent=0;
   fBETAG4EventGen = new BigcalCenterEventGenerator();

   gunMessenger = new BETAPrimaryGeneratorMessenger ( this );

   G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
   G4String particleName;
   fParticleGun = new G4ParticleGun(1);

   //fParticlesSource  = new G4GeneralParticleSource ( );
   //fParticlesSource->SetNumberOfParticles(1);

    electron = particleTable->FindParticle ( particleName="e-" );
    pionminus = particleTable->FindParticle ( particleName="pi-" );
    pionplus = particleTable->FindParticle ( particleName="pi+" );
    pionzero = particleTable->FindParticle ( particleName="pi0" );
    kaon = particleTable->FindParticle ( particleName="kaon+" );
    proton = particleTable->FindParticle ( particleName="proton" );

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

/// \todo improve event generator/Geant4 interface -> Number of particles thrown in particular
   Double_t * MCvect ;
   MCvect            = fBETAG4EventGen->GenerateEvent();
   fCurrentEnergy    = MCvect[0];
   fCurrentTheta     = MCvect[1];
   fCurrentPhi       = MCvect[2];
/*   G4ThreeVector aDirection(1,0,0);
      aDirection.setRThetaPhi(1,fCurrentTheta,fCurrentPhi);*/
   G4ThreeVector aPosition( 2.*(G4UniformRand()-0.5)*1.0*cm,
                            2.*(G4UniformRand()-0.5)*1.0*cm,
                            2.*(G4UniformRand()-0.5)*1.0*cm );
/// \todo implement target volume sampling 
  fParticleGun->SetParticlePosition( aPosition );

         G4ThreeVector aDirection(
            std::sin ( fCurrentTheta) *std::cos ( fCurrentPhi ) *m,
            std::sin ( fCurrentTheta ) *std::sin ( fCurrentPhi ) *m ,
            std::cos ( fCurrentTheta ) *m ); 
  fParticleGun->SetParticleMomentumDirection( aDirection );
  fParticleGun->SetParticleEnergy( (fCurrentEnergy)*1000.0*MeV );
  fParticleGun->SetParticleDefinition(electron);


   if(fMonteCarloEvent) {
/*      std::cout << " Filling thrown particle !!!!\n";*/
      fMonteCarloEvent->ClearEvent("C");
//   if(fBETAG4EventGen->fNumberOfGeneratedParticles == 1) {
      aThrownParticle = new((*fThrownParticles)[0]) BETAG4MonteCarloThrownParticle();
      aThrownParticle->fPosition.SetXYZ(aPosition.x()/cm,aPosition.y()/cm,aPosition.z()/cm);
      aThrownParticle->fMomentum.SetXYZ(aDirection.x()/MeV,aDirection.y()/MeV,aDirection.z()/MeV);
      aThrownParticle->fTheta = fCurrentTheta;
      aThrownParticle->fPhi = fCurrentPhi;
      aThrownParticle->fEnergy = (fCurrentEnergy)*1000.0;
      aThrownParticle->fPID = fParticleGun->GetParticleDefinition()->GetPDGEncoding();
      //aThrownParticle->fMomentum.
   }
//       for(int i=0;i<fBETAG4EventGen->fNumberOfParticlesThrown;i++) {
//          
//       }
  
  
  
  fParticleGun->GeneratePrimaryVertex ( anEvent );

}
//________________________________________________________
/*
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
//________________________________________________________//*/

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
