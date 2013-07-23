#include "BETASimulationManager.hh"
#include <iostream>
#include "G4UIdirectory.hh"
#include "G4UIcmdWithoutParameter.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4Tokenizer.hh"

#include "BETASimulationMessenger.hh"
#include "BETADetectorConstruction.hh"

//___________________________________________________________________
BETASimulationMessenger::BETASimulationMessenger ( BETASimulationManager* mgr )
      :fSimManager ( mgr )
{

   //--- Directories
   fDir_simulation = new G4UIdirectory ( "/simulation/" );
   fDir_simulation->SetGuidance ( "Simulation Configuration commands" );

   //fDir_simType = new G4UIcmdWithAString ( "/simulation/setType",this );
   //fDir_simType->SetGuidance ( " Set the simulation type " );

   fDir_beta = new G4UIdirectory ( "/beta/" );
   fDir_beta->SetGuidance ( "SANE and BETA related commands" );

   fDir_fieldDir = new G4UIdirectory ( "/beta/target/" );
   fDir_fieldDir->SetGuidance ( "Polarized target controls" );

   //--- commands
   fCmd_polSwitch = new G4UIcmdWithoutParameter ( "/beta/target/switchTargetPolarization",this );
   fCmd_polSwitch->SetGuidance ( "Switch between parallel and transverse" );
   fCmd_polSwitch->SetGuidance ( "Target magnetic field polarizations" );
   fCmd_polSwitch->AvailableForStates ( G4State_Idle );

   fCmd_polSet = new G4UIcmdWithADouble ( "/beta/target/setPolarizationAngle",this );
   fCmd_polSet->SetGuidance ( "Set the target field polariztion" );
   fCmd_polSet->SetGuidance ( "angle in units of degrees. For SANE, " );
   fCmd_polSet->SetGuidance ( "transverse = 80 and antiparallel = 180" );
   fCmd_polSet->SetDefaultValue ( fSimManager->GetTargetAngle() );
   fCmd_polSet->AvailableForStates ( G4State_Idle );

   fCmd_rotateMirror = new G4UIcmdWithAString ( "/beta/gasCherenkov/mirrors/rotateToroidalMirrors",this );
   fCmd_rotateMirror->SetGuidance ( "Arguments: [mirror number] [rotate horz] [rotate  vert]" );
   fCmd_rotateMirror->AvailableForStates ( G4State_Idle );

   fCmd_lookAtField = new G4UIcmdWithAString ( "/beta/target/lookAtField",this );
   fCmd_lookAtField->SetGuidance ( "Displays current magnetic field.  " );
   //fCmd_lookAtField->SetGuidance ( "Arguments: [component]" );
   //fCmd_lookAtField->SetGuidance ( "where component can be Z, z, R, or r" );
   fCmd_lookAtField->SetGuidance ( "Runs root interpreter, so use .q to quit when finished looking at plot" );
   fCmd_lookAtField->AvailableForStates ( G4State_Idle );

   fCmd_toggleTargetField = new G4UIcmdWithAString ( "/beta/target/toggleTargetField",this );
   fCmd_toggleTargetField->SetGuidance ( "Arguments: [on, off]" );
   fCmd_toggleTargetField->SetGuidance ( "Turns off the magnetic field of the oxford magnet" );
   fCmd_toggleTargetField->SetGuidance ( "Default is on" );
   fCmd_toggleTargetField->AvailableForStates ( G4State_Idle );

   fCmd_toggleTargetMaterial = new G4UIcmdWithAString ( "/beta/target/toggleTargetMaterial",this );
   fCmd_toggleTargetMaterial->SetGuidance ( "Arguments: [on, off, ovcOnly, magnetOnly]" );
   fCmd_toggleTargetMaterial->SetGuidance ( "Turns off or deletes the geometry and material of the target" );
   fCmd_toggleTargetMaterial->SetGuidance ( "Note: this does not turn off the field! " );
   fCmd_toggleTargetMaterial->AvailableForStates ( G4State_Idle );

   fCmd_toggleForwardTracker = new G4UIcmdWithAString ( "/beta/toggleForwardTracker",this );
   fCmd_toggleForwardTracker->SetGuidance ( "Turn on and off the simulation of the forward tracker" );
   fCmd_toggleForwardTracker->SetGuidance ( "Arguments can be either on, off, or noOptics" );
   fCmd_toggleForwardTracker->AvailableForStates ( G4State_Idle );

   fCmd_toggleGasCherenkov = new G4UIcmdWithAString ( "/beta/toggleGasCherenkov",this );
   fCmd_toggleGasCherenkov->SetGuidance ( "Turn on and off the simulation of the gas cherenkov" );
   fCmd_toggleGasCherenkov->SetGuidance ( "Arguments can be either on, off, or noOptics" );
   fCmd_toggleGasCherenkov->AvailableForStates ( G4State_Idle );

   fCmd_toggleLuciteHodoscope = new G4UIcmdWithAString ( "/beta/toggleLuciteHodoscope",this );
   fCmd_toggleLuciteHodoscope->SetGuidance ( "Turn on and off the simulation of the lucite hodoscope" );
   fCmd_toggleLuciteHodoscope->SetGuidance ( "Arguments can be either on, off, or noOptics" );
   fCmd_toggleLuciteHodoscope->AvailableForStates ( G4State_Idle );

   fCmd_toggleBigcal = new G4UIcmdWithAString ( "/beta/toggleBigcal",this );
   fCmd_toggleBigcal->SetGuidance ( "Turn on and off the simulation of BigCal" );
   fCmd_toggleBigcal->SetGuidance ( "Arguments can be either on, off, or noOptics" );
   fCmd_toggleBigcal->AvailableForStates ( G4State_Idle );

   //rotateToroidalMirrors = new G4UIcmdWithADouble("/field/rotateToroidalMirrors",this);
   //rotateToroidalMirrors->SetGuidance("Rotate all toroidal mirros by given angle (degrees) along horz");
   //rotateToroidalMirrors->AvailableForStates(G4State_Idle);

}
//___________________________________________________________________

BETASimulationMessenger::~BETASimulationMessenger()
{
}
//___________________________________________________________________//

void BETASimulationMessenger::SetNewValue ( G4UIcommand* command, G4String newValue )
{
   if ( command == fCmd_simType ) {

   }
   if ( command == fCmd_polSwitch )
   {
      G4cout << " Switching target field " <<  G4endl;
      fDetConstruction->SwitchTargetField();
      G4RunManager::GetRunManager()->GeometryHasBeenModified();

   }
   if ( command == fCmd_polSet )
   {
      fDetConstruction->SetTargetAngle( ( fCmd_polSet->GetNewDoubleValue ( newValue ) ) *TMath::Pi()/180. );
      G4RunManager::GetRunManager()->GeometryHasBeenModified();

   }
   if ( command == fCmd_rotateMirror )
   {
      G4Tokenizer next ( newValue );
      int numb = StoI ( next() );
      G4double alph = StoD ( next() ) *pi/180.;
      G4double bet = StoD ( next() ) *pi/180.;
      fDetConstruction->rotateMirror ( numb,alph,bet );
      G4RunManager::GetRunManager()->GeometryHasBeenModified();
   }
   if ( command == fCmd_lookAtField )
   {
      //G4Tokenizer next ( newValue );
      G4String component = "r";//newValue;
      fDetConstruction->lookAtField(component);
      G4RunManager::GetRunManager()->GeometryHasBeenModified();

   }
   if ( command == fCmd_toggleTargetField )
   {
      G4String arg = newValue;

      if(arg == "on") fDetConstruction->ToggleTargetField(1);
      else if(arg == "off") fDetConstruction->ToggleTargetField(0);
      else  std::cout << "Unknown option" << arg << "\n"; 

   G4RunManager::GetRunManager()->GeometryHasBeenModified();

   }
   if ( command == fCmd_toggleTargetMaterial )
   {
      G4String arg = newValue;
      if(arg == "on") fDetConstruction->ToggleTargetMaterial(1);
      else if(arg == "off") fDetConstruction->ToggleTargetMaterial(0);
      else  std::cout << "Unknown option" << arg << "\n"; 
   G4RunManager::GetRunManager()->GeometryHasBeenModified();
   }

   if ( command == fCmd_toggleLuciteHodoscope )
   {
      std::cout << " args = " << newValue.data() << "\n";
      if( !(strcmp(newValue.data(),"on")) )  {
         fDetConstruction->usingLuciteHodoscope = true;
         fDetConstruction->fSimulationManager->fSimulateHodoscopeOptics = true;
         if(fDetConstruction->constructed == true) fDetConstruction->ConstructHodoscope();
      }
      if( !(strcmp(newValue.data(),"off"))) {
         fDetConstruction->usingLuciteHodoscope = false;
         fDetConstruction->fSimulationManager->fSimulateHodoscopeOptics = false;
         delete fDetConstruction->hodoscopeContainerBox_phys;
         fDetConstruction->hodoscopeContainerBox_phys = 0;
      }
      if( !(strcmp(newValue.data(),"noOptics"))) {
         fDetConstruction->usingLuciteHodoscope = true;
         fDetConstruction->fSimulationManager->fSimulateHodoscopeOptics = false;
         if(fDetConstruction->constructed == true) fDetConstruction->ConstructHodoscope();

      }
         G4RunManager::GetRunManager()->GeometryHasBeenModified();
   }
   if ( command == fCmd_toggleForwardTracker )
   {
      std::cout << " args = " << newValue.data() << "\n";
      if( !(strcmp(newValue.data(),"on")) )  {
         fDetConstruction->usingForwardTracker = true;
         fDetConstruction->fSimulationManager->fSimulateTrackerOptics = true;
         if(fDetConstruction->tracker_phys) delete fDetConstruction->tracker_phys;
         fDetConstruction->tracker_phys = 0;
         if(fDetConstruction->constructed == true) fDetConstruction->ConstructForwardTracker();
      }
      if( !(strcmp(newValue.data(),"off"))) {
         fDetConstruction->usingForwardTracker = false;
         fDetConstruction->fSimulationManager->fSimulateTrackerOptics = false;
         if(fDetConstruction->tracker_phys) delete fDetConstruction->tracker_phys;
         fDetConstruction->tracker_phys = 0;
      }
      if( !(strcmp(newValue.data(),"noOptics"))) {
         fDetConstruction->usingForwardTracker = true;
         fDetConstruction->fSimulationManager->fSimulateTrackerOptics = false;
         if(fDetConstruction->tracker_phys) delete fDetConstruction->tracker_phys;
         fDetConstruction->tracker_phys = 0;
         if(fDetConstruction->constructed == true) fDetConstruction->ConstructForwardTracker();

/*         if(construction->hodoscopeContainerBox_log)  delete construction->hodoscopeContainerBox_log;*/
/*         if(constructi*/
      }
         G4RunManager::GetRunManager()->GeometryHasBeenModified();
   }

}
//___________________________________________________________________//

void BETASimulationMessenger::AddDetectorUICommands() {

	 //simType->AvailableForStates ( G4State_Idle );
/*
   setBeamEnergy = new G4UIcmdWithADouble("/beta/setBeamEnergy",this);
   setBeamEnergy->SetGuidance(" Set the electron beam energy in units of GeV.");
   setBeamEnergy->SetDefaultValue(5.9);
   setBeamEnergy->AvailableForStates(G4State_Idle);*/

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

