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
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

BETADetectorMessenger::BETADetectorMessenger ( BETADetectorConstruction * detConstruction )
      :construction ( detConstruction )
{
   fieldDir = new G4UIdirectory ( "/targetField" );
   fieldDir->SetGuidance ( "Magnetic Detector control" );

   polSwitch = new G4UIcmdWithoutParameter ( "/targetField/switchDetectorPolarization",this );
   polSwitch->SetGuidance ( "Switch between parallel and transverse" );
   polSwitch->SetGuidance ( "Target magnetic field polarizations" );
   polSwitch->AvailableForStates ( G4State_Idle );

   polSet = new G4UIcmdWithADouble ( "/targetField/setPolarizationAngle",this );
   polSet->SetGuidance ( "Set the target field polariztion" );
   polSet->SetGuidance ( "angle in units of degrees. For SANE, " );
   polSet->SetGuidance ( "transverse = 80 and antiparallel = 180" );

   rotateMirror = new G4UIcmdWithAString ( "/BETA/gasCherenkov/mirrors/rotateToroidalMirrors",this );
   rotateMirror->SetGuidance ( "Arguments: [mirror number] [rotate horz] [rotate  vert]" );
   rotateMirror->AvailableForStates ( G4State_Idle );

   lookAtField = new G4UIcmdWithAString ( "/targetField/lookAtField",this );
   lookAtField->SetGuidance ( "Arguments: [component]" );
   lookAtField->SetGuidance ( "where component can be Z, z, R, or r" );
   lookAtField->SetGuidance ( "Runs root interpreter, so use .q to quit when finished looking at plot" );

//   rotateToroidalMirrors = new G4UIcmdWithADouble("/field/rotateToroidalMirrors",this);
//   rotateToroidalMirrors->SetGuidance("Rotate all toroidal mirros by given angle (degrees) along horz");
//   rotateToroidalMirrors->AvailableForStates(G4State_Idle);

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

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
}
*/
   if ( command == polSwitch )
   {
      G4cout << " Switching target field " <<  G4endl;
      construction->switchTargetField();
   }
   if ( command == polSet )
   {
      construction->setTargetAngle ( ( polSet->GetNewDoubleValue ( newValue ) ) *pi/180. );
   }
   if ( command == rotateMirror )
   {
      G4Tokenizer next ( newValue );
      int numb = StoI ( next() );
      G4double alph = StoD ( next() ) *pi/180.;
      G4double bet = StoD ( next() ) *pi/180.;
      construction->rotateMirror ( numb,alph,bet );
   }
   if ( command == lookAtField )
   {
      //G4Tokenizer next ( newValue );
      G4String component = newValue;
      construction->lookAtField(component);
   }

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

