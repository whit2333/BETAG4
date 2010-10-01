#include "G4ios.hh"
#include <iomanip>

#include "globals.hh"
#include "BETAPhysicsList.hh"
#include "BETAPhysicsListMessenger.hh"

#include "G4ParticleDefinition.hh"
#include "G4ParticleTypes.hh"
#include "G4ParticleTable.hh"

#include "G4Material.hh"
#include "G4MaterialTable.hh"

#include "G4ProcessManager.hh"
#include "G4ProcessVector.hh"

#include "G4Cerenkov.hh"
#include "G4Scintillation.hh"
#include "G4OpAbsorption.hh"
#include "G4OpRayleigh.hh"
#include "G4OpBoundaryProcess.hh"

#include "G4GammaConversionToMuons.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

BETAPhysicsList::BETAPhysicsList() :  G4VUserPhysicsList()
{
   theCerenkovProcess           = 0;
   theScintillationProcess      = 0;
   theAbsorptionProcess         = 0;
   theRayleighScatteringProcess = 0;
   theBoundaryProcess           = 0;
   defaultCutValue = 1.0*mm;
   pMessenger = new BETAPhysicsListMessenger ( this );
   SetVerboseLevel ( 0 );
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

BETAPhysicsList::~BETAPhysicsList() {
   delete pMessenger;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void BETAPhysicsList::ConstructParticle()
{
   // In this method, static member functions should be called
   // for all particles which you want to use.
   // This ensures that objects of these particle types will be
   // created in the program.

   ConstructBosons();
   ConstructLeptons();
   ConstructMesons();
   ConstructBaryons();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void BETAPhysicsList::ConstructBosons()
{
   // pseudo-particles
   G4Geantino::GeantinoDefinition();
   G4ChargedGeantino::ChargedGeantinoDefinition();

   // gamma
   G4Gamma::GammaDefinition();

   // optical photon
   G4OpticalPhoton::OpticalPhotonDefinition();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void BETAPhysicsList::ConstructLeptons()
{
   // leptons
   G4Electron::ElectronDefinition();
   G4Positron::PositronDefinition();
   G4NeutrinoE::NeutrinoEDefinition();
   G4AntiNeutrinoE::AntiNeutrinoEDefinition();
   G4MuonPlus::MuonPlusDefinition();
   G4MuonMinus::MuonMinusDefinition();
   G4NeutrinoMu::NeutrinoMuDefinition();
   G4AntiNeutrinoMu::AntiNeutrinoMuDefinition();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void BETAPhysicsList::ConstructMesons()
{
//  mesons
   G4PionPlus::PionPlusDefinition();
   G4PionMinus::PionMinusDefinition();
   G4PionZero::PionZeroDefinition();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void BETAPhysicsList::ConstructBaryons()
{
//  barions
   G4Proton::ProtonDefinition();
   G4AntiProton::AntiProtonDefinition();
   G4Neutron::NeutronDefinition();
   G4AntiNeutron::AntiNeutronDefinition();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void BETAPhysicsList::ConstructProcess()
{
   AddTransportation();
   ConstructGeneral();
   ConstructEM();
   ConstructOp();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "G4Decay.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void BETAPhysicsList::ConstructGeneral()
{
   // Add Decay Process
   G4Decay* theDecayProcess = new G4Decay();
   theParticleIterator->reset();
   while ( ( *theParticleIterator ) () )
   {
      G4ParticleDefinition* particle = theParticleIterator->value();
      G4ProcessManager* pmanager = particle->GetProcessManager();
      if ( theDecayProcess->IsApplicable ( *particle ) )
      {
         pmanager ->AddProcess ( theDecayProcess );
         // set ordering for PostStepDoIt and AtRestDoIt
         pmanager ->SetProcessOrdering ( theDecayProcess, idxPostStep );
         pmanager ->SetProcessOrdering ( theDecayProcess, idxAtRest );
      }
   }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "G4ComptonScattering.hh"
#include "G4GammaConversion.hh"
#include "G4PhotoElectricEffect.hh"

#include "G4eMultipleScattering.hh"
#include "G4MuMultipleScattering.hh"
#include "G4MultipleScattering.hh"

#include "G4eIonisation.hh"
#include "G4eBremsstrahlung.hh"
#include "G4eplusAnnihilation.hh"

#include "G4MuIonisation.hh"
#include "G4MuBremsstrahlung.hh"
#include "G4MuPairProduction.hh"

#include "G4hIonisation.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void BETAPhysicsList::ConstructEM()
{
   theParticleIterator->reset();
   while ( ( *theParticleIterator ) () )
   {
      G4ParticleDefinition* particle = theParticleIterator->value();
      G4ProcessManager* pmanager = particle->GetProcessManager();
      G4String particleName = particle->GetParticleName();

      if ( particleName == "gamma" )
      {
         // gamma
         // Construct processes for gamma
         pmanager->AddDiscreteProcess ( new G4GammaConversion() );
         pmanager->AddDiscreteProcess ( new G4ComptonScattering() );
         pmanager->AddDiscreteProcess ( new G4PhotoElectricEffect() );
         pmanager->AddDiscreteProcess ( new G4GammaConversionToMuons() );

      }
      else if ( particleName == "e-" )
      {
         //electron
         // Construct processes for electron
         pmanager->AddProcess ( new G4eMultipleScattering(),-1, 1, 1 );
         pmanager->AddProcess ( new G4eIonisation(),       -1, 2, 2 );
         pmanager->AddProcess ( new G4eBremsstrahlung(),   -1, 3, 3 );

      }
      else if ( particleName == "e+" )
      {
         //positron
         // Construct processes for positron
         pmanager->AddProcess ( new G4eMultipleScattering(),-1, 1, 1 );
         pmanager->AddProcess ( new G4eIonisation(),       -1, 2, 2 );
         pmanager->AddProcess ( new G4eBremsstrahlung(),   -1, 3, 3 );
         pmanager->AddProcess ( new G4eplusAnnihilation(),  0,-1, 4 );

      }
      else if ( particleName == "mu+" ||
                particleName == "mu-" )
      {
         //muon
         // Construct processes for muon
         pmanager->AddProcess ( new G4MuMultipleScattering(),-1, 1, 1 );
         pmanager->AddProcess ( new G4MuIonisation(),      -1, 2, 2 );
         pmanager->AddProcess ( new G4MuBremsstrahlung(),  -1, 3, 3 );
         pmanager->AddProcess ( new G4MuPairProduction(),  -1, 4, 4 );

      }
      else
      {
         if ( ( particle->GetPDGCharge() != 0.0 ) &&
              ( particle->GetParticleName() != "chargedgeantino" ) )
         {
            // all others charged particles except geantino
            pmanager->AddProcess ( new G4MultipleScattering(),-1,1,1 );
            pmanager->AddProcess ( new G4hIonisation(),       -1,2,2 );
         }
      }
   }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void BETAPhysicsList::ConstructOp()
{
   theCerenkovProcess           = new G4Cerenkov ( "Cerenkov" );
   theScintillationProcess = new G4Scintillation ( "Scintillation" );
   theAbsorptionProcess     = new G4OpAbsorption();
   theRayleighScatteringProcess = new G4OpRayleigh();
   theBoundaryProcess  = new G4OpBoundaryProcess();

//  theCerenkovProcess->DumpPhysicsTable();
//  theScintillationProcess->DumpPhysicsTable();
//  theAbsorptionProcess->DumpPhysicsTable();
//  theRayleighScatteringProcess->DumpPhysicsTable();

   SetVerbose ( 0 );

   theCerenkovProcess->SetMaxNumPhotonsPerStep ( 300 );
   theCerenkovProcess->SetTrackSecondariesFirst ( true );

   theScintillationProcess->SetScintillationYieldFactor ( 1. );
   theScintillationProcess->SetTrackSecondariesFirst ( true );

   G4OpticalSurfaceModel themodel = unified;
   theBoundaryProcess->SetModel ( themodel );

   theParticleIterator->reset();
   while ( ( *theParticleIterator ) () )
   {
      G4ParticleDefinition* particle = theParticleIterator->value();
      G4ProcessManager* pmanager = particle->GetProcessManager();
      G4String particleName = particle->GetParticleName();
      if ( theCerenkovProcess->IsApplicable ( *particle ) )
      {
         // pmanager->AddContinuousProcess ( theCerenkovProcess );
         // per Release 9.1,
         pmanager->AddProcess(theCerenkovProcess);
         pmanager->SetProcessOrdering(theCerenkovProcess,idxPostStep);
      }
      if ( theScintillationProcess->IsApplicable ( *particle ) )
      {
         pmanager->AddProcess ( theScintillationProcess );
         pmanager->SetProcessOrderingToLast ( theScintillationProcess, idxAtRest );
         pmanager->SetProcessOrderingToLast ( theScintillationProcess, idxPostStep );
      }
      if ( particleName == "opticalphoton" )
      {
         G4cout << " AddDiscreteProcess to OpticalPhoton " << G4endl;
         pmanager->AddDiscreteProcess ( theAbsorptionProcess );
         pmanager->AddDiscreteProcess ( theRayleighScatteringProcess );
         pmanager->AddDiscreteProcess ( theBoundaryProcess );
      }
   }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void BETAPhysicsList::SetVerbose ( G4int verbose )
{
   theCerenkovProcess->SetVerboseLevel ( verbose );
   theScintillationProcess->SetVerboseLevel ( verbose );
   theAbsorptionProcess->SetVerboseLevel ( verbose );
   theRayleighScatteringProcess->SetVerboseLevel ( verbose );
   theBoundaryProcess->SetVerboseLevel ( verbose );
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void BETAPhysicsList::SetNbOfPhotonsCerenkov ( G4int MaxNumber )
{
   theCerenkovProcess->SetMaxNumPhotonsPerStep ( MaxNumber );
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void BETAPhysicsList::SetCuts()
{
   //  " G4VUserPhysicsList::SetCutsWithDefault" method sets
   //   the default cut value for all particle types
   //
   SetCutsWithDefault();

   if ( verboseLevel>0 ) DumpCutValuesTable();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
