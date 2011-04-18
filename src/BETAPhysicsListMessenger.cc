#include "BETAPhysicsListMessenger.hh"

#include "BETAPhysicsList.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAnInteger.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

BETAPhysicsListMessenger::BETAPhysicsListMessenger ( BETAPhysicsList* pPhys )
      :pPhysicsList ( pPhys )
{
   BETADir = new G4UIdirectory ( "/beta/" );
   BETADir->SetGuidance ( "UI commands of this example" );

   physDir = new G4UIdirectory ( "/beta/phys/" );
   physDir->SetGuidance ( "PhysicsList control" );

   verboseCmd = new G4UIcmdWithAnInteger ( "/beta/phys/verbose",this );
   verboseCmd->SetGuidance ( "Set verbose for physics processes" );
   verboseCmd->SetParameterName ( "verbose",true );
   verboseCmd->SetDefaultValue ( 0 );
   verboseCmd->SetRange ( "verbose>=0" );
   verboseCmd->AvailableForStates ( G4State_PreInit,G4State_Idle );

   cerenkovCmd = new G4UIcmdWithAnInteger ( "/beta/phys/cerenkovMaxPhotons",this );
   cerenkovCmd->SetGuidance ( "Set max # of photons per step" );
   cerenkovCmd->SetParameterName ( "MaxNumber",false );
   cerenkovCmd->SetRange ( "MaxNumber>=0" );
   cerenkovCmd->AvailableForStates ( G4State_Idle );
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

BETAPhysicsListMessenger::~BETAPhysicsListMessenger()
{
   delete verboseCmd;
   delete cerenkovCmd;
   delete physDir;
   delete BETADir;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void BETAPhysicsListMessenger::SetNewValue ( G4UIcommand* command,
                                             G4String newValue )
{
   if ( command == verboseCmd )
   {
      pPhysicsList->SetVerbose ( verboseCmd->GetNewIntValue ( newValue ) );
   }

   if ( command == cerenkovCmd )
   {
      pPhysicsList->SetNbOfPhotonsCerenkov ( cerenkovCmd->GetNewIntValue ( newValue ) );
   }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
