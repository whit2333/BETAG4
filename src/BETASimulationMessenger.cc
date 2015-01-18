#include "BETASimulationMessenger.hh"
#include <iostream>
#include "G4UIdirectory.hh"
#include "G4UIcmdWithoutParameter.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4Tokenizer.hh"
#include "G4UImanager.hh"


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
   fCmd_polSet->SetGuidance ( "Set the target field polariztion in the x-z plane" );
   fCmd_polSet->SetGuidance ( "Angle in units of degrees." );
   fCmd_polSet->SetGuidance ( "For SANE, transverse = 80 and antiparallel = 180" );
   fCmd_polSet->SetParameterName ( "polangle",false);
   fCmd_polSet->SetRange ( "polangle>=0. && polangle<=180.");
   fCmd_polSet->SetDefaultValue ( fSimManager->GetTargetAngle() );
   fCmd_polSet->AvailableForStates ( G4State_Idle );

   fCmd_simTrigger = new G4UIcmdWithAnInteger ( "/beta/simulateTrigger",this );
   fCmd_simTrigger->SetGuidance ( "Set to 1 in order to simulate the trigger for SANE.");
   fCmd_simTrigger->SetGuidance ( "Otherwise readout occurs for every event generated." );
   fCmd_simTrigger->SetGuidance ( "See BETARun::RecordEvent, BETADigitizer and BETAG4DAQReadout for details." );
   fCmd_simTrigger->SetDefaultValue(1);
   fCmd_simTrigger->AvailableForStates ( G4State_Idle );

   fCmd_rotateMirror = new G4UIcmdWithAString ( "/beta/gasCherenkov/mirrors/rotateToroidalMirrors",this );
   fCmd_rotateMirror->SetGuidance ( "Arguments: [mirror number] [rotate horz] [rotate  vert]" );
   fCmd_rotateMirror->AvailableForStates ( G4State_Idle );

   fCmd_lookAtField = new G4UIcmdWithAString ( "/beta/target/lookAtField",this );
   fCmd_lookAtField->SetGuidance ( "Displays current magnetic field.  " );
   fCmd_lookAtField->SetGuidance ( "Arguments: [component]" );
   fCmd_lookAtField->SetGuidance ( "where component can be Z, z, R, or r" );
   fCmd_lookAtField->SetGuidance ( "Runs root interpreter, so use .q to quit when finished looking at plot" );
   fCmd_lookAtField->AvailableForStates ( G4State_Idle );

   fCmd_toggleTargetField = new G4UIcmdWithAString ( "/beta/target/toggleTargetField",this );
   fCmd_toggleTargetField->SetGuidance ( "Arguments: [on, off]" );
   fCmd_toggleTargetField->SetGuidance ( "Turns off the magnetic field of the oxford magnet" );
   fCmd_toggleTargetField->SetGuidance ( "Default is on" );
   fCmd_toggleTargetField->SetDefaultValue ( "on" );
   fCmd_toggleTargetField->AvailableForStates ( G4State_Idle );

   fCmd_toggleTargetMaterial = new G4UIcmdWithAString ( "/beta/target/toggleTargetMaterial",this );
   fCmd_toggleTargetMaterial->SetGuidance ( "Arguments: [on, off, ovcOnly, magnetOnly]" );
   fCmd_toggleTargetMaterial->SetGuidance ( "Turns off or deletes the geometry and material of the target" );
   fCmd_toggleTargetMaterial->SetGuidance ( "Note: this does not turn off the field! " );
   fCmd_toggleTargetMaterial->SetDefaultValue ( "on" );
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

   //polarCmd->AvailableForStates ( G4State_Idle );

   fCmd_isotropic = new G4UIcmdWithAnInteger ( "/beta/gun/isotropic",this );
   fCmd_isotropic->SetGuidance ( " Random isotropic direction  " );
   fCmd_isotropic->SetGuidance ( " Set to non-zero to turn on. " );
   fCmd_isotropic->SetDefaultValue ( 0 );
   fCmd_isotropic->AvailableForStates ( G4State_Idle );

   fCmd_momentum = new G4UIcmdWithADouble ( "/beta/gun/momentum",this );
   fCmd_momentum->SetGuidance ( " Set momentum of particle " );
   fCmd_momentum->SetGuidance ( " " );
   fCmd_momentum->SetDefaultValue ( 0 );
   fCmd_momentum->AvailableForStates ( G4State_Idle );

   fCmd_setBeamEnergy = new G4UIcmdWithADouble("/beta/setBeamEnergy",this);
   fCmd_setBeamEnergy->SetGuidance(" Set the electron beam energy in units of GeV.");
   fCmd_setBeamEnergy->SetDefaultValue(fSimManager->GetBeamEnergy());
   fCmd_setBeamEnergy->AvailableForStates(G4State_Idle);

   fCmd_setThetaMin = new G4UIcmdWithADouble ( "/beta/gun/setThetaMin",this );
   fCmd_setThetaMin->SetGuidance ( " Set the (first) primary particle theta minimum in degrees " );
   fCmd_setThetaMin->SetDefaultValue ( 35.0 );
   fCmd_setThetaMin->AvailableForStates ( G4State_Idle );

   fCmd_setThetaMax = new G4UIcmdWithADouble ( "/beta/gun/setThetaMax",this );
   fCmd_setThetaMax->SetGuidance ( " Set the (first) primary particle theta maximum in degrees " );
   fCmd_setThetaMax->SetDefaultValue ( 45.0 );
   fCmd_setThetaMax->AvailableForStates ( G4State_Idle );

   fCmd_setPhiMin = new G4UIcmdWithADouble ( "/beta/gun/setPhiMin",this );
   fCmd_setPhiMin->SetGuidance ( " Set the (first) primary particle phi minimum in degrees " );
   fCmd_setPhiMin->SetDefaultValue ( -15.0 );
   fCmd_setPhiMin->AvailableForStates ( G4State_Idle );

   fCmd_setPhiMax = new G4UIcmdWithADouble ( "/beta/gun/setPhiMax",this );
   fCmd_setPhiMax->SetGuidance ( " Set the (first) primary particle phi maximum in degrees " );
   fCmd_setPhiMax->SetDefaultValue ( 15.0 );
   fCmd_setPhiMax->AvailableForStates ( G4State_Idle );

   fCmd_setEnergyMin = new G4UIcmdWithADouble ( "/beta/gun/setEnergyMin",this );
   fCmd_setEnergyMin->SetGuidance ( " Set the (first) primary particle minimum energy in GeV " );
   fCmd_setEnergyMin->SetDefaultValue ( 1.0 );
   fCmd_setEnergyMin->AvailableForStates ( G4State_Idle );

   fCmd_setEnergyMax = new G4UIcmdWithADouble ( "/beta/gun/setEnergyMax",this );
   fCmd_setEnergyMax->SetGuidance ( " Set the (first) primary particle maximum energy in GeV" );
   fCmd_setEnergyMax->SetDefaultValue ( 5.7 );
   fCmd_setEnergyMax->AvailableForStates ( G4State_Idle );

   fCmd_setMomentumMin = new G4UIcmdWithADouble ( "/beta/gun/setMomentumMin",this );
   fCmd_setMomentumMin->SetGuidance ( " Set the (first) primary particle minimum momentum in GeV/c " );
   fCmd_setMomentumMin->SetDefaultValue ( 1.0 );
   fCmd_setMomentumMin->AvailableForStates ( G4State_Idle );

   fCmd_setMomentumMax = new G4UIcmdWithADouble ( "/beta/gun/setMomentumMax",this );
   fCmd_setMomentumMax->SetGuidance ( " Set the (first) primary particle maximum momentum in GeV/c" );
   fCmd_setMomentumMax->SetDefaultValue ( 5.7 );
   fCmd_setMomentumMax->AvailableForStates ( G4State_Idle );

   fCmd_refreshGenerator = new G4UIcmdWithoutParameter("/beta/gun/refresh",this);
   fCmd_refreshGenerator->SetGuidance ( " Refresh the event generator after modifying settings " );
   fCmd_refreshGenerator->AvailableForStates ( G4State_Idle );

   fCmd_setType = new G4UIcmdWithAString("/beta/gun/setEventType",this);
   fCmd_setType->SetGuidance ( "Set the type of event generated. Note that this resets all the set values to their defaults." );
   fCmd_setType->SetGuidance ( "Arguments:" );
   fCmd_setType->SetGuidance ( " name              : Class" );
   fCmd_setType->SetGuidance ( "                     description" );
   fCmd_setType->SetGuidance ( " flat              : BigcalCenterEventGenerator" );
   fCmd_setType->SetGuidance ( "                     uniformly populate events in BigCal " );
   fCmd_setType->SetGuidance ( " electronPion      : InclusiveElectronPionGenerator" );
   fCmd_setType->SetGuidance ( "                     Inclusive electron cross section (DIS,QE,ERT) along with the" );
   fCmd_setType->SetGuidance ( "                     inclusive pion electro production using Oscar's fit." );
   fCmd_setType->SetGuidance ( "                     Uses the full UVA NH3 target model." );
   fCmd_setType->SetGuidance ( " electronPion_LHe  : InclusiveElectronPionGenerator" );
   fCmd_setType->SetGuidance ( "                     Same as electronPion but it uses just a simple one material liquid He target." );
   fCmd_setType->SetGuidance ( " allNH3            : NH3TargetEventGenerator" );
   fCmd_setType->SetGuidance ( "                     electrons and pions from NH3 target" );
   fCmd_setType->SetGuidance ( " disNH3            : SANEInclusiveDISEventGenerator" );
   fCmd_setType->SetGuidance ( "                     only electrons from NH3 target                                   " );
   fCmd_setType->SetGuidance ( " mott              : MottEventGenerator" );
   fCmd_setType->SetGuidance ( "                     mott cross section                                               " );
   fCmd_setType->SetGuidance ( " cone              : ConeEventGenerator" );
   fCmd_setType->SetGuidance ( "                     a cone with a small energy range                                 " );
   fCmd_setType->SetGuidance ( " dis               : DISEventGenerator" );
   fCmd_setType->SetGuidance ( "                     inclusive electron DIS(F1p and F2p)                              " );
   fCmd_setType->SetGuidance ( " beamOnTarget      : BeamOnTargetEventGenerator" );
   fCmd_setType->SetGuidance ( "                     electron beam shot from up stream on the target (GEANT4 physics) " );
   fCmd_setType->SetGuidance ( " saved             : BETAG4EventGenerator" );
   fCmd_setType->SetGuidance ( "                     should be used with loadFromFile" );
   fCmd_setType->SetDefaultValue ( "flat" );
   fCmd_setType->AvailableForStates ( G4State_Idle );

   fCmd_loadEGFromFile = new G4UIcmdWithAString("/beta/gun/loadEGFromFile",this);
   fCmd_loadEGFromFile->SetGuidance ( "Argument is name of saved object" );
   fCmd_loadEGFromFile->SetGuidance ( "Use setEGFile to set the location of the file before using this command." );
   fCmd_loadEGFromFile->SetDefaultValue ( "saved_event_gen" );
   fCmd_loadEGFromFile->AvailableForStates ( G4State_Idle );

   fCmd_saveEGToFile = new G4UIcmdWithAString("/beta/gun/saveEGToFile",this);
   fCmd_saveEGToFile->SetGuidance ( "Argument is name of the object to be saved." );
   fCmd_saveEGToFile->SetGuidance ( "Use setEGFile to set the location of the file before using this command." );
   fCmd_saveEGToFile->SetDefaultValue ( "saved_event_gen" );
   fCmd_saveEGToFile->AvailableForStates ( G4State_Idle );

   fCmd_setEGFile = new G4UIcmdWithAString("/beta/gun/setEGFile",this);
   fCmd_setEGFile->SetGuidance ( "Root file name where event generator is saved." );
   fCmd_setEGFile->SetGuidance ( "This is the file used when loadFromFile command is issued." );
   fCmd_setEGFile->SetDefaultValue ( "saved_event_gen.root" );
   fCmd_setEGFile->AvailableForStates ( G4State_Idle );

   fCmd_setPSVariable = new G4UIcmdWithAString ( "/beta/gun/setPSVariable",this );
   fCmd_setPSVariable->SetGuidance ( " Set a phase space variable to a value"  );
   fCmd_setPSVariable->SetGuidance ( " Note the units should be in the correct units for the variable"  );
   fCmd_setPSVariable->SetGuidance ( " "  );
   fCmd_setPSVariable->AvailableForStates ( G4State_Idle );


   fCmd_listPSVariables = new G4UIcmdWithoutParameter("/beta/gun/listPSVariables",this);
   fCmd_listPSVariables->SetGuidance ( " List the phase space variables " );
   fCmd_listPSVariables->AvailableForStates ( G4State_Idle );


   fCmd_setParticle = new G4UIcmdWithAString ( "/beta/gun/setParticle",this );
   fCmd_setParticle->SetGuidance ( "  Set the particle thrown (if event generator is applicable) \n use the simple particle name... e-,e+,pi+,proton,kaon- " );
   fCmd_setParticle->SetDefaultValue ( "e-" );
   fCmd_setParticle->AvailableForStates ( G4State_Idle );

   fCmd_sigmaMomentum = new G4UIcmdWithADouble ( "/beta/gun/sigmaMomentum",this );
   fCmd_sigmaMomentum->SetGuidance ( " Set relative error of particle momentum" );
   fCmd_sigmaMomentum->SetDefaultValue ( 0 );
   fCmd_sigmaMomentum->AvailableForStates ( G4State_Idle );

   fCmd_sete_piRatio = new G4UIcmdWithADouble ( "/beta/gun/SetElectronPionRatio",this );
   fCmd_sete_piRatio->SetGuidance ( " Set the electron - pion ratio \nSetting to one is all electrons " );
   fCmd_sete_piRatio->SetDefaultValue ( 1.0/100.0 );
   fCmd_sete_piRatio->AvailableForStates ( G4State_Idle );

   fCmd_setpi0Ratio = new G4UIcmdWithADouble ( "/beta/gun/SetPiZeroRatio",this );
   fCmd_setpi0Ratio->SetGuidance ( "  Set the pi0 - charged pion ratio \n Setting >= 1 makes all pions neutral " );
   fCmd_setpi0Ratio->SetDefaultValue ( 0.0 );
   fCmd_setpi0Ratio->AvailableForStates ( G4State_Idle );

}
//______________________________________________________________________________
BETASimulationMessenger::~BETASimulationMessenger() {

}
//______________________________________________________________________________
void BETASimulationMessenger::SetNewValue ( G4UIcommand* command, G4String newValue ) {
   BETADetectorConstruction   * fDetConstruction = GetDetectorConstruction();
   BETAPrimaryGeneratorAction * BETAAction       = GetPrimaryGeneratorAction();
   BETAG4EventGenerator       * anEventGen       = BETAAction->GetEventGenerator();
   BETASimulationManager      * simManager       = BETASimulationManager::GetInstance();

   if ( command == fCmd_simTrigger ) {
      std::cout << "simTrig" << fCmd_simTrigger->GetNewIntValue(newValue) << std::endl;
      if(fCmd_simTrigger->GetNewIntValue(newValue) != 0) simManager->fSimulateTrigger = true;
      else simManager->fSimulateTrigger = false;
   }
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
      //std::cout << " args = " << newValue.data() << "\n";
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
      //std::cout << " args = " << newValue.data() << "\n";
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

   if ( command == fCmd_toggleBigcal )
   {
      //std::cout << " args = " << newValue.data() << "\n";
      if( !(strcmp(newValue.data(),"on")) )  {
         fDetConstruction->usingBigcal = true;
         fDetConstruction->fSimulationManager->fSimulateBigcalOptics = true;
         fDetConstruction->DestroyBigCal();
         if(fDetConstruction->constructed == true) fDetConstruction->ConstructBigCal();
      }
      if( !(strcmp(newValue.data(),"off"))) {
         fDetConstruction->usingBigcal = false;
         fDetConstruction->fSimulationManager->fSimulateBigcalOptics = false;
         fDetConstruction->DestroyBigCal();
         if(fDetConstruction->constructed == true) fDetConstruction->ConstructBigCal();
      }
      if( !(strcmp(newValue.data(),"noOptics"))) {
         fDetConstruction->usingBigcal = true;
         fDetConstruction->fSimulationManager->fSimulateBigcalOptics = false;
         fDetConstruction->DestroyBigCal();
         if(fDetConstruction->constructed == true) fDetConstruction->ConstructBigCal();

      }
      G4RunManager::GetRunManager()->GeometryHasBeenModified();
   }

   if ( command == fCmd_setThetaMax ) {
      anEventGen->SetThetaMax(fCmd_setThetaMax->GetNewDoubleValue(newValue)*TMath::Pi()/180.0);
      anEventGen->NeedsRefreshed();
   }
   if ( command == fCmd_setPhiMax ) {
      anEventGen->SetPhiMax(fCmd_setPhiMax->GetNewDoubleValue(newValue)*TMath::Pi()/180.0);
      anEventGen->NeedsRefreshed();
   }
   if ( command == fCmd_setEnergyMax ) {
      anEventGen->SetEnergyMax(fCmd_setEnergyMax->GetNewDoubleValue(newValue));
      anEventGen->NeedsRefreshed();
   }
   if ( command == fCmd_setThetaMin ) {
      anEventGen->SetThetaMin(fCmd_setThetaMin->GetNewDoubleValue(newValue)*TMath::Pi()/180.0);
      anEventGen->NeedsRefreshed();
   }
   if ( command == fCmd_setPhiMin ) {
      anEventGen->SetPhiMin(fCmd_setPhiMin->GetNewDoubleValue(newValue)*TMath::Pi()/180.0);
      anEventGen->NeedsRefreshed();
   }
   if ( command == fCmd_setEnergyMin) {
      anEventGen->SetEnergyMin(fCmd_setEnergyMin->GetNewDoubleValue(newValue));
      anEventGen->NeedsRefreshed();
   }
   if ( command == fCmd_setMomentumMax ) {
      anEventGen->SetMomentumMax(fCmd_setMomentumMax->GetNewDoubleValue(newValue));
      anEventGen->NeedsRefreshed();
   }
   if ( command == fCmd_setMomentumMin) {
      anEventGen->SetMomentumMin(fCmd_setMomentumMin->GetNewDoubleValue(newValue));
      anEventGen->NeedsRefreshed();
   }

   if ( command == fCmd_setType) {

      double oldBeamEnergy = anEventGen->GetBeamEnergy();

      if( newValue == "flat" ) {
         anEventGen = new BigcalCenterEventGenerator();
         BETAAction->SetEventGenerator(anEventGen);

      } else if( newValue == "cone" ) {
         anEventGen = new ConeEventGenerator();
         BETAAction->SetEventGenerator(anEventGen);

      } else if( newValue == "dis" ) {
         anEventGen = new DISEventGenerator();
         BETAAction->SetEventGenerator(anEventGen);

      } else if( newValue == "allNH3" ) {
         anEventGen = new NH3TargetEventGenerator();
         BETAAction->SetEventGenerator(anEventGen);

      } else if( newValue == "disNH3" ) {
         anEventGen = new SANEInclusiveDISEventGenerator();
         BETAAction->SetEventGenerator(anEventGen);

      } else if( newValue == "poldis" ) {
         anEventGen = new PolarizedDISEventGenerator();
         BETAAction->SetEventGenerator(anEventGen);

      } else if( newValue == "mott" ) {
         anEventGen = new MottEventGenerator();
         BETAAction->SetEventGenerator(anEventGen);

      } else if( newValue == "pion" ) {
         anEventGen = new InclusivePionEventGenerator();
         BETAAction->SetEventGenerator(anEventGen);

      } else if( newValue == "wiser" ) {
         anEventGen = new WiserEventGenerator();
         BETAAction->SetEventGenerator(anEventGen);

      } else if( newValue == "electronPion" ) {
         anEventGen =  new InclusiveElectronPionGenerator();
         BETAAction->SetEventGenerator(anEventGen);

      } else if( newValue == "electronPion_LHe" ) {
         anEventGen =  new InclusiveElectronPionGenerator(new UVAPureHeliumTarget());
         BETAAction->SetEventGenerator(anEventGen);


      } else if( newValue == "beamOnTarget" ) {
         anEventGen = new BeamOnTargetEventGenerator();
         BETAAction->SetEventGenerator(anEventGen);

      } else if( newValue == "saved" ) {
         anEventGen = new BETAG4EventGenerator();
         BETAAction->SetEventGenerator(anEventGen);
         // Set to true because we want to avoid doing the initializations at the end of this memeber function
         anEventGen->fIsInitialized = true;

      } else {
         std::cout << " Illegal parameter: " << newValue << std::endl;
         G4UImanager* UI = G4UImanager::GetUIpointer();
         UI->ApplyCommand("help /beta/gun/setEventType");
      }
      //anEventGen->SetModified(true);
      if( !anEventGen->fIsInitialized ) {
         anEventGen->SetBeamEnergy(oldBeamEnergy);
         anEventGen->Initialize();
         anEventGen->fIsInitialized = true;
      } else if( !(newValue == "saved") ) {
         BETAAction->GetEventGenerator()->Refresh();
         anEventGen->SetBeamEnergy(oldBeamEnergy);
      }
   }

   if ( command == fCmd_setEGFile) {
      simManager->fEGFileName = newValue;
   }

   if ( command == fCmd_loadEGFromFile) {
      std::cout << " LOADING EG FROM FILE " << std::endl;
      simManager->fEGName = newValue;
      anEventGen = new BETAG4EventGenerator();
      BETAAction->SetEventGenerator(anEventGen);
      anEventGen->LoadFromFile(simManager->fEGFileName.data(),simManager->fEGName.data());
      anEventGen->Dump();
   }

   if ( command == fCmd_saveEGToFile) {
      simManager->fEGName = newValue;
      anEventGen->SaveToFile(simManager->fEGFileName.data(),simManager->fEGName.data());
   }

   if ( command == fCmd_refreshGenerator) {
        if( !anEventGen->fIsInitialized ) {
           anEventGen->Initialize();
           anEventGen->fIsInitialized = true;
        }
        else BETAAction->GetEventGenerator()->Refresh();
   }

   if ( command == fCmd_listPSVariables) {
      BETAAction->GetEventGenerator()->Print();
   }

   if ( command == fCmd_setBeamEnergy ) {
      BETAAction->GetEventGenerator()->SetBeamEnergy( fCmd_setBeamEnergy->GetNewDoubleValue(newValue) );
      fSimManager->SetBeamEnergy(fCmd_setBeamEnergy->GetNewDoubleValue(newValue)); 
   }

   if ( command == fCmd_setParticle )
   {
      G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
      TList * samplers = anEventGen->GetSamplers();
      if(samplers->GetEntries() > 0 ) {
	 int partnum =  TDatabasePDG::Instance()->GetParticle(newValue.data())->PdgCode(); 
         ((InSANEPhaseSpaceSampler*)(samplers->At(0)))->GetXSec()->SetParticleType(partnum );
         ((InSANEPhaseSpaceSampler*)(samplers->At(0)))->GetXSec()->InitializeFinalStateParticles( );
         //((InSANEPhaseSpaceSampler*)(samplers->At(0)))->GetXSec()->Print( );
	 //std::cout << " setting particle by pdg code " << partnum <<  " \n";
      }
      else std::cout << " NO SAMPLERS YET\n"; 
      BETAAction->GetParticleGun()->SetParticleDefinition(particleTable->FindParticle ( newValue ));
      anEventGen->SetModified(true);
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

