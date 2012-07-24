#include "BETAPrimaryGeneratorMessenger.hh"
#include "BETAPrimaryGeneratorAction.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "TDatabasePDG.h"
#include "TParticlePDG.h"
#include "TList.h"

/// \todo Implement directory with relevent commands based upon  which 
/// Type of PrimaryGenerator is used. 
/// eg BeamOnTarget, ZeroAsymmetryElectrons, ZeroAsymmetryBeamOnTarget etc...

BETAPrimaryGeneratorMessenger::BETAPrimaryGeneratorMessenger (
   BETAPrimaryGeneratorAction* genAction )
      :BETAAction ( genAction )
{
   gunDir = new G4UIdirectory ( "/beta/" );
   gunDir = new G4UIdirectory ( "/beta/gun/" );
   gunDir->SetGuidance ( "PrimaryGenerator control" );

//    polarCmd = new G4UIcmdWithADoubleAndUnit ( "/beta/gun/optPhotonPolar",this );
//    polarCmd->SetGuidance ( "Set linear polarization" );
//    polarCmd->SetGuidance ( "  angle w.r.t. (k,n) plane" );
//    polarCmd->SetParameterName ( "angle",true );
//    polarCmd->SetUnitCategory ( "Angle" );
//    polarCmd->SetDefaultValue ( -360.0 );
//    polarCmd->SetDefaultUnit ( "deg" );
//    polarCmd->AvailableForStates ( G4State_Idle );

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
   setThetaMin->SetGuidance ( " Set the (first) primary particle theta minimum in degrees " );
   setThetaMin->SetDefaultValue ( 35.0 );
   setThetaMin->AvailableForStates ( G4State_Idle );

   setThetaMax = new G4UIcmdWithADouble ( "/beta/gun/setThetaMax",this );
   setThetaMax->SetGuidance ( " Set the (first) primary particle theta maximum in degrees " );
   setThetaMax->SetDefaultValue ( 45.0 );
   setThetaMax->AvailableForStates ( G4State_Idle );

   setPhiMin = new G4UIcmdWithADouble ( "/beta/gun/setPhiMin",this );
   setPhiMin->SetGuidance ( " Set the (first) primary particle phi minimum in degrees " );
   setPhiMin->SetDefaultValue ( -15.0 );
   setPhiMin->AvailableForStates ( G4State_Idle );

   setPhiMax = new G4UIcmdWithADouble ( "/beta/gun/setPhiMax",this );
   setPhiMax->SetGuidance ( " Set the (first) primary particle phi maximum in degrees " );
   setPhiMax->SetDefaultValue ( 15.0 );
   setPhiMax->AvailableForStates ( G4State_Idle );

   setEnergyMin = new G4UIcmdWithADouble ( "/beta/gun/setEnergyMin",this );
   setEnergyMin->SetGuidance ( " Set the (first) primary particle minimum energy in GeV " );
   setEnergyMin->SetDefaultValue ( 1.0 );
   setEnergyMin->AvailableForStates ( G4State_Idle );

   setEnergyMax = new G4UIcmdWithADouble ( "/beta/gun/setEnergyMax",this );
   setEnergyMax->SetGuidance ( " Set the (first) primary particle maximum energy in GeV" );
   setEnergyMax->SetDefaultValue ( 5.7 );
   setEnergyMax->AvailableForStates ( G4State_Idle );

   setMomentumMin = new G4UIcmdWithADouble ( "/beta/gun/setMomentumMin",this );
   setMomentumMin->SetGuidance ( " Set the (first) primary particle minimum momentum in GeV/c " );
   setMomentumMin->SetDefaultValue ( 1.0 );
   setMomentumMin->AvailableForStates ( G4State_Idle );

   setMomentumMax = new G4UIcmdWithADouble ( "/beta/gun/setMomentumMax",this );
   setMomentumMax->SetGuidance ( " Set the (first) primary particle maximum momentum in GeV/c" );
   setMomentumMax->SetDefaultValue ( 5.7 );
   setMomentumMax->AvailableForStates ( G4State_Idle );

   refreshGenerator = new G4UIcmdWithoutParameter("/beta/gun/refresh",this);
   refreshGenerator->SetGuidance ( " Refresh the event generator after modifying settings " );
   refreshGenerator->AvailableForStates ( G4State_Idle );

   setType = new G4UIcmdWithAString("/beta/gun/setEventType",this);
   setType->SetGuidance ( " Set the type of event generated. Note that this resets all the set values to their defaults." );
   setType->SetGuidance ( " Possible arguments are :" );
   setType->SetGuidance ( " flat - uniformly populate events" );
   setType->SetGuidance ( " mott - mott cross section" );
   setType->SetGuidance ( " cone - a small cone near with a small energy range " );
   setType->SetGuidance ( " dis - inclusive electron DIS(F1p and F2p)" );
   setType->SetGuidance ( " beamOnTarget - electron beam shot from up stream on the target (GEANT4 physics)" );
   setType->SetDefaultValue ( "flat" );
   setType->AvailableForStates ( G4State_Idle );


   setPSVariable = new G4UIcmdWithAString ( "/beta/gun/setPSVariable",this );
   setPSVariable->SetGuidance ( " Set a phase space variable to a value"  );
   setPSVariable->SetGuidance ( " Note the units should be in the correct units for the variable"  );
   setPSVariable->SetGuidance ( " "  );
   setPSVariable->AvailableForStates ( G4State_Idle );


   listPSVariables = new G4UIcmdWithoutParameter("/beta/gun/listPSVariables",this);
   listPSVariables->SetGuidance ( " List the phase space variables " );
   listPSVariables->AvailableForStates ( G4State_Idle );


   setParticle = new G4UIcmdWithAString ( "/beta/gun/setParticle",this );
   setParticle->SetGuidance ( "  Set the particle thrown (if event generator is applicable) \n use the simple particle name... e-,e+,pi+,proton,kaon- " );
   setParticle->SetDefaultValue ( "e-" );
   setParticle->AvailableForStates ( G4State_Idle );

   sigmaMomentum = new G4UIcmdWithADouble ( "/beta/gun/sigmaMomentum",this );
   sigmaMomentum->SetGuidance ( " Set relative error of particle momentum" );
   sigmaMomentum->SetDefaultValue ( 0 );
   sigmaMomentum->AvailableForStates ( G4State_Idle );

   sete_piRatio = new G4UIcmdWithADouble ( "/beta/gun/SetElectronPionRatio",this );
   sete_piRatio->SetGuidance ( " Set the electron - pion ratio \nSetting to one is all electrons " );
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

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void BETAPrimaryGeneratorMessenger::SetNewValue (
   G4UIcommand* command, G4String newValue )
{
   BETAG4EventGenerator * anEventGen = BETAAction->GetEventGenerator();

/*   if(anEventGen!) return;*/

   if ( command == setThetaMax ) {
      anEventGen->SetThetaMax(setThetaMax->GetNewDoubleValue(newValue)*TMath::Pi()/180.0);
      anEventGen->NeedsRefreshed();
   }
   if ( command == setPhiMax ) {
      anEventGen->SetPhiMax(setPhiMax->GetNewDoubleValue(newValue)*TMath::Pi()/180.0);
      anEventGen->NeedsRefreshed();
   }
   if ( command == setEnergyMax ) {
      anEventGen->SetEnergyMax(setEnergyMax->GetNewDoubleValue(newValue));
      anEventGen->NeedsRefreshed();
   }
   if ( command == setThetaMin ) {
      anEventGen->SetThetaMin(setThetaMin->GetNewDoubleValue(newValue)*TMath::Pi()/180.0);
      anEventGen->NeedsRefreshed();
   }
   if ( command == setPhiMin ) {
      anEventGen->SetPhiMin(setPhiMin->GetNewDoubleValue(newValue)*TMath::Pi()/180.0);
      anEventGen->NeedsRefreshed();
   }
   if ( command == setEnergyMin) {
      anEventGen->SetEnergyMin(setEnergyMin->GetNewDoubleValue(newValue));
      anEventGen->NeedsRefreshed();
   }
   if ( command == setMomentumMax ) {
      anEventGen->SetMomentumMax(setMomentumMax->GetNewDoubleValue(newValue));
      anEventGen->NeedsRefreshed();
   }
   if ( command == setMomentumMin) {
      anEventGen->SetMomentumMin(setMomentumMin->GetNewDoubleValue(newValue));
      anEventGen->NeedsRefreshed();
   }
   if ( command == setType) {
      if(newValue == "flat") {
         anEventGen = new BigcalCenterEventGenerator();
         BETAAction->SetEventGenerator(anEventGen);
         anEventGen->Initialize();
         anEventGen->fIsInitialized = true;
      } else if(newValue == "cone") {
         anEventGen = new ConeEventGenerator();
         BETAAction->SetEventGenerator(anEventGen);
         anEventGen->Initialize();
         anEventGen->fIsInitialized = true;
      } else if(newValue == "dis") {
         anEventGen = new DISEventGenerator();
         BETAAction->SetEventGenerator(anEventGen);
         anEventGen->Initialize();
         anEventGen->fIsInitialized = true;
      } else if(newValue == "mott") {
         anEventGen = new MottEventGenerator();
         BETAAction->SetEventGenerator(anEventGen);
         anEventGen->Initialize();
         anEventGen->fIsInitialized = true;
      } else if(newValue == "wiser") {
         anEventGen = new WiserEventGenerator();
         BETAAction->SetEventGenerator(anEventGen);
         anEventGen->Initialize();
         anEventGen->fIsInitialized = true;
      } else if(newValue == "electronPion") {
         anEventGen =  new InclusiveElectronPionGenerator();
         BETAAction->SetEventGenerator(anEventGen);
         anEventGen->Initialize();
         anEventGen->fIsInitialized = true;
      } else if(newValue == "beamOnTarget") {
         anEventGen = new BeamOnTargetEventGenerator();
         BETAAction->SetEventGenerator(anEventGen);
         anEventGen->Initialize();
         anEventGen->fIsInitialized = true;
      } else {
         std::cout << " Illegal parameter: " << newValue << " !\n";
      }
      anEventGen->NeedsRefreshed();
   }
   if ( command == refreshGenerator) {
        if( !anEventGen->fIsInitialized ) {
           anEventGen->Initialize();
           anEventGen->fIsInitialized = true;
        }
        else BETAAction->GetEventGenerator()->Refresh();
   }
   if ( command == listPSVariables) {
/*      BETAAction->GetEventGenerator()->ListPhaseSpaceVariables();*/
      BETAAction->GetEventGenerator()->Print();
   }

//    if ( command == polarCmd )
//    {
//       G4double angle = polarCmd->GetNewDoubleValue ( newValue );
//       if ( angle == -360.0*deg )
//       {
//          BETAAction->SetOptPhotonPolar();
//       }
//       else
//       {
//          BETAAction->SetOptPhotonPolar ( angle );
//       }
//    }
//    if ( command == isotropic )
//    {
//       BETAAction->SetIsotropic ( isotropic->GetNewIntValue ( newValue ) );
//    }
// 
//    if ( command == momentum )
//    {
//       BETAAction->SetMomentum ( momentum->GetNewDoubleValue ( newValue ) );
//    }
// 
//    if ( command == sigmaMomentum )
//    {
//       BETAAction->SetSigmaMomentum ( sigmaMomentum->GetNewDoubleValue ( newValue ) );
//    }
//    if ( command == sete_piRatio )
//    {
//       BETAAction->SetElectronPionRatio ( sete_piRatio->GetNewDoubleValue ( newValue ) );
//    }
   if ( command == setParticle )
   {
      G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
      TList * samplers = anEventGen->GetSamplers();
      if(samplers->GetEntries() > 0 ) {
	 int partnum =  TDatabasePDG::Instance()->GetParticle(newValue.data())->PdgCode(); 
         ((InSANEPhaseSpaceSampler*)(samplers->At(0)))->GetXSec()->SetParticleType(partnum );
         ((InSANEPhaseSpaceSampler*)(samplers->At(0)))->GetXSec()->InitializeFinalStateParticles( );
         ((InSANEPhaseSpaceSampler*)(samplers->At(0)))->GetXSec()->Print( );
	 std::cout << " setting particle by pdg code " << partnum <<  " \n";
      }
      else std::cout << " NO SAMPLERS YET\n"; 
      BETAAction->fParticleGun->SetParticleDefinition(particleTable->FindParticle ( newValue ));
      
      anEventGen->NeedsRefreshed();
   }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
