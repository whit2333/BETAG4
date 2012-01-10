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
  simulation = new G4UIdirectory ( "/simulation/" );
  simulation->SetGuidance ( "Simulation Configuration commands" );
//    analysis = new G4UIdirectory ( "/analysis/" );
//    analysis->SetGuidance ( "Various analysis commands" );
// 
    simType = new G4UIcmdWithAString ( "/simulation/setType",this );
    simType->SetGuidance ( " Set the simulation type " );
// //   writeTree->SetParameterName("angle",true);
// //   writeTree->SetUnitCategory("Angle");
// //   writeTree->SetDefaultValue(-360.0);
// //   writeTree->SetDefaultUnit("deg");
//    writeTree->AvailableForStates ( G4State_Idle );

//    showPlot = new G4UIcmdWithAnInteger ( "/analysis/showPlot",this );
//    showPlot->SetGuidance ( " Argument 1 shows histograms and graphs" );
//    showPlot->SetGuidance ( " Argument 0 does not show anything" );
//    showPlot->AvailableForStates ( G4State_Idle );

}
//___________________________________________________________________//

BETASimulationMessenger::~BETASimulationMessenger()
{
}
//___________________________________________________________________//

void BETASimulationMessenger::SetNewValue (
   G4UIcommand* command, G4String newValue )
{
    if ( command == simType )
    {
    /* manager->showPlot ( showPlot->GetNewIntValue ( newValue ) );*/
    }
}
//___________________________________________________________________//

void BETASimulationMessenger::AddDetectorUICommands() {

	 simType->AvailableForStates ( G4State_Idle );

   setBeamEnergy = new G4UIcmdWithADouble("/beta/setBeamEnergy",this);
   setBeamEnergy->SetGuidance(" Set the electron beam energy in units of GeV.");
   setBeamEnergy->SetDefaultValue(5.9);
   setBeamEnergy->AvailableForStates(G4State_Idle);

//    fieldDir = new G4UIdirectory ( "/BETAG4/targetField" );
//    fieldDir->SetGuidance ( "Magnetic Detector control" );
// 
//    polSwitch = new G4UIcmdWithoutParameter ( "/BETAG4/targetField/switchDetectorPolarization",this );
//    polSwitch->SetGuidance ( "Switch between parallel and transverse" );
//    polSwitch->SetGuidance ( "Target magnetic field polarizations" );
//    polSwitch->AvailableForStates ( G4State_Idle );
// 
//    polSet = new G4UIcmdWithADouble ( "/BETAG4/targetField/setPolarizationAngle",this );
//    polSet->SetGuidance ( "Set the target field polariztion" );
//    polSet->SetGuidance ( "angle in units of degrees. For SANE, " );
//    polSet->SetGuidance ( "transverse = 80 and antiparallel = 180" );

//    rotateMirror = new G4UIcmdWithAString ( "/beta/gasCherenkov/mirrors/rotateToroidalMirrors",this );
//    rotateMirror->SetGuidance ( "Arguments: [mirror number] [rotate horz] [rotate  vert]" );
//    rotateMirror->AvailableForStates ( G4State_Idle );


//   rotateToroidalMirrors = new G4UIcmdWithADouble("/field/rotateToroidalMirrors",this);
//   rotateToroidalMirrors->SetGuidance("Rotate all toroidal mirros by given angle (degrees) along horz");
//   rotateToroidalMirrors->AvailableForStates(G4State_Idle);

}
//___________________________________________________________________//

