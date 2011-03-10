#ifdef G4FIELD_USE
#include "BETADetectorConstruction.hh"
#endif // G4FIELD_USE
#include "BETADetectorMessenger.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithoutParameter.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4UIdirectory.hh"
#include <iostream>
#include "G4Tokenizer.hh"

BETADetectorMessenger::BETADetectorMessenger ( BETADetectorConstruction * detConstruction )
      :construction ( detConstruction )
{
  fTargetAngle=TMath::Pi();

   fieldDir = new G4UIdirectory ( "/beta/target" );
   fieldDir->SetGuidance ( "Polarized target controls" );

   polSwitch = new G4UIcmdWithoutParameter ( "/beta/target/switchTargetPolarization",this );
   polSwitch->SetGuidance ( "Switch between parallel and transverse" );
   polSwitch->SetGuidance ( "Target magnetic field polarizations" );
   polSwitch->AvailableForStates ( G4State_Idle );

   polSet = new G4UIcmdWithADouble ( "/beta/target/setPolarizationAngle",this );
   polSet->SetGuidance ( "Set the target field polariztion" );
   polSet->SetGuidance ( "angle in units of degrees. For SANE, " );
   polSet->SetGuidance ( "transverse = 80 and antiparallel = 180" );

   rotateMirror = new G4UIcmdWithAString ( "/beta/gasCherenkov/mirrors/rotateToroidalMirrors",this );
   rotateMirror->SetGuidance ( "Arguments: [mirror number] [rotate horz] [rotate  vert]" );
   rotateMirror->AvailableForStates ( G4State_Idle );

   lookAtField = new G4UIcmdWithAString ( "/beta/target/lookAtField",this );
   lookAtField->SetGuidance ( "Arguments: [component]" );
   lookAtField->SetGuidance ( "where component can be Z, z, R, or r" );
   lookAtField->SetGuidance ( "Runs root interpreter, so use .q to quit when finished looking at plot" );
   lookAtField->AvailableForStates ( G4State_Idle );

   toggleTargetField = new G4UIcmdWithAString ( "/beta/target/toggleTargetField",this );
   toggleTargetField->SetGuidance ( "Arguments: [on, off]" );
   toggleTargetField->SetGuidance ( "Turns off the magnetic field of the oxford magnet" );
   toggleTargetField->SetGuidance ( "Default is on" );
   toggleTargetField->AvailableForStates ( G4State_Idle );

   toggleTargetMaterial = new G4UIcmdWithAString ( "/beta/target/toggleTargetMaterial",this );
   toggleTargetMaterial->SetGuidance ( "Arguments: [on, off, ovcOnly, magnetOnly]" );
   toggleTargetMaterial->SetGuidance ( "Turns off or deletes the geometry and material of the target" );
   toggleTargetMaterial->SetGuidance ( "Note: this does not turn off the field! " );
   toggleTargetMaterial->AvailableForStates ( G4State_Idle );
//   rotateToroidalMirrors = new G4UIcmdWithADouble("/field/rotateToroidalMirrors",this);
//   rotateToroidalMirrors->SetGuidance("Rotate all toroidal mirros by given angle (degrees) along horz");
//   rotateToroidalMirrors->AvailableForStates(G4State_Idle);

}


BETADetectorMessenger::~BETADetectorMessenger()
{
}
//_________________________________________________________________//

void BETADetectorMessenger::SetNewValue (
  G4UIcommand* command, G4String newValue )
{
/*
  switch ( command ) {
    case polSwitch : 
    // Process for test = 1
    ...
    break;

  case 5 : 
    // Process for test = 5
    ...
    break;

  default : 
    // Process for all other cases.
    ...
e
}*/
   if ( command == polSwitch )
   {
      G4cout << " Switching target field " <<  G4endl;
      construction->switchTargetField();
   G4RunManager::GetRunManager()->GeometryHasBeenModified();

   }
   if ( command == polSet )
   {
      construction->setTargetAngle ( ( polSet->GetNewDoubleValue ( newValue ) ) *TMath::Pi()/180. );
   G4RunManager::GetRunManager()->GeometryHasBeenModified();

   }
   if ( command == rotateMirror )
   {
      G4Tokenizer next ( newValue );
      int numb = StoI ( next() );
      G4double alph = StoD ( next() ) *pi/180.;
      G4double bet = StoD ( next() ) *pi/180.;
      construction->rotateMirror ( numb,alph,bet );
      G4RunManager::GetRunManager()->GeometryHasBeenModified();
   }
   if ( command == lookAtField )
   {
      //G4Tokenizer next ( newValue );
      G4String component = newValue;
      construction->lookAtField(component);
   G4RunManager::GetRunManager()->GeometryHasBeenModified();

   }
   if ( command == toggleTargetField )
   {
      G4String arg = newValue;

      if(arg == "on") construction->ToggleTargetField(1);
      else if(arg == "off") construction->ToggleTargetField(0);
      else  std::cout << "Unknown option" << arg << "\n"; 

   G4RunManager::GetRunManager()->GeometryHasBeenModified();

   }
   if ( command == toggleTargetMaterial )
   {
      G4String arg = newValue;
      if(arg == "on") construction->ToggleTargetMaterial(1);
      else if(arg == "off") construction->ToggleTargetMaterial(0);
      else  std::cout << "Unknown option" << arg << "\n"; 

   G4RunManager::GetRunManager()->GeometryHasBeenModified();

   }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

