#include "BETAG4EventRecorder.hh"


/**
 * Function to compare memeber GTime for function sort
 */
bool cmp(const BETAPMTHit *a, const BETAPMTHit *b)
{
return a->Gtime < b->Gtime;
}


BETAG4EventRecorder::BETAG4EventRecorder() {

fAnalysisManager = BETAAnalysisManager::getInstance ( 0 );


  fGasCherenkovHC = 0;
  fRCSCalorimeterHC = 0;
  fProtvinoCalorimeterHC = 0;

}

BETAG4EventRecorder::BETAG4EventRecorder(BETAEvent* ahit1 ,HMSEvent* ahit2, HallCBeamEvent* ahit3, BETAG4MonteCarloEvent* ahit4) {

  fGasCherenkovHC = 0;
  fRCSCalorimeterHC = 0;
  fProtvinoCalorimeterHC = 0;

   fBETAEvent = new BETAEvent();
   fHMSEvent= new HMSEvent();
   fHallCBeamEvent = new HallCBeamEvent();
   fBETAG4MonteCarloEvent = new BETAG4MonteCarloEvent();

}

BETAG4EventRecorder::~BETAG4EventRecorder() {



}

void BETAG4EventRecorder::FillGasCherenkovEvent(  BETAPMTHitsCollection * pmtHC) {


  fBETAEvent->fGasCherenkovEvent->fHits->Clear("C");
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

if( pmtHC->entries() > 0 ) pmtHC->PrintAllHits();
vector<BETAPMTHit*> * hitPointers = pmtHC->GetVector();
//vector<BETAPMTHit*>::iterator it;
std::sort (hitPointers->begin(),hitPointers->begin()+pmtHC->entries(),cmp);
  // print out content:
    for (  i1=0 ; i1 < pmtHC->entries();i1++ )
    {
       std::cout << "hitPointers " << i1 << " contains:";
       std::cout << " " << (*hitPointers)[i1]->Gtime << "\n";
    }

  std::cout << endl;
// end of attempt

// Loop loop through the Hit container once to find the earliest time
    for ( i1=0 ; i1 < pmtHC->entries();i1++ )
      { 
      aHit = ( *pmtHC )[i1];
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
//          waveforms->Fill(aHit->Gtime/ns);

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
//return(totalHits);

}


/*
void BETAG4EventRecorder::SetEventAddresses() {

}

void BETAG4EventRecorder::FillBigcalEvent() {


}
*/