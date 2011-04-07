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
   // Time comes from 2nd photon detected

  fSimulationManager = BETASimulationManager::GetInstance ();
  fSimulationManager->fRunNumber = runNumber;

  if(fSimulationManager) {
    fSimulationManager->SetDetectorVerbosity("GasCherenkov",0);
  }



// Get the event generator and detector constructoion so we can write the simulation truths to tree
   runManager = G4RunManager::GetRunManager();
   if(runManager) {
     //printf("\n got run manager \n");
     generator = (BETAPrimaryGeneratorAction *)runManager->GetUserPrimaryGeneratorAction();
   }
   if(runManager) {
     //printf("\n got run manager \n");
     construction = (BETADetectorConstruction *)runManager->GetUserDetectorConstruction();
   }

   RootFile     = fSimulationManager->fRootFile;
   detectorTree = fSimulationManager->fDetectorTree;

   fDAQReadout = new BETAG4DAQReadout("/DAQ/TriggerSupervisor");
   fDAQReadout->SetTriggerEvent(fSimulationManager->fEvents->TRIG);

   fBETADigitizer = new BETADigitizer("/DAQ/BETA");

// Fills events with realisitic data from Geant4 Hit collections
//     eventRecorder = 
//        new BETAG4EventRecorder( );
 
// For debugging cherenkov timing emulator
/*   waveforms = new TH1F(Form("waveform%d",numberOfEvent),"waveform",200,0,20);*/
//    G4double averageTime[12];
//   PMTG4HCID         =-1;
//   PMTHCID           =-1;
//   MirrorHCID        =-1;
//   hodoscopePMTHCID  =-1;
//   BIGCALID          =-1;
//   BIGCALID2         =-1;
//   FTID              =-1;
//   fakePlaneID       =-1;
/// TODO Implement a print to screen for run info... 
//    G4SDManager* SDman = G4SDManager::GetSDMpointer();
//    if (construction->usingGasCherenkov) 
//      PMTG4HCID = SDman->GetCollectionID( colName="myCellScorer/eTrackLength" );
//    if (construction->usingGasCherenkov) 
//      PMTHCID = SDman->GetCollectionID ( colName="PMT/pmt" );
//    if (construction->usingGasCherenkov) 
//      MirrorHCID = SDman->GetCollectionID ( colName="Mirrors/mirrors" );
//    if (construction->usingLuciteHodoscope) 
//      hodoscopePMTHCID = SDman->GetCollectionID ( colName="hodoscopePMT/pmt" );
//    if (construction->usingBigcal) 
//      BIGCALID  = SDman->GetCollectionID ( colName="BIGCALRCS/RCSCalorimeterCollection" );
//    if (construction->usingBigcal) 
//      BIGCALID2 = SDman->GetCollectionID ( colName="BIGCALBottom/BottomCalorimeterCollection" );
//    if (construction->usingForwardTracker)  
//      FTID = SDman->GetCollectionID ( colName="FrontTracker/FrontTracker" );
//    if (construction->usingFakePlaneAtBigcal) 
//      fakePlaneID = SDman->GetCollectionID ( colName="FakePlaneAtBigcal/FakePlane" );
//    if (construction->usingBigcal)
//      fBigcalHCID  = SDman->GetCollectionID ( colName="BIGCAL/bigcal" );

/*  G4String detName[4] = {"tracker","cherenkov","hodoscope","bigcal"};
  G4String primNameSum[4] = {"chargeHit","photonSurf","chargeHit","pSurfFlux"/*,"trackLength","nStep};*/
  //G4String primNameMin[3] = {"minEkinGamma","minEkinElectron","minEkinPositron"};


/*  G4String fullName;*/
//   for(size_t i=0;i<4;i++)
//   {
//     //for(size_t j=0;j<4;j++)
//     //{
//       fullName = detName[i]+"/"+primNameSum[i];
//       colIDSum[i] = SDman->GetCollectionID(fullName);
// std::cout << "HC ID " << i << " " <<  colIDSum[i] << " \n";
//     //}
// //    for(size_t k=0;k<3;k++)
// //    {
// //      fullName = detName[i]+"/"+primNameMin[k];
// //      colIDMin[i][k] = SDMan->GetCollectionID(fullName);
// //    }
//   }
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

  fSimulationManager->fEventNumber = numberOfEvent;

// Simulates the trigger supervisor
   fDAQReadout->Digitize();
   fDAQReadout->Print();
//  fBETAScalers->Digitize();
   if( fDAQReadout->IsGoodEvent() ) {

/*    std::cout << " Above Readout Triggered DAQ! \n";*/
      fBETADigitizer->Digitize();

      fSimulationManager->fEvents->BETA->fEventNumber = numberOfEvent;
      fSimulationManager->fEvents->BETA->fRunNumber = numberOfEvent;
      fSimulationManager->fEvents->fRunNumber = fSimulationManager->fRunNumber;

//     fSimulationManager->fEvents->MC->fEnergyThrown = generator->fCurrentEnergy;
//     fSimulationManager->fEvents->MC->fThetaThrown = generator->fCurrentTheta;
//     fSimulationManager->fEvents->MC->fPhiThrown = generator->fCurrentPhi;
//     fSimulationManager->fEvents->MC->fParticleThrown = 1;

     fBETADigitizer->Print();

      fBETADigitizer->ReadOut();
      fDAQReadout->ReadOut();

      fSimulationManager->fDetectorTree->Fill();

      fBETADigitizer->Clear();

    }

    fDAQReadout->Clear();

      if ( ( numberOfEvent%10 ) == 0 ) G4cout << " Event " << numberOfEvent << G4endl;
      numberOfEvent++;

//   } //triggered
#ifdef BETAG4_DEBUG
  if(BETASimulationManager::GetInstance()->fDebugLevel > 2) {
    std::cout << "end of BETARun::RecordEvent() \n";
  }
#endif
}
//____________________________________________________________________//

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
   G4cout << "  " << G4endl;
   G4cout << " PMT "<< pmtTotalCount << G4endl;
   G4cout << " Mirror "<< mirrorTotalCount << G4endl;
   std::ofstream outFile ( "data/efficiency.dat", std::ios_base::app );
   outFile << static_cast<G4double> ( pmtTotalCount ) /static_cast<G4double> ( mirrorTotalCount ) <<G4endl;
   outFile.close();

   std::stringstream outstream;
   std::string fname;

   double xdim;
   double ydim;
   for ( int i = 1; i<9;i++ )
   {
      if ( i/2 != 0 )
      {
         xdim =  35.5;
         ydim = 36.5;
      }
      else
      {
         xdim  = 42.5; // not right??????????
         ydim = 36.5;
      }
      outstream << "data/miror." << i << ".dat"  ;
      fname  = outstream.str();
      std::ofstream      outFile ( "data/mirror.dat", std::ios_base::app );
      for ( int xi = 0; xi< 28*2; xi++ )
      {
         for ( int yi = 0; yi< 28*2; yi++ )
         {
            /*    outFile << xi << " " << yi << " " << mirror3PosVsPhotoelectrons->binHeight ( xi,yi ) << G4endl;*/
//G4cout << xi << " " << yi << " " << mirror3PosVsPhotoelectrons->binHeight(xi,yi)<< G4endl;

         }
      }
      outFile.close();
   }

}
//____________________________________________________________________//

void BETARun::Print ( const std::vector<G4String>& title,
                      const std::map< G4int, std::vector<G4double> >&myMap ) const
{
   G4cout << "BETARun : PRINT "<< G4endl;

   // Print to G4cout and an output file
   std::ofstream outFile ( "output.csv" );

   // Print title vector
   std::vector<G4String>::const_iterator titleIter = title.begin();

   while ( titleIter != title.end() )
   {
      G4cout << std::setw ( 8 ) <<*titleIter<<" ";
      titleIter++;
   }

   G4cout<<G4endl;

   // Print current data
   std::map< G4int, std::vector<G4double> >::const_iterator iter = myMap.begin();

   while ( iter != myMap.end() )
   {
      G4cout << std::setw ( 8 ) <<std::setprecision ( 3 ) << iter->first<<" ";

      std::vector<G4double>::const_iterator energyBinIter = iter->second.begin();

      while ( energyBinIter != iter->second.end() )
      {
         G4double value = *energyBinIter;
         G4cout << std::setw ( 8 ) <<std::setprecision ( 3 ) << value*1e6<<" ";
         outFile << value<<",";
         energyBinIter++;
      }
      outFile<<G4endl;
      G4cout<<G4endl;
      iter++;
   }
}
//____________________________________________________________________//

void BETARun::GeneratePedestals() {
   // emulate pedestals

   for(int k = 0 ;k<1000;k++ ) {
     
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

     fBETADigitizer->DigitizePedestals();

     fBETADigitizer->ReadOut();

     fSimulationManager->fDetectorTree->Fill();

     fBETADigitizer->Clear();

     numberOfEvent++;
   }
   
   

}
//________________________________________________________________________//
//int BETARun::FillGasCherenkovEvent() {
/*  betaEvent->fGasCherenkovEvent->fHits->Clear("C");
  TClonesArray &cherenkovHits = *(betaEvent->fGasCherenkovEvent->fHits);
  GasCherenkovHit * aCERhit;
  BETAPMTHit * aHit;
  int lastHitIndex[12] = {0,0,0,0,0,0,0,0,0,0,0,0};
  G4int totalHits = 0;

  G4int indTotalHits[12] = {0,0,0,0,0,0,0,0,0,0,0,0};
  G4double earliestHitTime[12] =  {9990.0,9990.0,9990.0,9990.0,9990.0,9990.0,9990.0,9990.0,9990.0,9990.0,9990.0,9990.0};
  G4double averageTime[12] = {-200000.0,-200000.0,-200000.0,-200000.0,-200000.0,-200000.0,
                              -200000.0,-200000.0,-200000.0,-200000.0,-200000.0,-200000.0 };
  G4double lastHitTime[12] = {-200000.0,-200000.0,-200000.0,-200000.0,-200000.0,-200000.0,
                              -200000.0,-200000.0,-200000.0,-200000.0,-200000.0,-200000.0 };

vector<BETAPMTHit*> * hitPointers = pmtHC->GetVector();
vector<BETAPMTHit*>::iterator it;
//std::sort (hitPointers->begin(), hitPointers->end(),cmp);
  // print out content:

  for (it=hitPointers->begin(); it!=hitPointers->end(); ++it)
    {
       std::cout << "hitPointers " << (*it) << " contains:";
       std::cout << " " << (*it)->Gtime << "\n";
    }

  std::cout << endl;
// end of attempt

// Loop loop through the Hit container once to find the earliest time
    for ( int i1=0 ; i1 < pmtHC->entries();i1++ )
      { 
      aHit = ( *pmtHC )[i1];
        //
        if ( aHit->tubeNumber == -1 )
          {
//        numPMTHitsAtFace++;
          }
        else
          {// normal pmt hit

  if(fSimulationManager->GetDetectorVerbosity("GasCherenkov") > 0) 
  { 
  std::cout << "Tube " <<  aHit->tubeNumber
            << " hit with global time " << aHit->Gtime/ns
            <<  "ns where the earliest time was " << earliestHitTime[aHit->tubeNumber]
            <<  "\n";
  }

          if( (aHit->Gtime/ns - lastHitTime[aHit->tubeNumber]) > 20.0 )
            {
            lastHitIndex[aHit->tubeNumber] = totalHits ;
            aCERhit = new(cherenkovHits[totalHits]) GasCherenkovHit();
            indTotalHits[aHit->tubeNumber]++;
            aCERhit->fMirrorNumber = aHit->tubeNumber;
            aCERhit->fADC = 0;
            averageTime[aHit->tubeNumber] = 0.0;
            totalHits++;

  if(  fSimulationManager->GetDetectorVerbosity("GasCherenkov") > 0) 
    std::cout << "new hit number " << totalHits << "\n";

            } // end of 
          waveforms->Fill(aHit->Gtime/ns);

          if( aHit->Gtime/ns < earliestHitTime[aHit->tubeNumber] ) earliestHitTime[aHit->tubeNumber]=aHit->Gtime/ns;

          aCERhit = (GasCherenkovHit*)(cherenkovHits)[lastHitIndex[aHit->tubeNumber]];

          (aCERhit[lastHitIndex[aHit->tubeNumber]]).fADC++;
          averageTime[aHit->tubeNumber] += aHit->Gtime/ns;
          aCERhit[lastHitIndex[aHit->tubeNumber]].fTDC =
            averageTime[lastHitIndex[aHit->tubeNumber]]/(aCERhit[lastHitIndex[aHit->tubeNumber]].fADC) ;
          lastHitTime[aHit->tubeNumber] = aHit->Gtime/ns;

          }
      }
//for(int jj =0;jj<totalHits;jj++) waveforms[jj]->Draw();
//  for (int k=0;k<8;k++) numPMTHits+=CherenkovPMTCount[k];
//   triggered = true;
//   if (triggered) //numPMTHits > 1 )   // 1 P.E.
//   {
//  Trigger=1; // Record  event
//      recordedEvent->cer_hit = tdc_count;
//      for (int k=0;k<8;k++) {
//         recordedEvent->cer_adc[k] = CherenkovPMTCount[k];
//         recordedEvent->ceradc_num[k]=k+1;
//      }
// RUN TOTALED VALUES
//      pmtTotalCount += numPMTHitsAtFace;
//      mirrorTotalCount += numMirrorHits;
return(totalHits);
*/
//}


//________________________________________________________________________//
// int BETARun::FillBigcalEvent() {

// return(0);
// }



