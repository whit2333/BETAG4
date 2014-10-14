#include "BETARun.hh"

#include "G4MultiFunctionalDetector.hh"
#include "G4SDManager.hh"
#include "G4VPrimitiveScorer.hh"
#include "G4THitsMap.hh"

#include <assert.h>
#include <string>
#include <sstream>
#include <fstream>
#include <algorithm>

#include "BETAFakePlane.hh"
#include "BETAEvent.h"
#include "HMSEvent.h"
#include "HallCBeamEvent.h"
#include "BETAG4MonteCarloEvent.h"


//______________________________________________________________________________
BETARun::BETARun ( const int runNumber ) : catLastFile( false ) {

   fNUpdate = 100;

   //if(BETASimulationManager::GetInstance()->fDebugLevel > 0) {
   //   G4cout << "= Created new BETARun \n";
   //}

   fSimulationManager             = BETASimulationManager::GetInstance ();
   fSimulationManager->fRunNumber = runNumber;

   // Get the event generator and detector constructoion 
   // so we can write the simulation truths to tree
   runManager = G4RunManager::GetRunManager();
   if(runManager) {
      //generator = (BETAPrimaryGeneratorAction *)runManager->GetUserPrimaryGeneratorAction();
      //generator->SetMCEventAddress(mcEvent);
      construction = (BETADetectorConstruction *)runManager->GetUserDetectorConstruction();
   }

   // Create the DAQ system
   fDAQReadout = new BETAG4DAQReadout("/DAQ/TSMod");
   fDAQReadout->SetTriggerEvent(fSimulationManager->fEvents->TRIG);

   // Create Detector digitizers 
   fBETADigitizer = new BETADigitizer("/DAQ/BETAMod");
   fBETADigitizer->SetTriggerEvent(fSimulationManager->fEvents->TRIG);

   G4DigiManager * DM = G4DigiManager::GetDMpointer();
   DM->AddNewModule(fDAQReadout);
   DM->AddNewModule(fBETADigitizer);

   std::cout << "Digitizer Modules: " << std::endl;
   DM->List();

   runManager->SetNumberOfEventsToBeStored(1);
}
//______________________________________________________________________________
BETARun::~BETARun() {
   //if(BETASimulationManager::GetInstance()->fDebugLevel > 0) {
   //   G4cout << "= Deleted BETARun \n";
   //}
}
//______________________________________________________________________________
void BETARun::RecordEvent ( const G4Event* anEvent ) {

   //if(BETASimulationManager::GetInstance()->fDebugLevel > 2) { std::cout << "start of BETARun::RecordEvent() \n";  }
   if ( ( numberOfEvent%fNUpdate ) == 0 ) {
      G4cout << " Event " << numberOfEvent << G4endl;
   }

   //---------------------------
   // "scaler event"
   // Every 100 events a scaler event is read. 
   //---------------------------
   if ( ( numberOfEvent%100 ) == 0 ) {
      //G4cout << " Event " << numberOfEvent << G4endl;
      if(fSimulationManager->fSANEScalers->fTriggerEvent){
         fSimulationManager->fSANEScalers->fTriggerEvent->fCodaType = 0;
         fSimulationManager->fSANEScalers->fTriggerEvent->fEventNumber = numberOfEvent;
         fSimulationManager->fSANEScalers->fTriggerEvent->fRunNumber = fSimulationManager->fRunNumber;
      }
      SANEScalerEvent * scalerEvent = fSimulationManager->fSANEScalers->fScalerEvent;

      scalerEvent->fEventNumber = numberOfEvent;
      scalerEvent->fRunNumber   = fSimulationManager->fRunNumber;

      scalerEvent->f1MHzClockScaler2 = 1000000;
      scalerEvent->f1MHzClockScaler  = 1000000;
      scalerEvent->f1MHzClockPositiveHelicityScaler = 500000; 
      scalerEvent->f1MHzClockNegativeHelicityScaler = 500000; 
      double current = 90.0;
      scalerEvent->fBCM1Scaler = scalerEvent->GetScalerBCM1(current); 
      scalerEvent->fBCM2Scaler = scalerEvent->GetScalerBCM2(current); 
      scalerEvent->fBCM1PositiveHelicityScaler = scalerEvent->GetPositiveScalerBCM1(current); 
      scalerEvent->fBCM2PositiveHelicityScaler = scalerEvent->GetPositiveScalerBCM2(current); 
      scalerEvent->fBCM1NegativeHelicityScaler = scalerEvent->GetNegativeScalerBCM1(current); 
      scalerEvent->fBCM2NegativeHelicityScaler = scalerEvent->GetNegativeScalerBCM2(current); 

      fSimulationManager->fScalerTree->Fill();
      fSimulationManager->fSANEScalers->ClearEvent();
      fSimulationManager->fSANEScalers->fScalerEvent->ClearEvent();
      numberOfEvent++;
   }
   //-----------------------------------------------------------------
  
   fSimulationManager->fEventNumber = numberOfEvent; // numberOfEvent is G4Run datamember that is incremented manually by user (at bottom)

   fBETADigitizer->Reset();
   fDAQReadout->Reset();

   // Simulates the trigger supervisor
   fDAQReadout->Digitize();

   //fDAQReadout->Print();
   //fBETAScalers->Digitize();
   
   if( fDAQReadout->IsGoodEvent() || !(fSimulationManager->fSimulateTrigger) ) {

      //if( fDAQReadout->IsGoodEvent() ) 
         fBETADigitizer->SetTriggered(true);

      //std::cout << " Above Readout Triggered DAQ! \n";
      if(fBETADigitizer->IsTriggered()){
         fBETADigitizer->Digitize();
      }

      fSimulationManager->fEvents->fEventNumber       = numberOfEvent;
      fSimulationManager->fEvents->fRunNumber         = fSimulationManager->GetRunNumber();
      fSimulationManager->fEvents->TRIG->fEventNumber = numberOfEvent;
      fSimulationManager->fEvents->TRIG->fRunNumber   = fSimulationManager->fRunNumber;
      fSimulationManager->fEvents->BETA->fEventNumber = numberOfEvent;
      fSimulationManager->fEvents->BETA->fRunNumber   = fSimulationManager->GetRunNumber();
      fSimulationManager->fEvents->BEAM->fEventNumber = numberOfEvent;
      fSimulationManager->fEvents->BEAM->fRunNumber   = fSimulationManager->GetRunNumber();
      fSimulationManager->fEvents->TRIG->fEventNumber = numberOfEvent;
      fSimulationManager->fEvents->TRIG->fRunNumber   = fSimulationManager->GetRunNumber();
      fSimulationManager->fEvents->HMS->fEventNumber  = numberOfEvent;
      fSimulationManager->fEvents->HMS->fRunNumber    = fSimulationManager->GetRunNumber();

      //fBETADigitizer->Print();
      

      if(fBETADigitizer->IsTriggered())fBETADigitizer->ReadOut(); // contains the detector's readout
      if(fBETADigitizer->IsTriggered())fDAQReadout->ReadOut();    // contains the fake plane's readout

      fSimulationManager->fDetectorTree->Fill();

   }

   fBETADigitizer->Clear();
   fDAQReadout->Clear();
   //fDAQReadout->Reset();
   //fBETADigitizer->Reset();

   //if(BETASimulationManager::GetInstance()->fDebugLevel > 2) {
   //   std::cout << "end of BETARun::RecordEvent() \n";
   //}

   numberOfEvent++;

}
//______________________________________________________________________________
void BETARun::DumpHallCMC() {
   MCOutput.open ( "MC_BIGCAL.dat" );
   // Double_t  BigCalEnergyDeposit[720+1024] ;
   int k;
   for (k=0;k<(720+1024);k++)
   {
      //MCOutput << recordedEvent->BigCalEnergyDeposit[k] << "\n";
   }
   MCOutput.close();
}
//______________________________________________________________________________
void BETARun::DumpData() const {
}
//______________________________________________________________________________
void BETARun::Print ( const std::vector<G4String>& title,
                      const std::map< G4int, std::vector<G4double> >&myMap ) const
{
//    G4cout << "BETARun : PRINT "<< G4endl;
// 
//    // Print to G4cout and an output file
//    std::ofstream outFile ( "output.csv" );
// 
//    // Print title vector
//    std::vector<G4String>::const_iterator titleIter = title.begin();
// 
//    while ( titleIter != title.end() )
//    {
//       G4cout << std::setw ( 8 ) <<*titleIter<<" ";
//       titleIter++;
//    }
// 
//    G4cout<<G4endl;
// 
//    // Print current data
//    std::map< G4int, std::vector<G4double> >::const_iterator iter = myMap.begin();
// 
//    while ( iter != myMap.end() )
//    {
//       G4cout << std::setw ( 8 ) <<std::setprecision ( 3 ) << iter->first<<" ";
// 
//       std::vector<G4double>::const_iterator energyBinIter = iter->second.begin();
// 
//       while ( energyBinIter != iter->second.end() )
//       {
//          G4double value = *energyBinIter;
//          G4cout << std::setw ( 8 ) <<std::setprecision ( 3 ) << value*1e6<<" ";
//          outFile << value<<",";
//          energyBinIter++;
//       }
//       outFile<<G4endl;
//       G4cout<<G4endl;
//       iter++;
//    }
}
//______________________________________________________________________________
void BETARun::GeneratePedestals() {
   // emulate pedestals
   TRandom * rand = InSANERunManager::GetRunManager()->GetRandom();
   
   for(int k = 0 ;k<1000;k++ ) {
     fSimulationManager->fEvents->Clear();
     fSimulationManager->fEvents->fEventNumber = numberOfEvent;
     fSimulationManager->fEvents->fRunNumber   = fSimulationManager->GetRunNumber();
     fSimulationManager->fEvents->BETA->fEventNumber = numberOfEvent;
     fSimulationManager->fEvents->BETA->fRunNumber = fSimulationManager->GetRunNumber();
     fSimulationManager->fEvents->BEAM->fEventNumber = numberOfEvent;
     fSimulationManager->fEvents->BEAM->fRunNumber = fSimulationManager->GetRunNumber();
     fSimulationManager->fEvents->TRIG->fEventNumber = numberOfEvent;
     fSimulationManager->fEvents->TRIG->fRunNumber = fSimulationManager->GetRunNumber();
     fSimulationManager->fEvents->HMS->fEventNumber = numberOfEvent;
     fSimulationManager->fEvents->HMS->fRunNumber = fSimulationManager->GetRunNumber();
     fSimulationManager->fEvents->TRIG->fTriggerBits[7] = true;//ped bit
     fSimulationManager->fEvents->TRIG->fCodaType = 4; // ped coda type

     TClonesArray * fThrownParticles = fSimulationManager->fEvents->MC->fThrownParticles;
     if(fThrownParticles->GetEntries() > 0){
        InSANEParticle * aPart = (InSANEParticle*)(*fThrownParticles)[0];
        aPart->fHelicity = rand->Uniform(-1.0,1.0);
     }
     fBETADigitizer->DigitizePedestals();
     fBETADigitizer->ReadOut();

     fSimulationManager->fDetectorTree->Fill();

     fBETADigitizer->Clear();

     fSimulationManager->fEvents->Clear();

     numberOfEvent++;
   }

   fBETADigitizer->Clear();
   fBETADigitizer->DeleteHitCollections();

}
//______________________________________________________________________________


