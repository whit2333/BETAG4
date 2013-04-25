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
#include "G4GeneralParticleSource.hh"
#include "G4SingleParticleSource.hh"
#include "G4SPSPosDistribution.hh"
#include "G4SPSEneDistribution.hh"
#include "G4SPSAngDistribution.hh"
#include "BETASimulationManager.hh"

double mottCrossSection(double p, double theta) ;

BETAPrimaryGeneratorAction::BETAPrimaryGeneratorAction() {

   G4cout << "BETAPrimaryGeneratorAction constructor" << G4endl;
   
   fBETAG4EventGen = new BETAG4EventGenerator();
   //fBETAG4EventGen->Initialize();
   //fBETAG4EventGen->fIsInitialized = true;
   fOutputTree = 0;

   gunMessenger = new BETAPrimaryGeneratorMessenger ( this );
  
   // ---- setup particles 

   G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
   fParticleGun = new G4ParticleGun(1);

   G4String particleName;
   electron = particleTable->FindParticle ( particleName="e-" );
   positron = particleTable->FindParticle ( particleName="e+" );
   pionminus = particleTable->FindParticle ( particleName="pi-" );
   pionplus = particleTable->FindParticle ( particleName="pi+" );
   pionzero = particleTable->FindParticle ( particleName="pi0" );
   kaon = particleTable->FindParticle ( particleName="kaon+" );
   proton = particleTable->FindParticle ( particleName="proton" );
   gamma = particleTable->FindParticle ( particleName="gamma" );

   /// Electron by default. Use /beta/gun/setParticle
   fParticleGun->SetParticleDefinition(electron);

   fParticlesSource=0;
   //fParticlesSource  = new G4GeneralParticleSource ( );
   //fParticlesSource->SetNumberOfParticles(1);
   // Using General Particle Source
   //   fParticlesSource->SetParticleDefinition(electron);
   //   fParticlesSource->SetCurrentSourceto(1);
   //   fParticlesSource->GetCurrentSource()->GetEneDist()->SetEneDisType("Gauss");
   //   fParticlesSource->GetCurrentSource()->GetAngDist()->SetAngDisType("iso");
   //   fParticlesSource->GetCurrentSource()->GetPosDist()->SetEneDisType("Volume");
   /*   fSimulationManager = ;*/
   //   if(BETASimulationManager::GetInstance()->fEvents) std::cout << "fEvents Found !\n";
   //   fMonteCarloEvent = BETASimulationManager::GetInstance()->fEvents->MC;
   fMonteCarloEvent=0;
}
//________________________________________________________//

BETAPrimaryGeneratorAction::~BETAPrimaryGeneratorAction()
{
   if(fParticlesSource) delete fParticlesSource;
   if(gunMessenger) delete gunMessenger;
   if(fBETAG4EventGen) delete fBETAG4EventGen;
}
//________________________________________________________//

void BETAPrimaryGeneratorAction::GeneratePrimaries ( G4Event* anEvent ) {

   if(!fBETAG4EventGen->fIsInitialized) {
      fBETAG4EventGen->Initialize();
      fBETAG4EventGen->fIsInitialized = true;
   }
   if(fBETAG4EventGen->IsModified()) fBETAG4EventGen->Refresh();

   if(fMonteCarloEvent) {
      fMonteCarloEvent->ClearEvent("C");
   } else { std::cout << " NO MC EVENT!!!\n"; }

   Double_t * MCvect ;
   TParticle * aPart=0;

   // Generate the event and get the list of particles
   TList * parts = fBETAG4EventGen->GenerateEvent();
   // Loop over all particles to create. (usually just a single electron)

   for(int ipart = 0;ipart<parts->GetEntries();ipart++) {
      aPart = (TParticle*) parts->At(ipart);
      /*      aPart->Print();*/
      if( aPart->GetPdgCode() == 11) fParticleGun->SetParticleDefinition(electron);
      else if( aPart->GetPdgCode() == -11) fParticleGun->SetParticleDefinition(positron);
      else if( aPart->GetPdgCode() == 111) fParticleGun->SetParticleDefinition(pionzero);
      else if( aPart->GetPdgCode() == 211) fParticleGun->SetParticleDefinition(pionplus);
      else if( aPart->GetPdgCode() == -211) fParticleGun->SetParticleDefinition(pionminus);
      else if( aPart->GetPdgCode() == 22) fParticleGun->SetParticleDefinition(gamma);
      else fParticleGun->SetParticleDefinition(electron);

      fParticleGun->SetParticlePosition( fBETAG4EventGen->GetInitialPosition(aPart) );
      fParticleGun->SetParticleMomentumDirection(  fBETAG4EventGen->GetInitialDirection(aPart) );
      fParticleGun->SetParticleEnergy( (fBETAG4EventGen->GetParticleEnergy(aPart))*1000.0*MeV );

      /// Gather information about the event for simulated truth
      if(fMonteCarloEvent) {
         fMonteCarloEvent->fNumberOfParticlesThrown++;
         /// \todo Should Have a loop here 
         aThrownParticle = new((*fThrownParticles)[ipart]) BETAG4MonteCarloThrownParticle();
         (*(TParticle *)aThrownParticle) = (*aPart);
         // 
         aThrownParticle->fMomentum4Vector.SetXYZT(aThrownParticle->Px(),aThrownParticle->Py(),aThrownParticle->Pz(),aThrownParticle->Energy());
         if(fOutputTree) fOutputTree->Fill();
      }
      else printf("NO MC EVENT!!!\n");
      fParticleGun->GeneratePrimaryVertex ( anEvent );
   } // end of loop over particles to throw for this event. 

   /*   MCvect            = fBETAG4EventGen->GenerateEvent();*/
   //    fCurrentEnergy    = MCvect[0];
   //    fCurrentTheta     = MCvect[1];
   //    fCurrentPhi       = MCvect[2];
   /*   G4ThreeVector aDirection(1,0,0);
        aDirection.setRThetaPhi(1,fCurrentTheta,fCurrentPhi);*/
   //    G4ThreeVector aPosition( 2.*(G4UniformRand()-0.5)*1.0*cm,
   //                             2.*(G4UniformRand()-0.5)*1.0*cm,
   //                             2.*(G4UniformRand()-0.5)*1.0*cm );
   /*  fParticleGun->SetParticlePosition( aPosition );*/
   //   fParticleGun->SetParticlePosition( fBETAG4EventGen->GetInitialPosition() );

   /*
      G4ThreeVector aDirection(
      std::sin ( fCurrentTheta) *std::cos ( fCurrentPhi ) *m,
      std::sin ( fCurrentTheta ) *std::sin ( fCurrentPhi ) *m ,
      std::cos ( fCurrentTheta ) *m ); */

   //   fParticleGun->SetParticleMomentumDirection(  fBETAG4EventGen->GetInitialDirection() );s
   //   fParticleGun->SetParticleEnergy( (fBETAG4EventGen->GetParticleEnergy())*1000.0*MeV );




   //       for(int i=0;i<fBETAG4EventGen->fNumberOfParticlesThrown;i++) {
   //          
   //       }

   //   fParticleGun->GeneratePrimaryVertex ( anEvent );

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
