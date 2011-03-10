#include "BETASimulationManager.hh"
#include <iostream>
#include "G4UIdirectory.hh"
#include "G4UIcmdWithoutParameter.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4Tokenizer.hh"

#include "BETASimulationMessenger.hh"
#include "BETADetectorConstruction.hh"
#include "BETADetectorMessenger.hh"

//___________________________________________________________________//

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
//___________________________________________________________________//

BETASimulationMessenger::~BETASimulationMessenger()
{
}
//___________________________________________________________________//

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
//___________________________________________________________________//

void BETASimulationMessenger::AddDetectorUICommands() {
   fieldDir = new G4UIdirectory ( "/BETAG4/targetField" );
   fieldDir->SetGuidance ( "Magnetic Detector control" );

   polSwitch = new G4UIcmdWithoutParameter ( "/BETAG4/targetField/switchDetectorPolarization",this );
   polSwitch->SetGuidance ( "Switch between parallel and transverse" );
   polSwitch->SetGuidance ( "Target magnetic field polarizations" );
   polSwitch->AvailableForStates ( G4State_Idle );

   polSet = new G4UIcmdWithADouble ( "/BETAG4/targetField/setPolarizationAngle",this );
   polSet->SetGuidance ( "Set the target field polariztion" );
   polSet->SetGuidance ( "angle in units of degrees. For SANE, " );
   polSet->SetGuidance ( "transverse = 80 and antiparallel = 180" );

   rotateMirror = new G4UIcmdWithAString ( "/BETAG4/gasCherenkov/mirrors/rotateToroidalMirrors",this );
   rotateMirror->SetGuidance ( "Arguments: [mirror number] [rotate horz] [rotate  vert]" );
   rotateMirror->AvailableForStates ( G4State_Idle );

   lookAtField = new G4UIcmdWithAString ( "/BETAG4/targetField/lookAtField",this );
   lookAtField->SetGuidance ( "Arguments: [component]" );
   lookAtField->SetGuidance ( "where component can be Z, z, R, or r" );
   lookAtField->SetGuidance ( "Runs root interpreter, so use .q to quit when finished looking at plot" );

//   rotateToroidalMirrors = new G4UIcmdWithADouble("/field/rotateToroidalMirrors",this);
//   rotateToroidalMirrors->SetGuidance("Rotate all toroidal mirros by given angle (degrees) along horz");
//   rotateToroidalMirrors->AvailableForStates(G4State_Idle);

}
//___________________________________________________________________//

