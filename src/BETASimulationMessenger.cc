#include "BETASimulationManager.hh"
#include "BETASimulationMessenger.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithoutParameter.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

BETASimulationMessenger::BETASimulationMessenger ( BETASimulationManager* mgr )
      :manager ( mgr )
{
   analysis = new G4UIdirectory ( "/analysis/" );
   analysis->SetGuidance ( "Various analysis commands" );

   writeTree = new G4UIcmdWithoutParameter ( "/analysis/writeTree",this );
   writeTree->SetGuidance ( "Write analysis file" );
   writeTree->SetGuidance ( "  angle w.r.t. (k,n) plane" );
//   writeTree->SetParameterName("angle",true);
//   writeTree->SetUnitCategory("Angle");
//   writeTree->SetDefaultValue(-360.0);
//   writeTree->SetDefaultUnit("deg");
   writeTree->AvailableForStates ( G4State_Idle );

   showPlot = new G4UIcmdWithAnInteger ( "/analysis/showPlot",this );
   showPlot->SetGuidance ( " Argument 1 shows histograms and graphs" );
   showPlot->SetGuidance ( " Argument 0 does not show anything" );
   showPlot->AvailableForStates ( G4State_Idle );

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

BETASimulationMessenger::~BETASimulationMessenger()
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void BETASimulationMessenger::SetNewValue (
   G4UIcommand* command, G4String newValue )
{
   if ( command == writeTree )
   {
      G4cout << " T1 " <<  G4endl;

      manager->write();
   }
   if ( command == showPlot )
   {
      manager->showPlot ( showPlot->GetNewIntValue ( newValue ) );
   }
}


