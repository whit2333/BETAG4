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

// ROOT //
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
#include "TH2.h"
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

#include "BETAG4EventRecorder.hh"

//______________________________________________________________________//
BETARun::BETARun ( const int runNumber, const int showThePlots ) : catLastFile( false )
{
   fShowUnrealisticData  = false;
   cer_tdc_thresh = 2;/// Time comes from 2nd photon detected

   analysisManager = BETAAnalysisManager::getInstance ( showThePlots );
   if(analysisManager) {
     analysisManager->SetDetectorVerbosity("GasCherenkov",0);
   }
// Get the event generator and detector constructoion so we can write the simulation truths to tree
   runManager = G4RunManager::GetRunManager();
   if (runManager) {
      //printf("\n got run manager \n");
      generator = (BETAPrimaryGeneratorAction *)runManager->GetUserPrimaryGeneratorAction();
   }
   if (runManager) {
      //printf("\n got run manager \n");
      construction = (BETADetectorConstruction *)runManager->GetUserDetectorConstruction();
   }

// Open Root File With A name Depending on the Run number
   std::string fname;
   std::stringstream out;

   if (catLastFile) {
      out << "data/beta.accumulate.root"  ;
      fname = out.str();
      const char * rootName = fname.c_str();
      RootFile = new TFile ( rootName,"UPDATE" );

   } else {
      out << "data/beta.run." << runNumber << ".root"  ;
      fname = out.str();
      const char * rootName = fname.c_str();
      RootFile = new TFile ( rootName,"RECREATE","BETA Simulation Output" );
   }

////////////////////////////////////////////////
//                    HISTOGRAMS              //
////////////////////////////////////////////////

   /*NONE */

////////////////////////////////////////////////
//           ROOT TREE          ////////////////
////////////////////////////////////////////////
    simulationRun = new InSANERun();

    betaEvent = new BETAEvent();
      betaEvent->AllocateMemory();
      betaEvent->AllocateHitsMemory();
    hmsEvent = new HMSEvent();
    beamEvent = new HallCBeamEvent();
    mcEvent = new BETAG4MonteCarloEvent();
      mcEvent->AllocateMemory();
      mcEvent->AllocateHitsMemory();
    eventRecorder = new BETAG4EventRecorder(betaEvent,hmsEvent,beamEvent,mcEvent);
// 
    if (catLastFile) {
 
       detectorTree=(TTree *) gROOT->FindObject("betaDetectors");
       if (!detectorTree) detectorTree = new TTree ( "betaDetectors","Detector Tree" );
 
       TBranch *b;
       b = detectorTree->GetBranch ("betaDetectorEvent");
       if (!b)  detectorTree->Branch ( "betaDetectorEvent","BETAEvent",&betaEvent );
       else b->SetAddress(&betaEvent );
       
       b = detectorTree->GetBranch("hmsDetectorEvent");
       if (!b)  detectorTree->Branch ( "hmsDetectorEvent","HMSEvent",&hmsEvent );
       else b->SetAddress(&hmsEvent );
 
       b = detectorTree->GetBranch("beamDetectorEvent");
       if (!b)  detectorTree->Branch ( "beamDetectorEvent","HallCBeamEvent",&beamEvent );
       else b->SetAddress(&beamEvent );
       
       b = detectorTree->GetBranch("monteCarloEvent");
       if (!b)  detectorTree->Branch ( "monteCarloEvent","BETAG4MonteCarloEvent",&mcEvent );
       else b->SetAddress(&mcEvent );
       
    } else { // Make new tree(s) and branch(es)
       detectorTree = new TTree ( "betaDetectors","Detector Tree" );
       detectorTree->Branch ( "betaDetectorEvent","BETAEvent",&betaEvent );
       detectorTree->Branch ( "hmsDetectorEvent","HMSEvent",&hmsEvent );
       detectorTree->Branch ( "beamDetectorEvent","HallCBeamEvent",&beamEvent );
       detectorTree->Branch ( "monteCarloEvent","BETAG4MonteCarloEvent",&mcEvent );
 
    }
 
// Initialize various counters and data
   fakePlaneEventNumber = 0;
   Int_t trackerZero = 0;
   FrontTrackerCellNumber = trackerZero ;
   eCountsThisEvent = 0;
   pmtTotalCount = 0;
   mirrorTotalCount = 0;

// For debugging cherenkov timing emulator
   waveforms = new TH1F(Form("waveform%d",numberOfEvent),"waveform",200,0,20);
   G4double averageTime[12];

/// TODO Implement a print to screen for run info... 

}

//____________________________________________________________________//

BETARun::~BETARun()
{
G4cout << "Writing ROOT File\n";
// Save all objects in this file
detectorTree->Write();
   RootFile->Write();
// Close the file. Note that this is automatically done when you leave
// the application.
 RootFile->Close();
delete betaEvent;
delete hmsEvent;
delete mcEvent;
delete beamEvent;


}

//____________________________________________________________________//
/**
 * NOW ... This might be the most busy function
 * ... probably should be cleaned up
 */
void BETARun::RecordEvent ( const G4Event* anEvent )
{
   G4String colName;
   G4SDManager* SDman = G4SDManager::GetSDMpointer();

   if (construction->usingGasCherenkov) PMTHCID = SDman->GetCollectionID ( colName="PMT/pmt" );
   if (construction->usingGasCherenkov) MirrorHCID = SDman->GetCollectionID ( colName="Mirrors/mirrors" );
   if (construction->usingLuciteHodoscope) hodoscopePMTHCID = SDman->GetCollectionID ( colName="hodoscopePMT/pmt" );
   if (construction->usingBigcal) BIGCALID  = SDman->GetCollectionID ( colName="BIGCALRCS/RCSCalorimeterCollection" );
   if (construction->usingBigcal) BIGCALID2 = SDman->GetCollectionID ( colName="BIGCALBottom/BottomCalorimeterCollection" );
   if (construction->usingForwardTracker)  FTID = SDman->GetCollectionID ( colName="FrontTracker/FrontTracker" );
   if (construction->usingFakePlaneAtBigcal) fakePlaneID = SDman->GetCollectionID ( colName="FakePlaneAtBigcal/FakePlane" );

   G4HCofThisEvent * HCE = anEvent->GetHCofThisEvent();
   hodoscopepmtHC = 0;
   pmtHC = 0;
   BIGCALHC = 0;
   BIGCALHC2 = 0;
   FTHC = 0;
   mirrorHC = 0;
   fakePlaneHC = 0;

   if ( HCE && HCE->GetNumberOfCollections() != 0)
   {
      if (construction->usingLuciteHodoscope) hodoscopepmtHC =(BETAHodoscopePMTHitsCollection* )( HCE->GetHC ( hodoscopePMTHCID ) );
      if (construction->usingGasCherenkov) pmtHC = ( BETAPMTHitsCollection* ) ( HCE->GetHC ( PMTHCID ) );
      if (construction->usingBigcal) BIGCALHC = ( BETARCSCalorimeterHitsCollection* ) ( HCE->GetHC ( BIGCALID ) );
      if (construction->usingBigcal) BIGCALHC2 = ( BETAProtvinoCalorimeterHitsCollection* ) ( HCE->GetHC ( BIGCALID2 ) );
      if (construction->usingForwardTracker) FTHC = ( BETAFrontTrackerHitsCollection* ) ( HCE->GetHC ( FTID ) );
      if (construction->usingGasCherenkov) mirrorHC = ( BETAMirrorHitsCollection* ) ( HCE->GetHC ( MirrorHCID ) );
      if (construction->usingFakePlaneAtBigcal) fakePlaneHC = ( BETAFakePlaneHitsCollection* ) ( HCE->GetHC ( fakePlaneID ) );
   }

/// Initialize counters and data holders
//eCountsThisEvent = 0;
   for (int i=0;i<20;i++) CherenkovPMTCount[i] =0;
   int tdc_count=0;
   int numPMTHits ( 0 ), numPMTHitsAtFace ( 0 ), numMirrorHits ( 0 );
   pmt1Count =0;
   pmt2Count =0;
   pmt3Count =0;
   pmt4Count =0;
   pmt5Count =0;
   pmt6Count =0;
   pmt7Count =0;
   pmt8Count =0;
   mirror1Count =0;
   mirror2Count =0;
   mirror3Count =0;
   mirror4Count =0;
   mirror5Count =0;
   mirror6Count =0;
   mirror7Count =0;
   mirror8Count =0;
   BCTE = 0.0;
   hodoscopePMTcount =0;


    mcEvent->mc_nhit_bcplane = 0;
    mcEvent->mc_e_init[0]= generator->eventEnergy/GeV;
    mcEvent->mc_theta_init[0]=generator->eventTheta;
    mcEvent->mc_phi_init[0]=generator->eventPhi;

//    mcEvent->mc_phi[i1]=generator->phi_particle;



   /****************************************************/
/// \TODO Reimplement fake plane and Montecarlo events
   if (construction->usingFakePlaneAtBigcal && fakePlaneHC && fakePlaneID != -1 ) {
      BETAFakePlaneHit* aHit;
// printf(" Fake Plane Entries : %d",fakePlaneHC->entries() );
      for ( int i1=0;i1<(fakePlaneHC->entries())&&i1<10;i1++ ) {
         aHit = ( *fakePlaneHC ) [i1];
//  if(aHit->energy >0.10) {
// printf(" Fake Plane Entries : %d",fakePlaneHC->entries());
         mcEvent->mc_e_bcplane[i1] = aHit->energy/GeV;
         mcEvent->mc_x_bcplane[i1] = aHit->localPos.y()/cm;
         mcEvent->mc_y_bcplane[i1] = aHit->localPos.x()/cm;
         mcEvent->mc_theta_bcplane[i1] = aHit->worldPos.theta();
         mcEvent->mc_phi_bcplane[i1] = aHit->worldPos.phi();
         mcEvent->mc_pid_bcplane[i1] = aHit->pid;
         mcEvent->mc_nhit_bcplane++;

// }
      }
   }



   triggered = true;

///////////////////////
   if (construction->usingGasCherenkov) if ( pmtHC && PMTHCID != -1)
      {
        eventRecorder->SetGasCherenkovHitCollection(pmtHC);
        eventRecorder->FillGasCherenkovEvent();
      }

   if (triggered) //numPMTHits > 1 )   // 1 P.E.
   {
        if( BIGCALHC && BIGCALHC2   && construction->usingBigcal) {
          eventRecorder->SetBigcalHitCollections(BIGCALHC,BIGCALHC2);
          eventRecorder->FillBigcalEvent();
	}
        if ( FTHC   && FTID != -1 && construction->usingForwardTracker) {
          eventRecorder->SetForwardTrackerHitCollection(FTHC);
          eventRecorder->FillForwardTrackerEvent();
        }

        if ( hodoscopepmtHC && hodoscopePMTHCID!=-1 && construction->usingLuciteHodoscope){
          eventRecorder->SetLuciteHodoscopeHitCollection(hodoscopepmtHC);
          eventRecorder->FillLuciteHodoscopeEvent();
        }
//  Trigger=1; // Record  event
/*
      recordedEvent->cer_hit = tdc_count;
      for (int k=SSSSSSSSSSSSSSSSSSSSSSSSSSSSS0;k<8;k++) {
         recordedEvent->cer_adc[k] = CherenkovPMTCount[k];
         recordedEvent->ceradc_num[k]=k+1;
      }
*/
// RUN TOTALED VALUES
      pmtTotalCount += numPMTHitsAtFace;
      mirrorTotalCount += numMirrorHits;


      /*



      */


//  recordedEvent->BigCalMeanVertical = yAverage/ ( BCTE/MeV );
//  recordedEvent->BigCalMeanHorizontal = xAverage/ ( BCTE/MeV );
//  recordedEvent->BigCalTotalEnergy = BCTE/MeV ;
      /*     G4cout << " ENERGY " << BCTE/MeV << G4endl;*/
//  TBIGCALTotalEnergy->Fill ( BCTE/MeV );
// FILL BIGCAL DATA () FOR HALL C ANALYZER
//DumpHallCMC();



      /*
       if ( analysisManager->plotterVisible() != 0 && ( numberOfEvent%10 ) == 0  ) {
        c1->cd(0);
        TfakePlaneElectronEnergy->Draw();
        c1->cd(1);
        TfakePlanePositronEnergy->Draw();
        c1->Update();
       }
      */
/////////
// Simulated Truths
/////////
//
//recordedEvent->mc_e= generator->momentum;
//recordedEvent->mc_theta=generator->theta_particle;
//recordedEvent->mc_phi=generator->phi_particle;

////////////////////////////////////////
//     DONE grabbing data from detectors
////////////////////////////////////////
      detectorTree->Fill();

      if ( ( numberOfEvent%10 ) == 0 ) G4cout << " Event " << numberOfEvent << G4endl;

      numberOfEvent++;

   } //triggered




}

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

//________________________________________________________________________//
int BETARun::FillGasCherenkovEvent() {
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

  if(analysisManager->GetDetectorVerbosity("GasCherenkov") > 0) 
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

  if(  analysisManager->GetDetectorVerbosity("GasCherenkov") > 0) 
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
}


//________________________________________________________________________//
 int BETARun::FillBigcalEvent() {

return(0);
}



