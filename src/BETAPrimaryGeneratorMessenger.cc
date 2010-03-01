//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
//
// $Id: BETAPrimaryGeneratorMessenger.cc,v 1.3 2006/06/29 17:54:29 gunter Exp $
// GEANT4 tag $Name: geant4-08-01 $
//
//

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "BETAPrimaryGeneratorMessenger.hh"
#include "BETAPrimaryGeneratorAction.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithAnInteger.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

BETAPrimaryGeneratorMessenger::BETAPrimaryGeneratorMessenger (
   BETAPrimaryGeneratorAction* BETAGun )
      :BETAAction ( BETAGun )
{
   gunDir = new G4UIdirectory ( "/BETA/gun/" );
   gunDir->SetGuidance ( "PrimaryGenerator control" );

   polarCmd = new G4UIcmdWithADoubleAndUnit ( "/BETA/gun/optPhotonPolar",this );
   polarCmd->SetGuidance ( "Set linear polarization" );
   polarCmd->SetGuidance ( "  angle w.r.t. (k,n) plane" );
   polarCmd->SetParameterName ( "angle",true );
   polarCmd->SetUnitCategory ( "Angle" );
   polarCmd->SetDefaultValue ( -360.0 );
   polarCmd->SetDefaultUnit ( "deg" );
   polarCmd->AvailableForStates ( G4State_Idle );

   isotropic = new G4UIcmdWithAnInteger ( "/BETA/gun/isotropic",this );
   isotropic->SetGuidance ( " Random isotropic direction  " );
   isotropic->SetGuidance ( " Set to non-zero to turn on. " );
   isotropic->SetDefaultValue ( 0 );
   isotropic->AvailableForStates ( G4State_Idle );

   momentum = new G4UIcmdWithADouble ( "/BETA/gun/momentum",this );
   momentum->SetGuidance ( " Set momentum of particle " );
   momentum->SetGuidance ( " " );
   momentum->SetDefaultValue ( 0 );
   momentum->AvailableForStates ( G4State_Idle );

   setParticleTheta = new G4UIcmdWithADouble ( "/BETA/gun/setParticleTheta",this );
   setParticleTheta->SetGuidance ( " Set primary particle theta - degrees " );
   setParticleTheta->SetDefaultValue ( 40.0 );
   setParticleTheta->AvailableForStates ( G4State_Idle );

   setParticlePhi = new G4UIcmdWithADouble ( "/BETA/gun/setParticlePhi",this );
   setParticlePhi->SetGuidance ( " Set primary particle phi - degrees " );
   setParticlePhi->SetDefaultValue ( 0.0 );
   setParticlePhi->AvailableForStates ( G4State_Idle );


   sigmaMomentum = new G4UIcmdWithADouble ( "/BETA/gun/sigmaMomentum",this );
   sigmaMomentum->SetGuidance ( " Set relative error of particle momentum" );
   sigmaMomentum->SetDefaultValue ( 0 );
   sigmaMomentum->AvailableForStates ( G4State_Idle );

   sete_piRatio = new G4UIcmdWithADouble ( "/BETA/gun/SetElectronPionRatio",this );
   sete_piRatio->SetGuidance ( " Set the electron - pion ratio " );
   sete_piRatio->SetGuidance ( " Setting to one is all electrons " );
   sete_piRatio->SetDefaultValue ( 1.0/100.0 );
   sete_piRatio->AvailableForStates ( G4State_Idle );

   setpi0Ratio = new G4UIcmdWithADouble ( "/BETA/gun/SetPiZeroRatio",this );
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
      BETAAction->setPartTheta ( setParticleTheta->GetNewDoubleValue ( newValue ) );
   }

   if ( command == setParticlePhi )
   {
      BETAAction->setPartPhi ( setParticlePhi->GetNewDoubleValue ( newValue ) );
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
      BETAAction->setIsotropic ( isotropic->GetNewIntValue ( newValue ) );
   }

   if ( command == momentum )
   {
      BETAAction->setMomentum ( momentum->GetNewDoubleValue ( newValue ) );
   }

   if ( command == sigmaMomentum )
   {
      BETAAction->setSigmaMomentum ( sigmaMomentum->GetNewDoubleValue ( newValue ) );
   }
   if ( command == sete_piRatio )
   {
      BETAAction->setElectronPionRatio ( sete_piRatio->GetNewDoubleValue ( newValue ) );
   }
   if ( command == setpi0Ratio )
   {
      BETAAction->setPiZeroRatio ( setpi0Ratio->GetNewDoubleValue ( newValue ) );
   }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
