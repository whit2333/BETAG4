#include "G4MultiFunctionalDetector.hh"
#include "G4SDManager.hh"
#include "G4VPrimitiveScorer.hh"
#include <assert.h>
#include "BETAPMT.hh"
#include "BETAFakePlane.hh"
#include <string>  //For the string functions
#include <sstream>
#include <fstream>
#include <algorithm>
#include "Riostream.h"
#include "TROOT.h"
#include "TFile.h"
#include "TNetFile.h"
#include "TRandom.h"
#include "TTree.h"
#include "TBranch.h"
#include "TClonesArray.h"
#include "TStopwatch.h"
#include "TH1F.h"
#include "TObjArray.h"
#include "TH1.h"
#include "G4THitsMap.hh"
#include "TProfile.h"
#include "TRint.h"
#include "TApplication.h"
#include "TCanvas.h"
#include "BETARun.hh"
// InSANE
#include "BETAEvent.h"
#include "HMSEvent.h"
#include "HallCBeamEvent.h"
#include "BETAG4MonteCarloEvent.h"
#include "InSANERun.h"


//______________________________________________________________________//

BETARun::BETARun ( const int runNumber ) : catLastFile( false ) {

#ifdef BETAG4_DEBUG
  if(BETASimulationManager::GetInstance()->fDebugLevel > 0) {
    G4cout << "= Created new BETARun \n";
  }
#endif
  
  G4String colName;

  fSimulationManager = BETASimulationManager::GetInstance ();
//   fSimulationManager->fRunNumber = runNumber;

  if(fSimulationManager) {
    fSimulationManager->SetDetectorVerbosity("GasCherenkov",0);
  }

   mcEvent = fSimulationManager->fEvents->MC;

   /// Get the event generator and detector constructoion 
   /// so we can write the simulation truths to tree
   runManager = G4RunManager::GetRunManager();
   if(runManager) {
     generator = (BETAPrimaryGeneratorAction *)runManager->GetUserPrimaryGeneratorAction();
     generator->SetMCEventAddress(mcEvent);

     construction = (BETADetectorConstruction *)runManager->GetUserDetectorConstruction();
   }

   fDAQReadout = new BETAG4DAQReadout("/DAQ/TriggerSupervisor");
   fDAQReadout->SetTriggerEvent(fSimulationManager->fEvents->TRIG);

   fBETADigitizer = new BETADigitizer("/DAQ/BETA");

}
//____________________________________________________________________//

BETARun::~BETARun()
{
if(BETASimulationManager::GetInstance()->fDebugLevel > 0) {
   G4cout << "= Deleted BETARun \n";
}
}
//____________________________________________________________________//

void BETARun::RecordEvent ( const G4Event* anEvent ) {

#ifdef BETAG4_DEBUG
   if(BETASimulationManager::GetInstance()->fDebugLevel > 2) { std::cout << "start of BETARun::RecordEvent() \n";  }
#endif

   //---------------------------
   // "scaler event"
   // Every 100 events a scaler event is read. 
   //---------------------------
   if ( ( numberOfEvent%100 ) == 0 ) {
      G4cout << " Event " << numberOfEvent << G4endl;
      if(fSimulationManager->fSANEScalers->fTriggerEvent){
         fSimulationManager->fSANEScalers->fTriggerEvent->fCodaType = 0;
         fSimulationManager->fSANEScalers->fTriggerEvent->fEventNumber = numberOfEvent;
         fSimulationManager->fSANEScalers->fTriggerEvent->fRunNumber = fSimulationManager->fRunNumber;
      }

      fSimulationManager->fSANEScalers->fScalerEvent->fEventNumber = numberOfEvent;
      fSimulationManager->fSANEScalers->fScalerEvent->fRunNumber   = fSimulationManager->fRunNumber;

      fSimulationManager->fScalerTree->Fill();
      fSimulationManager->fSANEScalers->ClearEvent();
      fSimulationManager->fSANEScalers->fScalerEvent->ClearEvent();
      numberOfEvent++;
   }
   //---------------------------
  

   fSimulationManager->fEventNumber = numberOfEvent; /// numberOfEvent is G4Run datamember that is incremented manually by user (at bottom)

   // Simulates the trigger supervisor
   fDAQReadout->Digitize();
   //fDAQReadout->Print();
   //fBETAScalers->Digitize();
   if( fDAQReadout->IsGoodEvent() || !(fSimulationManager->fSimulateTrigger) ) {

      /*    std::cout << " Above Readout Triggered DAQ! \n";*/
      fBETADigitizer->Digitize();

      fSimulationManager->fEvents->fEventNumber = numberOfEvent;
      fSimulationManager->fEvents->fRunNumber = fSimulationManager->GetRunNumber();
      fSimulationManager->fEvents->TRIG->fEventNumber = numberOfEvent;
      fSimulationManager->fEvents->TRIG->fRunNumber = fSimulationManager->fRunNumber;
      fSimulationManager->fEvents->BETA->fEventNumber = numberOfEvent;
      fSimulationManager->fEvents->BETA->fRunNumber = fSimulationManager->GetRunNumber();
      fSimulationManager->fEvents->BEAM->fEventNumber = numberOfEvent;
      fSimulationManager->fEvents->BEAM->fRunNumber = fSimulationManager->GetRunNumber();
      fSimulationManager->fEvents->TRIG->fEventNumber = numberOfEvent;
      fSimulationManager->fEvents->TRIG->fRunNumber = fSimulationManager->GetRunNumber();
      fSimulationManager->fEvents->HMS->fEventNumber = numberOfEvent;
      fSimulationManager->fEvents->HMS->fRunNumber = fSimulationManager->GetRunNumber();

      //fBETADigitizer->Print();

      fBETADigitizer->ReadOut();
      fDAQReadout->ReadOut();

      fSimulationManager->fDetectorTree->Fill();

      fBETADigitizer->Clear();

   }

   fDAQReadout->Clear();
   numberOfEvent++;


#ifdef BETAG4_DEBUG
   if(BETASimulationManager::GetInstance()->fDebugLevel > 2) {
      std::cout << "end of BETARun::RecordEvent() \n";
   }
#endif

}
//____________________________________________________________________

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
//____________________________________________________________________//

void BETARun::DumpData() const
{
/*   G4cout << "  " << G4endl;
   G4cout << " PMT "<< pmtTotalCount << G4endl;
   G4cout << " Mirror "<< mirrorTotalCount << G4endl;*/
//    std::ofstream outFile ( "data/efficiency.dat", std::ios_base::app );
//    outFile << static_cast<G4double> ( pmtTotalCount ) /static_cast<G4double> ( mirrorTotalCount ) <<G4endl;
//    outFile.close();

//    std::stringstream outstream;
//    std::string fname;
// 
//    double xdim;
//    double ydim;
//    for ( int i = 1; i<9;i++ )
//    {
//       if ( i/2 != 0 )
//       {
//          xdim =  35.5;
//          ydim = 36.5;
//       }
//       else
//       {
//          xdim  = 42.5; // not right??????????
//          ydim = 36.5;
//       }
//       outstream << "data/miror." << i << ".dat"  ;
//       fname  = outstream.str();
//       std::ofstream      outFile ( "data/mirror.dat", std::ios_base::app );
//       for ( int xi = 0; xi< 28*2; xi++ )
//       {
//          for ( int yi = 0; yi< 28*2; yi++ )
//          {
//             /*    outFile << xi << " " << yi << " " << mirror3PosVsPhotoelectrons->binHeight ( xi,yi ) << G4endl;*/
// //G4cout << xi << " " << yi << " " << mirror3PosVsPhotoelectrons->binHeight(xi,yi)<< G4endl;
// 
//          }
//       }
//       outFile.close();
//    }

}
//____________________________________________________________________//

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
//____________________________________________________________________//

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

}
//________________________________________________________________________



