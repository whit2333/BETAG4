#include "BETAG4EventRecorder.hh"

#include "ForwardTrackerHit.h"
#include "LuciteHodoscopeHit.h"

/**
 * Function to compare memeber GTime for function sort
 */
bool cmp(const BETAPMTHit *a, const BETAPMTHit *b)
{
return a->Gtime < b->Gtime;
}

//________________________________________________________________________//
BETAG4EventRecorder::BETAG4EventRecorder() {

fAnalysisManager = BETASimulationManager::getInstance ( 0 );


  fGasCherenkovHC = 0;
  fRCSCalorimeterHC = 0;
  fProtvinoCalorimeterHC = 0;
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
}

//________________________________________________________________________//
BETAG4EventRecorder::BETAG4EventRecorder(BETAEvent* ahit1 ,HMSEvent* ahit2, HallCBeamEvent* ahit3, BETAG4MonteCarloEvent* ahit4) {

fAnalysisManager = BETASimulationManager::getInstance ( 0 );

  fGasCherenkovHC = 0;
  fRCSCalorimeterHC = 0;
  fProtvinoCalorimeterHC = 0;

  fBETAEvent = ahit1;//new BETAEvent();
  fHMSEvent = ahit2;// new HMSEvent();
  fHallCBeamEvent = ahit3;// new HallCBeamEvent();
  fBETAG4MonteCarloEvent = ahit4;// new BETAG4MonteCarloEvent();

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

}

//________________________________________________________________________//
BETAG4EventRecorder::~BETAG4EventRecorder() {



}

//________________________________________________________________________//
int BETAG4EventRecorder::FillGasCherenkovEvent( ) {

// For debugging cherenkov timing emulator
//   waveforms = new TH1F(Form("waveform%d",333),"waveform",200,0,20);

  fBETAEvent->fGasCherenkovEvent->fHits->Clear();
  TClonesArray &cherenkovHits = *(fBETAEvent->fGasCherenkovEvent->fHits);
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

//if( fGasCherenkovHC->entries() > 0 ) fGasCherenkovHC->PrintAllHits();
vector<BETAPMTHit*> * hitPointers = fGasCherenkovHC->GetVector();
//vector<BETAPMTHit*>::iterator it;
std::sort (hitPointers->begin(),hitPointers->begin()+fGasCherenkovHC->entries(),cmp);
  // print out content:
  if( fAnalysisManager->GetDetectorVerbosity("GasCherenkov")  > 0) 
    {
    for (  i1=0 ; i1 < fGasCherenkovHC->entries();i1++ )
      {
        std::cout << "hitPointers " << i1 << " contains:";
        std::cout << " " << (*hitPointers)[i1]->Gtime << "\n";
      }
    std::cout << endl;
// end of attempt
    }
// Loop loop through the Hit container once to find the earliest time
    for ( i1=0 ; i1 < fGasCherenkovHC->entries();i1++ )
      { 
      aHit = ( *fGasCherenkovHC )[i1];
        //
        if ( aHit->tubeNumber == -1 )
          {
//        numPMTHitsAtFace++;
          }
        else
          {// normal pmt hit

  if( fAnalysisManager->GetDetectorVerbosity("GasCherenkov")  > 0) 
  {
  std::cout << "Tube " <<  aHit->tubeNumber
            << " hit with global time " << aHit->Gtime/ns
            <<  "ns where the earliest time was " << earliestHitTime[aHit->tubeNumber]
            <<  "\n";
  }

// If the time difference is bigger than 20ns then it would be a new hit
          if( (aHit->Gtime/ns - lastHitTime[aHit->tubeNumber]) > 20.0 )
            {
            lastHitIndex[aHit->tubeNumber] = totalHits ;
            aCERhit = new(cherenkovHits[totalHits]) GasCherenkovHit();
            indTotalHits[aHit->tubeNumber]++;
            aCERhit->fMirrorNumber = aHit->tubeNumber;
            aCERhit->fADC = 0;
            averageTime[aHit->tubeNumber] = 0.0;
            totalHits++;

         if(fAnalysisManager->GetDetectorVerbosity("GasCherenkov") > 0) 
          std::cout << "new hit number " << totalHits << "\n";

            } // end of 
      //    waveforms->Filsl(aHit->Gtime/ns);

          if( aHit->Gtime/ns < earliestHitTime[aHit->tubeNumber] ) earliestHitTime[aHit->tubeNumber]=aHit->Gtime/ns;

          aCERhit = (GasCherenkovHit*)(cherenkovHits)[lastHitIndex[aHit->tubeNumber]];

          (aCERhit[lastHitIndex[aHit->tubeNumber]]).fADC++;
          averageTime[aHit->tubeNumber] += aHit->Gtime/ns;
          aCERhit[lastHitIndex[aHit->tubeNumber]].fTDC =
            averageTime[lastHitIndex[aHit->tubeNumber]]/(aCERhit[lastHitIndex[aHit->tubeNumber]].fADC) ;
          lastHitTime[aHit->tubeNumber] = aHit->Gtime/ns;

          }
      }
//for(int jj =0;jj<totalHits;jj++)
// waveforms->Draw();
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
//return(totalHits);
return(0);
}

/*
void BETAG4EventRecorder::SetEventAddresses() {

}
*/
//________________________________________________________________________//
int BETAG4EventRecorder::FillBigcalEvent() {

  G4double bigcal_block_thresh = 0.01*MeV; // To record a TDC hit energy Deposited must be greater than this energy

  fBETAEvent->fBigcalEvent->fHits->Clear();
  TClonesArray &bigcalHits = *(fBETAEvent->fBigcalEvent->fHits);
  BigcalHit * aBigcalHit;
  BETARCSCalorimeterHit * rcsHit;
  BETAProtvinoCalorimeterHit * protHit;

// locally used variables
  G4double xAverage= 0;
  G4double yAverage=0;
  G4int prot_hits = 0;
  G4int rcs_hits = 0;
/// \todo The following should placed in a Clear or Init method
  fBETAEvent->fBigcalEvent->fTotalEnergyDeposited = 0.0;
  fBETAEvent->fBigcalEvent->fNumberOfHits = 0;
  fBETAEvent->fBigcalEvent->fNumberOfADCHits = 0;
  fBETAEvent->fBigcalEvent->fNumberOfTDCHits = 0;
  fBETAEvent->fBigcalEvent->fNumberOfTimedADCHits = 0;
  fBETAEvent->fBigcalEvent->fNumberOfTimedTDCHits = 0;

  int gg;
  G4double energyTemp;
// Protvino Cells
  for ( gg =0;gg<1024;gg++ )
  {
    protHit = ( *fProtvinoCalorimeterHC) [gg];
    energyTemp = protHit->GetDepositedEnergy();
    if ( energyTemp > bigcal_block_thresh)
    {
      aBigcalHit = new(bigcalHits[prot_hits]) BigcalHit();
      aBigcalHit->fADC  = energyTemp/MeV;
      aBigcalHit->fEnergy = energyTemp/MeV;
      aBigcalHit->fHitNumber = gg+1;
      aBigcalHit->fiCell = (gg)%32 +1;
      aBigcalHit->fjCell = (gg)/32 +1;
      aBigcalHit->fjCell = (gg)/32 +1;
      prot_hits++;
    }
    if ( energyTemp != 0. )
    {
      xAverage += ( ( double ) ( gg%32 ) ) *energyTemp/MeV;
      yAverage += ( ( double ) ( gg/32 ) ) *energyTemp/MeV;
      fBETAEvent->fBigcalEvent->fTotalEnergyDeposited = fBETAEvent->fBigcalEvent->fTotalEnergyDeposited+ energyTemp ;
    }
  }

// RCS
  for ( gg =0;gg<720;gg++ )
  {
    rcsHit = ( *fRCSCalorimeterHC ) [gg];
    energyTemp = rcsHit->GetDepositedEnergy();
    if ( energyTemp > bigcal_block_thresh) {
      aBigcalHit = new(bigcalHits[rcs_hits+prot_hits]) BigcalHit();
      aBigcalHit->fADC  = energyTemp/MeV;
      aBigcalHit->fEnergy = energyTemp/MeV;
      aBigcalHit->fHitNumber = prot_hits+gg+1;
      aBigcalHit->fiCell = (gg)%32 +1+1024;
      aBigcalHit->fjCell = (gg)/32 +1+1024;
      rcs_hits++;
    }
    if ( energyTemp != 0. )
    {
      xAverage += ( ( double ) ( gg%30 +1 ) ) *energyTemp/MeV;
      yAverage += ( ( double ) ( gg / 30 +24 ) ) *energyTemp/MeV;
      fBETAEvent->fBigcalEvent->fTotalEnergyDeposited = fBETAEvent->fBigcalEvent->fTotalEnergyDeposited+ energyTemp ;
    }
  }
  fBETAEvent->fBigcalEvent->fNumberOfHits = prot_hits+rcs_hits;

return(0);
}

//________________________________________________________________________//
int BETAG4EventRecorder::FillForwardTrackerEvent() {
  /////////////////////////////
  // Front Tracker
  fBETAEvent->fForwardTrackerEvent->fHits->Clear();
  TClonesArray &trackerHits = *(fBETAEvent->fForwardTrackerEvent->fHits);
  ForwardTrackerHit * aTrackerHit;

  int gg;
  BETAFrontTrackerHit * trackerHit;
  G4double cosTheta;
  G4ThreeVector Runit;
  G4ThreeVector Punit;
  G4int FrontTrackerCellNumber,TFTcosTheta;
  if ( fForwardTrackerHC->entries() != 0 ) 
  {
    int x= 0, y1= 0, y2= 0;
    for ( gg=0;gg<fForwardTrackerHC->entries();gg++ )
    {
      trackerHit = ( *fForwardTrackerHC ) [gg];
      aTrackerHit = new(trackerHits[gg]) ForwardTrackerHit();
      aTrackerHit->fRow=trackerHit->cellNumber;
      aTrackerHit->fScintLayer=trackerHit->layerNumber;

/*      Runit = trackerHit->worldPos.unit();
      Punit = trackerHit->worldMom.unit();
      cosTheta = ( Runit.x() *Punit.x() ) + ( Runit.y() *Punit.y() ) + ( Runit.z() *Punit.z() ) ;
      if ( ( trackerHit->cellNumber ) <73 ) x = trackerHit->cellNumber;
      if ( ( trackerHit->cellNumber ) >= 73 && ( trackerHit->cellNumber ) < (133+73) ) y1 = trackerHit->cellNumber - 73;
      if ( ( trackerHit->cellNumber ) >= (73+133) ) y2 = trackerHit->cellNumber - (73+133);*/
      // TFTPos->Fill(x,y1);
      // TFTPos->Fill(x,y2);

//          TFTcosTheta->Fill ( cosTheta );
      FrontTrackerCellNumber = trackerHit->cellNumber ;
    }
    G4double smallSeparation = 0.01*mm;

      //   recordedEvent->FTvert1 = (double)(66-y1 ) *3.*mm + 1.5*mm+smallSeparation  ;
      //   recordedEvent->FTvert2 = (double)(66-y2 ) *3.*mm +smallSeparation ;
      //   recordedEvent->FThorz = (double)( 36-x ) *3.*mm + 1.5*mm+smallSeparation ;

 //        TFTPos->Fill ( x,y1 );
      // TFTPos->Fill(x,y2);

        }

return(0);
}

//________________________________________________________________________//
int BETAG4EventRecorder::FillLuciteHodoscopeEvent(){

////////////////////////////////
// Lucite Hodoscope
  BETAHodoscopePMTHit* aHodoscopeHit;

//    ThodoscopepmtSumHits->Fill ( hodoscopepmtHC->entries() );
    //  recordedEvent->HodoscopeTotalPE =  hodoscopepmtHC->entries();
return(0);
}




