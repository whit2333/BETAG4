#include "BETAPrimaryGeneratorMessenger.hh"
#include "BETAPrimaryGeneratorAction.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithAnInteger.hh"

/// \todo Implement directory with relevent commands based upon  which 
/// Type of PrimaryGenerator is used. 
/// eg BeamOnTarget, ZeroAsymmetryElectrons, ZeroAsymmetryBeamOnTarget etc...

BETAPrimaryGeneratorMessenger::BETAPrimaryGeneratorMessenger (
   BETAPrimaryGeneratorAction* BETAGun )
      :BETAAction ( BETAGun )
{
   gunDir = new G4UIdirectory ( "/beta/gun/" );
   gunDir->SetGuidance ( "PrimaryGenerator control" );

   polarCmd = new G4UIcmdWithADoubleAndUnit ( "/beta/gun/optPhotonPolar",this );
   polarCmd->SetGuidance ( "Set linear polarization" );
   polarCmd->SetGuidance ( "  angle w.r.t. (k,n) plane" );
   polarCmd->SetParameterName ( "angle",true );
   polarCmd->SetUnitCategory ( "Angle" );
   polarCmd->SetDefaultValue ( -360.0 );
   polarCmd->SetDefaultUnit ( "deg" );
   polarCmd->AvailableForStates ( G4State_Idle );

   isotropic = new G4UIcmdWithAnInteger ( "/beta/gun/isotropic",this );
   isotropic->SetGuidance ( " Random isotropic direction  " );
   isotropic->SetGuidance ( " Set to non-zero to turn on. " );
   isotropic->SetDefaultValue ( 0 );
   isotropic->AvailableForStates ( G4State_Idle );

   momentum = new G4UIcmdWithADouble ( "/beta/gun/momentum",this );
   momentum->SetGuidance ( " Set momentum of particle " );
   momentum->SetGuidance ( " " );
   momentum->SetDefaultValue ( 0 );
   momentum->AvailableForStates ( G4State_Idle );

   setThetaMin = new G4UIcmdWithADouble ( "/beta/gun/setThetaMin",this );
   setThetaMin->SetGuidance ( " Set primary particle theta - degrees " );
   setThetaMin->SetDefaultValue ( 35.0 );
   setThetaMin->AvailableForStates ( G4State_Idle );

   setThetaMax = new G4UIcmdWithADouble ( "/beta/gun/setThetaMax",this );
   setThetaMax->SetGuidance ( " Set primary particle theta - degrees " );
   setThetaMax->SetDefaultValue ( 45.0 );
   setThetaMax->AvailableForStates ( G4State_Idle );

   setPhiMin = new G4UIcmdWithADouble ( "/beta/gun/setPhiMin",this );
   setPhiMin->SetGuidance ( " Set primary particle phi - degrees " );
   setPhiMin->SetDefaultValue ( -15.0 );
   setPhiMin->AvailableForStates ( G4State_Idle );

   setPhiMax = new G4UIcmdWithADouble ( "/beta/gun/setPhiMax",this );
   setPhiMax->SetGuidance ( " Set primary particle phi - degrees " );
   setPhiMax->SetDefaultValue ( 15.0 );
   setPhiMax->AvailableForStates ( G4State_Idle );

   setEnergyMax = new G4UIcmdWithADouble ( "/beta/gun/setEnergyMax",this );
   setEnergyMax->SetGuidance ( " Set primary particle phi - degrees " );
   setEnergyMax->SetDefaultValue ( 5.7 );
   setEnergyMax->AvailableForStates ( G4State_Idle );

   setEnergyMin = new G4UIcmdWithADouble ( "/beta/gun/setEnergyMin",this );
   setEnergyMin->SetGuidance ( " Set primary particle phi - degrees " );
   setEnergyMin->SetDefaultValue ( 1.0 );
   setEnergyMin->AvailableForStates ( G4State_Idle );



   sigmaMomentum = new G4UIcmdWithADouble ( "/beta/gun/sigmaMomentum",this );
   sigmaMomentum->SetGuidance ( " Set relative error of particle momentum" );
   sigmaMomentum->SetDefaultValue ( 0 );
   sigmaMomentum->AvailableForStates ( G4State_Idle );

   sete_piRatio = new G4UIcmdWithADouble ( "/beta/gun/SetElectronPionRatio",this );
   sete_piRatio->SetGuidance ( " Set the electron - pion ratio " );
   sete_piRatio->SetGuidance ( " Setting to one is all electrons " );
   sete_piRatio->SetDefaultValue ( 1.0/100.0 );
   sete_piRatio->AvailableForStates ( G4State_Idle );

   setpi0Ratio = new G4UIcmdWithADouble ( "/beta/gun/SetPiZeroRatio",this );
   setpi0Ratio->SetGuidance ( "  Set the pi0 - charged pion ratio \n Setting >= 1 makes all pions neutral " );
   setpi0Ratio->SetDefaultValue ( 0.0 );
   setpi0Ratio->AvailableForStates ( G4State_Idle );
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

BETAPrimaryGeneratorMessenger::~BETAPrimaryGeneratorMessenger()
{
   delete polarCmd;
   delete gunDir;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void BETAPrimaryGeneratorMessenger::SetNewValue (
   G4UIcommand* command, G4String newValue )
{

   if ( command == setThetaMax ) BETAAction->fBigcalEventGen->fThetaMax = (setThetaMax->GetNewDoubleValue(newValue));
   if ( command == setPhiMax ) BETAAction->fBigcalEventGen->fPhiMax = (setPhiMax->GetNewDoubleValue(newValue));
   if ( command == setEnergyMax ) BETAAction->fBigcalEventGen->fEnergyMax = (setEnergyMax->GetNewDoubleValue(newValue));
   if ( command == setThetaMin ) BETAAction->fBigcalEventGen->fThetaMin = (setThetaMin->GetNewDoubleValue(newValue));
   if ( command == setPhiMin ) BETAAction->fBigcalEventGen->fPhiMin = (setPhiMin->GetNewDoubleValue(newValue));
   if ( command == setEnergyMin) BETAAction->fBigcalEventGen->fEnergyMin = (setEnergyMin->GetNewDoubleValue(newValue));

   if ( command == polarCmd )
   {
      G4double angle = polarCmd->GetNewDoubleValue ( newValue );
      if ( angle == -360.0*deg )
      {
         BETAAction->SetOptPhotonPolar();
      }
      else
      {
         BETAAction->SetOptPhotonPolar ( angle );
      }
   }
   if ( command == isotropic )
   {
      BETAAction->SetIsotropic ( isotropic->GetNewIntValue ( newValue ) );
   }

   if ( command == momentum )
   {
      BETAAction->SetMomentum ( momentum->GetNewDoubleValue ( newValue ) );
   }

   if ( command == sigmaMomentum )
   {
      BETAAction->SetSigmaMomentum ( sigmaMomentum->GetNewDoubleValue ( newValue ) );
   }
   if ( command == sete_piRatio )
   {
      BETAAction->SetElectronPionRatio ( sete_piRatio->GetNewDoubleValue ( newValue ) );
   }
   if ( command == setpi0Ratio )
   {
      BETAAction->SetPiZeroRatio ( setpi0Ratio->GetNewDoubleValue ( newValue ) );
   }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
