#include "BETAG4DAQReadout.hh"

BETAG4DAQReadout::BETAG4DAQReadout(G4String modName) : G4VDigitizerModule(modName) {

  fCherenkovADCSumDC =
      new BETAG4DigiADCCollection ( this->GetName(), "cerADCSums" );
  fBigcalADCSumDC =
      new BETAG4DigiADCCollection ( this->GetName(), "bigcalADCSums" );

  fSimulationManager = BETASimulationManager::GetInstance();

  G4SDManager* SDman = G4SDManager::GetSDMpointer();
  if(fSimulationManager->fConstruction->usingBigcal)
    fBigcalHCID  = SDman->GetCollectionID ( "BIGCAL/bigcal" );
  if(fSimulationManager->fConstruction->usingGasCherenkov)
    fCherenkovHCID  = SDman->GetCollectionID ( "GasCherenkov/pmt" );

  fBigcalTriggerThreshold    = 500.0; //MeV
  fCherenkovTriggerThreshold = 5;//photons???

}
//__________________________________________________________________

BETAG4DAQReadout::~BETAG4DAQReadout() {

  delete fCherenkovADCSumDC;
  delete fBigcalADCSumDC;

}
//__________________________________________________________________

void BETAG4DAQReadout::Digitize() {

// Reset all event level values

// Get pointers
  G4String colName;
  G4SDManager* SDman = G4SDManager::GetSDMpointer();
  G4RunManager* fRM = G4RunManager::GetRunManager();
  const G4Event* currentEvent = fRM->GetCurrentEvent();
  G4HCofThisEvent* HCofEvent = currentEvent->GetHCofThisEvent();



  if(fSimulationManager->fConstruction->usingBigcal) {
    fBigcalHC = ( BETAG4BigcalHitsCollection* ) ( HCofEvent->GetHC ( fBigcalHCID ) );
  }

  if(fSimulationManager->fConstruction->usingGasCherenkov) {
    fGasCherenkovHC = ( BETAG4PMTHitsCollection* ) ( HCofEvent->GetHC ( fCherenkovHCID ) );
  }

// Loop over bigcal and add up the energy for each trigger group
  BETAG4BigcalHit * bcHit;
  G4double energyTemp;
  fNBigcalHits=0;
  for ( int gg =0;gg<1744;gg++ )
  {
    bcHit = ( *fBigcalHC)[gg];
    energyTemp = bcHit->GetDepositedEnergy();

    if(energyTemp > 0.01){ ///10 MeV Block Threshold?
      fNBigcalHits++;
      fTriggerGroupEnergy[fSimulationManager->fBigcalDetector->fGeoCalc->GetTriggerGroup(gg+1)-1] += energyTemp;
    }

  }

  fNCherenkovHits=0;
// Loop over gas cherenkov
  BETAG4PMTHit * cerHit;
  for ( int i1=0 ; i1 < fGasCherenkovHC->entries();i1++ ) {
    cerHit = ( *fGasCherenkovHC )[i1];
    fCherenkovTotal += (G4int)(cerHit->GetNumberOfPhotoElectrons());
      if(cerHit->GetNumberOfPhotoElectrons()>2) fNCherenkovHits++;
  }
  if(fCherenkovTotal>fCherenkovTriggerThreshold) {
    fCherenkovFired=true;
  }
//    std::cout << "Cherenkov Total " << fCherenkovTotal << " \n";

  for(int i=0;i<4;i++) { 
    if( fTriggerGroupEnergy[i] > fBigcalTriggerThreshold ) {
      fBigcalFired=true;
      fNumberOfTriggeredGroups++;
    }
  }

  if(fBigcalFired) {

// pi0 trigger bits
     if(fNumberOfTriggeredGroups>1) {
       fTriggerEvent->fCodaType = 5; // bigcal types

       fTriggerEvent->fTriggerBits[2] = true;
       fIsTriggered=true;
     }

// Beta trigger bits
    if(fCherenkovFired) {
       fTriggerEvent->fCodaType = 5; // bigcal types
       fTriggerEvent->fTriggerBits[1] = true;//b1trig
       fTriggerEvent->fTriggerBits[3] = true;//b2trig
       fIsTriggered=true;
    }
//others...
  }

}
//__________________________________________________________________

void BETAG4DAQReadout::ReadOut() {
   

}
//     if ( energyTemp > bigcal_block_thresh)
//     {
//       aBigcalHit = new(bigcalHits[prot_hits]) BigcalHit();
//       aBigcalHit->fADC  = 10.0*energyTemp/MeV;
//       aBigcalHit->fEnergy = energyTemp/MeV;
//       aBigcalHit->fHitNumber = prot_hits+1;
//       aBigcalHit->fiCell = (gg)%32 +1;
//       aBigcalHit->fjCell = (gg)/32 +1;
//       aBigcalHit->fTDCLevel=-1;
//       prot_hits++;
//     }
//     if ( energyTemp != 0. )
//     {
//       xAverage += ( ( double ) ( gg%32 ) ) *energyTemp/MeV;
//       yAverage += ( ( double ) ( gg/32 ) ) *energyTemp/MeV;
//       fBETAEvent->fBigcalEvent->fTotalEnergyDeposited = fBETAEvent->fBigcalEvent->fTotalEnergyDeposited+ energyTemp ;
//     }

