#include "BETARunActionMessenger.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithoutParameter.hh"
#include "BETARunAction.hh"
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

BETARunActionMessenger::BETARunActionMessenger ( BETARunAction * theRunAction )
      :runAction ( theRunAction )
{
   aidaDir = new G4UIdirectory ( "/aida/" );
   aidaDir->SetGuidance ( "Write aida tree RunAction control" );

   writeTree = new G4UIcmdWithoutParameter ( "/aida/writeTree",this );
   writeTree->SetGuidance ( "Write aida file" );
   writeTree->SetGuidance ( "  angle w.r.t. (k,n) plane" );
//   writeTree->SetParameterName("angle",true);
//   writeTree->SetUnitCategory("Angle");
//   writeTree->SetDefaultValue(-360.0);
//   writeTree->SetDefaultUnit("deg");
   writeTree->AvailableForStates ( G4State_Idle );

   showPlot = new G4UIcmdWithAnInteger ( "/aida/showPlot",this );
   showPlot->SetGuidance ( " Argument 1 shows histograms and graphs" );
   showPlot->SetGuidance ( " Argument 0 does not show anything" );
   showPlot->AvailableForStates ( G4State_Idle );

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

BETARunActionMessenger::~BETARunActionMessenger()
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void BETARunActionMessenger::SetNewValue (
   G4UIcommand* command, G4String newValue )
{
   if ( command == writeTree )
   {
      G4cout << " T1 " <<  G4endl;

   }
   if ( command == showPlot )
   {
      runAction->showPlot ( showPlot->GetNewIntValue ( newValue ) );
   }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

