#include "BETAEventActionMessenger.hh"
#include "BETAEventAction.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4ios.hh"

BETAEventActionMessenger::BETAEventActionMessenger ( BETAEventAction * mpga )
      :target ( mpga )
{
   verboseCmd = new G4UIcmdWithAnInteger ( "/mydet/verbose",this );
   verboseCmd->SetGuidance ( "Verbose level for each event." );
   verboseCmd->SetGuidance ( " Event summary will be displayed for every 'level' events." );
   verboseCmd->SetParameterName ( "level",true );
   verboseCmd->SetRange ( "level>=0" );
   verboseCmd->SetDefaultValue ( 1 );
}

BETAEventActionMessenger::~BETAEventActionMessenger()
{
   delete verboseCmd;
}

void BETAEventActionMessenger::SetNewValue ( G4UIcommand * command,G4String newValue )
{
   if ( command==verboseCmd )
   {
      target->SetVerbose ( verboseCmd->GetNewIntValue ( newValue ) );
   }
}

G4String BETAEventActionMessenger::GetCurrentValue ( G4UIcommand * command )
{
   G4String cv;
   if ( command==verboseCmd )
   {
      cv = verboseCmd->ConvertToString ( target->GetVerbose() );
   }

   return cv;
}
