#include "BETADigitizer.hh"
#include "G4SDManager.hh"
#include "BETAG4BigcalHit.hh"
#include "BETAG4PMTHit.hh"
#include "BigcalHit.h"
#include "BigcalEvent.h"
#include "BETAEvent.h"
#include "GasCherenkovHit.h"
#include "GasCherenkovEvent.h"
#include "BIGCALGeometryCalculator.h"
#include "ForwardTrackerGeometryCalculator.h"
#include "InSANERunManager.h"



BETADigitizer::BETADigitizer(G4String modName) : G4VDigitizerModule(modName) {
  fSimulationManager=0;
  fRandomNumberGenerator = InSANERunManager::GetRunManager()->GetRandom(); //new TRandom();

  fCherenkovADCDC =
      new BETAG4DigiADCCollection ( this->GetName(), "cerADCs" );
  fCherenkovTDCDC =
      new BETAG4DigiTDCCollection ( this->GetName(), "cerTDCs" );
  fBigcalADCDC =
      new BETAG4DigiADCCollection ( this->GetName(), "bigcalADCs" );
  fBigcalTDCDC =
      new BETAG4DigiTDCCollection ( this->GetName(), "bigcalTDCs" );
  fHodoscopeADCDC =
      new BETAG4DigiADCCollection ( this->GetName(), "hodoscopeADCs" );
  fHodoscopeTDCDC =
      new BETAG4DigiTDCCollection ( this->GetName(), "hodoscopeTDCs" );
  fTrackerADCDC =
      new BETAG4DigiADCCollection ( this->GetName(), "trackerADCs" );
  fTrackerTDCDC =
      new BETAG4DigiTDCCollection ( this->GetName(), "trackerTDCs" );

//   fSimulationManager = BETASimulationManager::GetInstance();
//  fBetaEvent = BETASimulationManager::GetInstance()->fEvents->BETA;

  G4SDManager* SDman = G4SDManager::GetSDMpointer();

/*  if(fSimulationManager->fConstruction->usingBigcal)*/
    fBigcalHCID  = SDman->GetCollectionID ( "BIGCAL/bigcal" );

/*  if(fSimulationManager->fConstruction->usingGasCherenkov)*/
    fCherenkovHCID  = SDman->GetCollectionID ( "GasCherenkov/pmt" );

/*  if(fSimulationManager->fConstruction->usingForwardTracker)*/
    fTrackerHCID  = SDman->GetCollectionID ( "ForwardTracker/tracking" );

/*  if(fSimulationManager->fConstruction->usingLuciteHodoscope)*/
    fHodoscopeHCID  = SDman->GetCollectionID ( "LuciteHodoscope/lpmt" );

  fBigcalChannelThreshold = 5.0; //MeV

}
//_______________________________________________________//

BETADigitizer::~BETADigitizer() {

  if(fCherenkovADCDC) delete fCherenkovADCDC;
  if(fCherenkovTDCDC) delete fCherenkovTDCDC;
  if(fBigcalADCDC) delete fBigcalADCDC;
  if(fBigcalTDCDC) delete fBigcalTDCDC;
  if(fHodoscopeADCDC) delete fHodoscopeADCDC;
  if(fHodoscopeTDCDC) delete fHodoscopeTDCDC;
  if(fTrackerADCDC) delete fTrackerADCDC;
  if(fTrackerTDCDC) delete fTrackerTDCDC;
  if(fRandomNumberGenerator) delete fRandomNumberGenerator;
}
//_______________________________________________________//

void BETADigitizer::Digitize() {
  G4String colName;
   if(!fSimulationManager ) fSimulationManager = BETASimulationManager::GetInstance();

  fBetaEvent = BETASimulationManager::GetInstance()->fEvents->BETA;

  G4SDManager* SDman = G4SDManager::GetSDMpointer();
  G4RunManager* fRM = G4RunManager::GetRunManager();
  const G4Event* currentEvent = fRM->GetCurrentEvent();
  G4HCofThisEvent* HCofEvent = currentEvent->GetHCofThisEvent();

  BETAG4BigcalHit * bcHit;
  BETAG4PMTHit * cerHit;
  BETAHodoscopePMTHit * lucHit;
  BETAFrontTrackerHit * ftHit;

  BETAG4DigiADC * aDigi;
  BETAG4DigiTDC * tDigi;

// Get Hits Collection pointers
  if( fBigcalHCID != -1 ) 
    fBigcalHC = (BETAG4BigcalHitsCollection* ) ( HCofEvent->GetHC ( fBigcalHCID ) );
  if( fCherenkovHCID != -1 ) 
     fGasCherenkovHC   = (BETAG4PMTHitsCollection* ) ( HCofEvent->GetHC ( fCherenkovHCID ) );
//   else  {
//      fCherenkovHCID  = SDman->GetCollectionID ( "GasCherenkov/pmt" );
//      fGasCherenkovHC    = (BETAG4PMTHitsCollection* ) ( HCofEvent->GetHC ( fCherenkovHCID ) );
//   }

  if( fTrackerHCID != -1 )   fForwardTrackerHC  = (BETAFrontTrackerHitsCollection* )( HCofEvent->GetHC ( fTrackerHCID ) );

  BIGCALGeometryCalculator * bigcalGeoCalc = BIGCALGeometryCalculator::GetCalculator();

  /////////////////////////////////
  /// Bigcal Hits
  /////////////////////////////////
  G4double energyTemp = 0.0;
  G4int deltaT = 10;//ns
  tDigi=0;
  int fNADC=0;
  int fNTDC=0;
  
  G4double bigcalTDCChannelScaleFactor = 10.0;
  G4double cherenkovTDCChannelScaleFactor = 10.0;
  
  int currentGroup = 0;
  for ( int gg =0;gg<1744;gg++ )
  {
    bcHit = ( *fBigcalHC)[gg];
    energyTemp = bcHit->GetDepositedEnergy();

    if( ( energyTemp/(bigcalGeoCalc->GetCalibrationCoefficient(gg+1)) ) > fBigcalChannelThreshold )
      { // Record Bigcal Data !
      aDigi = new BETAG4DigiADC(gg+1);
      fNADC++;
      aDigi->fTrueValue = energyTemp;
      /// Divide the energy deposited by the calibration coefficient so that 
      /// in reconstruction, we can multiply by the calibration coefficient.
      aDigi->fADCValue = 
          (float)energyTemp/((float) bigcalGeoCalc->GetCalibrationCoefficient(aDigi->fChannelNumber));
      fBigcalADCDC->insert ( aDigi );


//       std::cout << aDigi->fChannelNumber +1 <<  " has calib coef = " 
//                 << bigcalGeoCalc->GetCalibrationCoefficient(aDigi->fChannelNumber) << "\n";
//       std::cout << " digi value = " << aDigi->fADCValue << " from energy value " << energyTemp << "\n";

// Fill the "TDC hits" if there was a discriminator fire with the time that was recorded
// The hits are summed for the "group of 8" 
// since we loop through all blocks, we just add a hit indexed by group if there was a hit
      if(bcHit->fTimingHit) {
// we create an energy weighted time within some interval deltat
        if(currentGroup==0 || bigcalGeoCalc->GetGroupNumber(gg+1) != currentGroup || bcHit->fTiming - tDigi->fTDCValue > deltaT ) {
          if(tDigi) fBigcalTDCDC->insert ( tDigi );
          currentGroup = bigcalGeoCalc->GetGroupNumber(gg+1);
/*      tDigi = new(fBigcalTDCDC[fNTDC]) BETAG4DigiTDC(currentGroup);*/
          tDigi = new BETAG4DigiTDC(currentGroup);
          tDigi->fADCValue += energyTemp/(bigcalGeoCalc->GetCalibrationCoefficient(aDigi->fChannelNumber));
          tDigi->fTrueValue += bcHit->fTiming;
          tDigi->fNumberOfHits++;
          tDigi->fTDCValue = (bcHit->fTiming*bigcalTDCChannelScaleFactor)/(tDigi->fNumberOfHits);
        } else {
          tDigi->fADCValue += energyTemp/(bigcalGeoCalc->GetCalibrationCoefficient(aDigi->fChannelNumber));
          tDigi->fTrueValue += bcHit->fTiming;
          tDigi->fNumberOfHits++;
          tDigi->fTDCValue = (bcHit->fTiming*bigcalTDCChannelScaleFactor)/(tDigi->fNumberOfHits);
        }
      }
    }
  }
  if(tDigi) if( tDigi->fChannelNumber == (G4int)bigcalGeoCalc->GetGroupNumber(1744) ) fBigcalTDCDC->insert ( tDigi );

// Loop over Cherenkov Hits
  for ( int i=0 ; i < fGasCherenkovHC->entries();i++ ) {
    cerHit =  ( *fGasCherenkovHC )[i];
/*    std::cout << "tube number: " << cerHit->fTubeNumber << "\n";*/
    aDigi = new BETAG4DigiADC(cerHit->fTubeNumber);
    aDigi->fTrueValue = cerHit->GetNumberOfPhotons();
    aDigi->fADCValue = 100.*cerHit->GetNumberOfPhotoElectrons();
    fCherenkovADCDC->insert ( aDigi );
    if(cerHit->fTimingHit) {
      tDigi = new BETAG4DigiTDC(cerHit->fTubeNumber);
      tDigi->fTDCValue = cherenkovTDCChannelScaleFactor*cerHit->fTiming; 
      tDigi->fTrueValue = cerHit->fTiming; 
      tDigi->fADCValue = aDigi->fADCValue;
      tDigi->fNumberOfHits++;
      fCherenkovTDCDC->insert ( tDigi );
    }
  }


  if( fTrackerHCID != -1 ) {
     fLuciteHodoscopeHC = (BETAHodoscopePMTHitsCollection* )( HCofEvent->GetHC ( fHodoscopeHCID ) );
// Loop over Hodoscope Hits
  for ( int i=0 ; i < fLuciteHodoscopeHC->entries();i++ ) {
    lucHit =  ( *fLuciteHodoscopeHC )[i];
    aDigi = new BETAG4DigiADC(lucHit->fTubeNumber);
    aDigi->fTrueValue = lucHit->GetNumberOfPhotons();
    aDigi->fADCValue = 90*0.15*lucHit->GetNumberOfPhotons();
    fHodoscopeADCDC->insert ( aDigi );
    if(lucHit->fTimingHit) {
      tDigi = new BETAG4DigiTDC(lucHit->fTubeNumber);
      tDigi->fTDCValue = cherenkovTDCChannelScaleFactor*lucHit->fTiming; 
      tDigi->fTrueValue = lucHit->fTiming; 
      tDigi->fADCValue = aDigi->fADCValue;
      tDigi->fNumberOfHits++;
      fHodoscopeTDCDC->insert ( tDigi );
    }
  }
  }

// Loop over Tracker Hits
   for ( int i=0 ; i < fForwardTrackerHC->entries();i++ ) {
     ftHit =  ( *fForwardTrackerHC )[i];
//     aDigi = new BETAG4DigiADC(i);
//     aDigi->fTrueValue = ftHit->GetNumberOfPhotons();
//     aDigi->fADCValue = 90*cerHit->GetNumberOfPhotons();
//     fCherenkovADCDC->insert ( aDigi );
//     if(cerHit->fTimingHit) {
       tDigi = new BETAG4DigiTDC(ftHit->cellNumber);
       tDigi->fTrueValue = ftHit->fTiming; 
       tDigi->fChannelNumber = ftHit->cellNumber ;
       fTrackerTDCDC->insert ( tDigi );
//     }
   }

}
//__________________________________________________________________

void BETADigitizer::ReadOut() {
   if(!fSimulationManager ) fSimulationManager = BETASimulationManager::GetInstance();

  BETAG4DigiADC * aDigi;
  BETAG4DigiTDC * tDigi;
  fBetaEvent = BETASimulationManager::GetInstance()->fEvents->BETA;

// Get Digi Collection pointers
//   if( fBigcal != -1 )    fBigcalHC          = (BETAG4BigcalHitsCollection* ) ( HCofEvent->GetHC ( fBigcalHCID ) );
//   if( fCherenkovHCID != -1 ) fGasCherenkovHC    = (BETAG4PMTHitsCollection* ) ( HCofEvent->GetHC ( fCherenkovHCID ) );
//   if( fHodoscopeHCID != -1 ) fLuciteHodoscopeHC = (BETAHodoscopePMTHitsCollection* )( HCofEvent->GetHC ( fHodoscopeHCID ) );
//   if( fTrackerHCID != -1 )   fForwardTrackerHC  = (BETAFrontTrackerHitsCollection* )( HCofEvent->GetHC ( fTrackerHCID ) );
// Loop over Bigcal ADC and TDC Digi collections
// We now make it look like class AnalyzerToInSANE::Process()

////////////////////////
/// FORWARD TRACKER
   ForwardTrackerEvent * ftEvent = fSimulationManager->fEvents->BETA->fForwardTrackerEvent;
   ftEvent->ClearEvent("C");
   TClonesArray  &trackerHits = *(ftEvent->fTrackerHits);
  ftEvent->fNumberOfHits = 0;
  ftEvent->fRunNumber    =  fSimulationManager->GetRunNumber();
  ftEvent->fEventNumber  =  fSimulationManager->GetEventNumber();

   ForwardTrackerHit *aFThit;
   for(int i = 0; i < fTrackerTDCDC->entries() ; i++ ) {
      tDigi = (*fTrackerTDCDC)[i];
      aFThit = new( trackerHits[ftEvent->fNumberOfHits] ) ForwardTrackerHit(); 
      aFThit->fChannel = tDigi->fChannelNumber+1;
      aFThit->fTDC     = tDigi->fTrueValue;
      aFThit->fScintLayer  = ForwardTrackerGeometryCalculator::GetCalculator()->GetLayerNumber(aFThit->fChannel);
      aFThit->fRow  = ForwardTrackerGeometryCalculator::GetCalculator()->GetScintNumber(aFThit->fChannel);
      
//      tDigi->Print();
//      aFThit->Print(); 
      ftEvent->fNumberOfHits++;    
   }  
   // 
/*
   for(i=0;i<y1t_hit;i++) {
     aFThit = new(trackerHits[ftEvent->fNumberOfHits]) ForwardTrackerHit();
     aFThit->fHitNumber          = ftEvent->fNumberOfHits;
     ftEvent->fNumberOfHits++;
     aFThit->fLevel              = 1;
     aFThit->fScintLayer         = 1;
     aFThit->fPositionCoordinate = 2;
     aFThit->fRow                = y1t_row[i];
     aFThit->fTDC                = y1t_tdc[i];
     aFThit->fPosition           = y1t_y[i];

   // // std::cout << "alayer " << aFThit->fScintLayer 
// //           << " row "  << aFThit->fRow 
// //           << " tdc "  << aFThit->fTDC 
// //           << "\n";
   }
   */

   //////////////////////
   /// LUCITE HODOSCOPE
   // only timing information
   LuciteHodoscopeEvent * lhEvent = fSimulationManager->fEvents->BETA->fLuciteHodoscopeEvent;
   Int_t lucpedval = fSimulationManager->fHodoscopeDetector->fTypicalPedestal;
   Int_t luctdcval = fSimulationManager->fHodoscopeDetector->fTypicalTDCPeak ;
   lhEvent->ClearEvent("C");
   TClonesArray &lucHits = *(lhEvent->fLuciteHits);
   LuciteHodoscopeHit * aLUChit;

  lhEvent->fNumberOfHits = 0;
  lhEvent->fRunNumber    =  fSimulationManager->GetRunNumber();
  lhEvent->fEventNumber  =  fSimulationManager->GetEventNumber();

// It was assumed in the Fortran code that both sides would have a hits
// This is probably not a good idea so early
/// \note Lucite indices go from 1 to 56 where 1,3,5,... are positive sides, and 2,4,6... are negative
  for(int i=0;i<fHodoscopeTDCDC->entries();i++) {
    tDigi = (*fHodoscopeTDCDC)[i];
    aLUChit = new(lucHits[lhEvent->fNumberOfHits]) LuciteHodoscopeHit();
    aLUChit->fRow       = (tDigi->fChannelNumber-1)/2 +1;
    aLUChit->fPMTNumber =(tDigi->fChannelNumber);
    aLUChit->fADC       = tDigi->fADCValue + lucpedval +
           fRandomNumberGenerator->Gaus(0,fSimulationManager->fHodoscopeDetector->fTypicalPedestalWidth/2.0); ;
    aLUChit->fTDC       = tDigi->fTDCValue + luctdcval +
           fRandomNumberGenerator->Gaus(0,fSimulationManager->fHodoscopeDetector->fTypicalTDCPeakWidth/2.0);  ;
    aLUChit->fPosition  = 0;
    aLUChit->fLevel     = 1;
    lhEvent->fNumberOfHits++;
//     aLUChit = new(lucHits[lhEvent->fNumberOfHits]) LuciteHodoscopeHit();
//     aLUChit->fRow       = luc_row[i];
//     aLUChit->fPMTNumber = (luc_row[i]-1)*2+2;
//     aLUChit->fADC       = ladc_neg[i];
//     aLUChit->fTDC       = ltdc_neg[i];
//     aLUChit->fPosition  = luc_y[i];
//     aLUChit->fLevel     = 1;
//     lhEvent->fNumberOfHits++;
  }

//////////////////////
/// GAS CHERENKOV
// We count the number of hits by TDC hits
   GasCherenkovEvent * gcEvent = fSimulationManager->fEvents->BETA->fGasCherenkovEvent;
   Int_t cerpedval = fSimulationManager->fCherenkovDetector->fTypicalPedestal;
   Int_t certdcval = fSimulationManager->fCherenkovDetector->fTypicalTDCPeak ;

  gcEvent->ClearEvent("C");
  gcEvent->fNumberOfHits = 0;
  TClonesArray &cherenkovHits = *(gcEvent->fGasCherenkovHits);
  GasCherenkovHit * aCERhit;
//cherenkovHits.Delete();

  gcEvent->fRunNumber    =  fSimulationManager->GetRunNumber();
  gcEvent->fEventNumber  =  fSimulationManager->GetEventNumber();

/** Gas Cherenkov: Inside the tdc hit loop
 *  we add a hit for the cherenkov each time
 *  the mirror numbers are the same. 
 */

  for( int j=0; j<fCherenkovADCDC->entries(); j++ ) {
      aDigi = (*fCherenkovADCDC)[j];
      aCERhit = new(cherenkovHits[gcEvent->fNumberOfHits]) GasCherenkovHit();
      aCERhit->fTDC           = 0;
      aCERhit->fLevel         = 0; /// Level zero is just ADC data
      aCERhit->fADC           = aDigi->fADCValue + cerpedval+
           fRandomNumberGenerator->Gaus(0,fSimulationManager->fCherenkovDetector->fTypicalPedestalWidth/2.0); 
      aCERhit->fHitNumber     = gcEvent->fNumberOfHits;;
      aCERhit->fMirrorNumber  = aDigi->fChannelNumber;
      gcEvent->fNumberOfHits++;
   }

//} else {
for( int i=0; i<fCherenkovTDCDC->entries(); i++ ) { // TDC loop
  for( int j=0; j<fCherenkovADCDC->entries(); j++ ) {
      aDigi = (*fCherenkovADCDC)[j];
      tDigi = (*fCherenkovTDCDC)[i];
    if( aDigi->fChannelNumber == tDigi->fChannelNumber ) {
//       tDigi->Print();

      aCERhit = new(cherenkovHits[gcEvent->fNumberOfHits]) GasCherenkovHit();
      aCERhit->fTDC          = tDigi->fTDCValue + certdcval +
           fRandomNumberGenerator->Gaus(0,fSimulationManager->fCherenkovDetector->fTypicalTDCPeakWidth/2.0); 
      aCERhit->fADC          = aDigi->fADCValue ;
      aCERhit->fLevel        = 1; /// Level 1 is TDC data with ADC data
      aCERhit->fHitNumber    = gcEvent->fNumberOfHits;
      aCERhit->fMirrorNumber = tDigi->fChannelNumber ;
      gcEvent->fNumberOfHits++;
      }
   }
}

  ///////////////////////////
  /// BIGCAL
  ///////////////////////////
  BIGCALGeometryCalculator * bigcalGeoCalc = BIGCALGeometryCalculator::GetCalculator();
  BigcalEvent * bcEvent = fBetaEvent->fBigcalEvent;

  bcEvent->ClearEvent("C");
  BigcalHit * trigGroupBigcalHit[263][128]; // 128 is arbitrary 32*2*2
  Int_t nTrigGrouphits[263];
  for(int i = 0; i<263;i++) nTrigGrouphits[i]=0;

  bcEvent->fRunNumber    =  fSimulationManager->GetRunNumber();
  bcEvent->fEventNumber  =  fSimulationManager->GetEventNumber();

  TClonesArray &BCHits = *(bcEvent->fBigcalHits);
  BigcalHit * aBChit;
  Int_t * rows;
  Int_t grouprowNumber;
  bcEvent->fNumberOfHits = 0;
  bcEvent->fTotalEnergyDeposited = 0.0;
  Int_t lowerLeftGroup=0;

  for(int i = 0; i<fBigcalADCDC->entries() ; i++) {
    aDigi = (*fBigcalADCDC)[i];
//  for(i=0;i<Bigcal_prot_nhit;i++) {
    aBChit = new(BCHits[bcEvent->fNumberOfHits]) BigcalHit();
    bcEvent->fNumberOfHits++;
    aBChit->fiCell = bigcalGeoCalc->GetBlock_i(aDigi->fChannelNumber);
    aBChit->fjCell = bigcalGeoCalc->GetBlock_j(aDigi->fChannelNumber);
    aBChit->fADC   = aDigi->fADCValue + fSimulationManager->fBigcalDetector->fTypicalPedestal+
           fRandomNumberGenerator->Gaus(0,fSimulationManager->fBigcalDetector->fTypicalPedestalWidth/2.0);

/// Group number is numbered such that 1-4 form the bottom row, 5-8 form the next...
    grouprowNumber = bigcalGeoCalc->GetGroupNumber(aBChit->fiCell,aBChit->fjCell);
///  pointers to pointers to hits in trigger group for a given group
///  this is done so that we do not have too loop through again later
///  when going through the tdcs 
    trigGroupBigcalHit[grouprowNumber-1][nTrigGrouphits[grouprowNumber-1]] = aBChit;

/// Group number is numbered such that 1-4 form the bottom row, 5-8 form the next...
    nTrigGrouphits[grouprowNumber-1]++;

//     if( (bcgeo->GetTDCRow(aBChit->fiCell,aBChit->fjCell))[1] != -1 ) { // Block is not only in one TDC row
//       grouprowNumber=((bcgeo->GetTDCRow(aBChit->fiCell,aBChit->fjCell))[1]-1)*4 + bcgeo->GetTDCGroup(aBChit->fiCell,aBChit->fjCell);
//       trigGroupBigcalHit[grouprowNumber-1][nTrigGrouphits[grouprowNumber-1]]=aBChit;
//       nTrigGrouphits[grouprowNumber-1]++;
//      }
    aBChit->fTDC=0;
    aBChit->fTDCRow = (bigcalGeoCalc->GetTDCRow(aBChit->fiCell,aBChit->fjCell))[0];
    aBChit->fTDCGroup = bigcalGeoCalc->GetTDCGroup(aBChit->fiCell,aBChit->fjCell);
    aBChit->fTDCLevel=-1;
    aBChit->fLevel=0;
    aBChit->fEnergy = aDigi->fTrueValue;
//(Float_t)aBChit->fADC*(Float_t)bigcalGeoCalc->GetCalibrationCoefficient(aBChit->fiCell,aBChit->fjCell);
    bcEvent->fTotalEnergyDeposited+=aBChit->fEnergy;
  }

// Now TDCs
  for(int j = 0; j<fBigcalTDCDC->entries() ; j++) {
    tDigi = (*fBigcalTDCDC)[j];
/*    tDigi->Print();*/
/*    for(int j=0;j<Bigcal_tdc_nhit;j++)  {*/
    aBChit = new(BCHits[bcEvent->fNumberOfHits]) BigcalHit();
    bcEvent->fNumberOfHits++;
    aBChit->fADC=0;
    aBChit->fEnergy=0;
    aBChit->fTDC      = tDigi->fTDCValue + fSimulationManager->fBigcalDetector->fTypicalTDCPeak +
           fRandomNumberGenerator->Gaus(0,fSimulationManager->fBigcalDetector->fTypicalTDCPeakWidth/2.0);
    aBChit->fTDCRow   = (tDigi->fChannelNumber-1)/4 +1;
    aBChit->fTDCGroup = (tDigi->fChannelNumber-1)%4 +1;
/*std::cout << " " << tDigi->fChannelNumber << ", " << tDigi->fTDCValue << ", " << tDigi->fADCValue << "\n";*/
//loop over the each trigger group's hits
    for(int i =0;i<nTrigGrouphits[tDigi->fChannelNumber-1];i++) {
      grouprowNumber  = (Int_t)tDigi->fChannelNumber;
      trigGroupBigcalHit[grouprowNumber-1][i]->fTDC       = aBChit->fTDC; 
      trigGroupBigcalHit[grouprowNumber-1][i]->fTDCRow    = (tDigi->fChannelNumber-1)/4 +1;
      trigGroupBigcalHit[grouprowNumber-1][i]->fTDCGroup  = (tDigi->fChannelNumber-1)%4 +1;
      aBChit->fADC    += trigGroupBigcalHit[grouprowNumber-1][i]->fADC;
      aBChit->fEnergy += trigGroupBigcalHit[grouprowNumber-1][i]->fEnergy;
      }
    aBChit->fiCell    = -1;
    aBChit->fjCell    = -1;
    aBChit->fTDCLevel = 1;
    aBChit->fLevel    = 1;
    }

/// \note { BIGCAL_tdc_raw_igroup (see above) is a horizontal grouping of 8 blocks,
/// while the Bigcal_ttrig_igroup below is a vertical index of 1-19
/// group(1-19) Unlike the group for the sum of 8 which goes horizontally!
/// ihalf(1-2) Left-Right
// for( i=0;i<Bigcal_atrig_nhit;i++) 
//  for( j=0;j<Bigcal_ttrig_nhit;j++){
//   if(Bigcal_ttrig_igroup[j] == Bigcal_atrig_igroup[i] 
//      && Bigcal_atrig_ihalf[i] == Bigcal_ttrig_ihalf[j]) {
// 
//     aBChit = new(BCHits[bcEvent->fNumberOfHits]) BigcalHit();
//     bcEvent->fNumberOfHits++;
//        aBChit->fiCell = Bigcal_ttrig_ihalf[j];
//        aBChit->fjCell = Bigcal_ttrig_igroup[j];
//     aBChit->fTDCGroup = Bigcal_atrig_ihalf[i];
//       aBChit->fTDCRow = Bigcal_atrig_igroup[i];
//          aBChit->fADC = Bigcal_atrig_adc_raw[i];
//          aBChit->fTDC = Bigcal_ttrig_tdc_raw[j];
//     aBChit->fLevel=2;
//     aBChit->fTDCLevel = 3;
//       aBChit->fEnergy = 0.0;
//     
// /// The following sums all the energies in the "sum of 8" TDC groups
// /// it grabs the lower left+1 "sum of 8" and steps through the 2(horizontal "sum of 8" groups) by (4 vert) 
// /// And if there is a hit in the group it adds the energy
// ///  \todo A cut needs to be placed here so that tdc hits that pass a timing cut are only added!!!
//     for(int i2=0;i2<2;i2++)for(int j2=0;j2<4;j2++) 
//      if((Bigcal_atrig_igroup[i]-1)*3+1 +j2<57) /*this if prevents the following from looping over rows 57,58 of the top sum of 64 groups which actully have 32-2*2=28 cells */ {
//       lowerLeftGroup =
//       bcgeo->GetTDCRow(
//         (Bigcal_atrig_ihalf[i]-1)*16 +i2*8+2,/*+2, not +1 avoids the imaginary cells in rcs*/
//         (Bigcal_atrig_igroup[i]-1)*3+1 +j2)[0];
//     for(int i3 =0;i3<nTrigGrouphits[lowerLeftGroup];i3++) 
//        aBChit->fEnergy += trigGroupBigcalHit[lowerLeftGroup][i3]->fEnergy;
//     }
// //  std::cout << " bc  half:" << aBChit->fjCell << "\n";
// //  std::cout << " bc group:" << aBChit->fiCell << "\n";
//   }
// }

//     aBigcalHit = new(fBigcalHits[i]) BigcalHit();
// 
//       aBigcalHit->fADC         = aDigi->fADCValue;
//       aBigcalHit->fEnergy      = aDigi->fTrueValue;
//       aBigcalHit->fHitNumber   = i;
//       aBigcalHit->fBlockNumber = aDigi->fChannelNumber;
//       aBigcalHit->fiCell       = fSimulationManager->fBigcalDetector->fGeoCalc->GetBlock_i(aDigi->fChannelNumber);
//       aBigcalHit->fjCell       = fSimulationManager->fBigcalDetector->fGeoCalc->GetBlock_j(aDigi->fChannelNumber);
//       aBigcalHit->fTDCLevel    = 5;
//       aBigcalHit->fTDC         = aDigi->fBlockTime;
// 
// 
//   }
//   aBetaEvent->ClearEvent();


}
//__________________________________________________________________

void BETADigitizer::DigitizePedestals(){
  G4String colName;

  BETAG4BigcalHit     * bcHit;
  BETAG4PMTHit        * cerHit;
  BETAHodoscopePMTHit * lucHit;
  BETAFrontTrackerHit * ftHit;

  BETAG4DigiADC * aDigi;
  BETAG4DigiTDC * tDigi;

/// 
  for ( int gg = 0; gg<1744; gg++ )
  {
      aDigi = new BETAG4DigiADC(gg+1);
      fBigcalADCDC->insert ( aDigi );
  }

  for ( int gg = 0; gg<12; gg++ )
  {
      aDigi = new BETAG4DigiADC(gg+1);
      fCherenkovADCDC->insert ( aDigi );
  }
  for ( int gg = 0; gg<56; gg++ )
  {
      aDigi = new BETAG4DigiADC(gg+1);
      fHodoscopeADCDC->insert ( aDigi );
  }
}
//__________________________________________________________________

void BETADigitizer::Clear() {
   delete fCherenkovADCDC;
   fCherenkovADCDC =
      new BETAG4DigiADCCollection ( this->GetName(), "cerADCs" );
   delete fCherenkovTDCDC;
   fCherenkovTDCDC =
      new BETAG4DigiTDCCollection ( this->GetName(), "cerTDCs" );
   delete fBigcalADCDC;
   fBigcalADCDC =
      new BETAG4DigiADCCollection ( this->GetName(), "bigcalADCs" );
   delete fBigcalTDCDC;
   fBigcalTDCDC =
      new BETAG4DigiTDCCollection ( this->GetName(), "bigcalTDCs" );
   delete fHodoscopeADCDC;
   fHodoscopeADCDC =
      new BETAG4DigiADCCollection ( this->GetName(), "hodoscopeADCs" );
   delete fHodoscopeTDCDC;
   fHodoscopeTDCDC =
      new BETAG4DigiTDCCollection ( this->GetName(), "hodoscopeTDCs" );
   delete fTrackerADCDC;
   fTrackerADCDC =
      new BETAG4DigiADCCollection ( this->GetName(), "trackerADCs" );
    delete fTrackerTDCDC;
  fTrackerTDCDC =
      new BETAG4DigiTDCCollection ( this->GetName(), "trackerTDCs" );
}
//__________________________________________________________________

void BETADigitizer::Print() {
    std::cout << "++ BETA Digitizer ++\n";

    std::cout << "   Bigcal    (" << fBigcalHCID << ")   Entries : " 
              << fBigcalADCDC->entries() << " adc, " 
              << fBigcalTDCDC->entries() << " tdc.\n";
    std::cout << "   Cherenkov (" << fCherenkovHCID << ")  Entries : " 
              << fCherenkovADCDC->entries() << " adc, " 
              << fCherenkovTDCDC->entries() << " tdc.\n";

    std::cout << "   Hodoscope (" << fHodoscopeHCID << ")  Entries : " << fHodoscopeADCDC->entries() << " adc, " << fHodoscopeTDCDC->entries() << " tdc.\n";
    std::cout << "   Tracker   (" << fTrackerHCID << ")  Entries : " << fTrackerADCDC->entries() << " adc, " << fTrackerTDCDC->entries() << " tdc.\n";
}
//__________________________________________________________________


