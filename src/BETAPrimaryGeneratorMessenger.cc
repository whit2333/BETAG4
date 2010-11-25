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
   gunDir = new G4UIdirectory ( "/BETAG4/gun/" );
   gunDir->SetGuidance ( "PrimaryGenerator control" );

   polarCmd = new G4UIcmdWithADoubleAndUnit ( "/BETAG4/gun/optPhotonPolar",this );
   polarCmd->SetGuidance ( "Set linear polarization" );
   polarCmd->SetGuidance ( "  angle w.r.t. (k,n) plane" );
   polarCmd->SetParameterName ( "angle",true );
   polarCmd->SetUnitCategory ( "Angle" );
   polarCmd->SetDefaultValue ( -360.0 );
   polarCmd->SetDefaultUnit ( "deg" );
   polarCmd->AvailableForStates ( G4State_Idle );

   isotropic = new G4UIcmdWithAnInteger ( "/BETAG4/gun/isotropic",this );
   isotropic->SetGuidance ( " Random isotropic direction  " );
   isotropic->SetGuidance ( " Set to non-zero to turn on. " );
   isotropic->SetDefaultValue ( 0 );
   isotropic->AvailableForStates ( G4State_Idle );

   momentum = new G4UIcmdWithADouble ( "/BETAG4/gun/momentum",this );
   momentum->SetGuidance ( " Set momentum of particle " );
   momentum->SetGuidance ( " " );
   momentum->SetDefaultValue ( 0 );
   momentum->AvailableForStates ( G4State_Idle );

   setParticleTheta = new G4UIcmdWithADouble ( "/BETAG4/gun/setParticleTheta",this );
   setParticleTheta->SetGuidance ( " Set primary particle theta - degrees " );
   setParticleTheta->SetDefaultValue ( 40.0 );
   setParticleTheta->AvailableForStates ( G4State_Idle );

   setParticlePhi = new G4UIcmdWithADouble ( "/BETAG4/gun/setParticlePhi",this );
   setParticlePhi->SetGuidance ( " Set primary particle phi - degrees " );
   setParticlePhi->SetDefaultValue ( 0.0 );
   setParticlePhi->AvailableForStates ( G4State_Idle );


   sigmaMomentum = new G4UIcmdWithADouble ( "/BETAG4/gun/sigmaMomentum",this );
   sigmaMomentum->SetGuidance ( " Set relative error of particle momentum" );
   sigmaMomentum->SetDefaultValue ( 0 );
   sigmaMomentum->AvailableForStates ( G4State_Idle );

   sete_piRatio = new G4UIcmdWithADouble ( "/BETAG4/gun/SetElectronPionRatio",this );
   sete_piRatio->SetGuidance ( " Set the electron - pion ratio " );
   sete_piRatio->SetGuidance ( " Setting to one is all electrons " );
   sete_piRatio->SetDefaultValue ( 1.0/100.0 );
   sete_piRatio->AvailableForStates ( G4State_Idle );

   setpi0Ratio = new G4UIcmdWithADouble ( "/BETAG4/gun/SetPiZeroRatio",this );
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

   if ( command == setParticleTheta )
   {
      BETAAction->SetPartTheta ( setParticleTheta->GetNewDoubleValue ( newValue ) );
   }

   if ( command == setParticlePhi )
   {
      BETAAction->SetPartPhi ( setParticlePhi->GetNewDoubleValue ( newValue ) );
   }

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
