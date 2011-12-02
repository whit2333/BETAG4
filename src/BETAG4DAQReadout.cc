#include "BETAG4DAQReadout.hh"
#include "BIGCALGeometryCalculator.h"

BETAG4DAQReadout::BETAG4DAQReadout(G4String modName) : G4VDigitizerModule(modName) {

  fCherenkovADCSumDC =
      new BETAG4DigiADCCollection ( this->GetName(), "cerADCSums" );
  fBigcalADCSumDC =
      new BETAG4DigiADCCollection ( this->GetName(), "bigcalADCSums" );
  
  fSimulationManager = 0;
  fSimulationManager = BETASimulationManager::GetInstance();
  mcEvent= fSimulationManager->fEvents->MC;
  fFTPlaneHits=mcEvent->GetTrackerPlaneHitsArray();
  fBCPlaneHits=mcEvent->GetBigcalPlaneHitsArray();
   std::cout << " asdftracker " << mcEvent->GetTrackerPlaneHitsArray() << "\n";
   std::cout << " asdftracker " << mcEvent->GetTrackerPlaneHitsArray() << "\n";
   std::cout << " asdftracker " << mcEvent->GetTrackerPlaneHitsArray() << "\n";


   fBigcalHCID=-1;
   fCherenkovHCID=-1;
   fTrackerFakePlaneHCID=-1;
   fBigcalFakePlaneHCID=-1;

  G4SDManager* SDman = G4SDManager::GetSDMpointer();
  G4String colName;

/*  if(fSimulationManager->fConstruction->usingBigcal)*/
     fBigcalHCID  = SDman->GetCollectionID (colName= "BIGCAL/bigcal" );
  if(fBigcalHCID == -1) std::cout << " Collection " << colName << "  NOT FOUND!\n";

/*  if(fSimulationManager->fConstruction->usingGasCherenkov)*/
     fCherenkovHCID  = SDman->GetCollectionID (colName= "GasCherenkov/pmt" );
  if(fCherenkovHCID == -1) std::cout << " Collection " << colName << "  NOT FOUND!\n";

/*  if(fSimulationManager->fConstruction->usingFakePlaneAtBigcal)*/
    fBigcalFakePlaneHCID  = SDman->GetCollectionID (colName= "ForwardTrackerPlane/fakePlane" );
  if(fBigcalFakePlaneHCID == -1) std::cout << " Collection " << colName << "  NOT FOUND!\n";

/*  if(fSimulationManager->fConstruction->usingFakePlaneAtForwardTracker)*/
    fTrackerFakePlaneHCID  = SDman->GetCollectionID (colName= "BIGCALPlane/fakePlane" );
  if(fTrackerFakePlaneHCID == -1) std::cout << " Collection " << colName << "  NOT FOUND!\n";

  fBigcalTriggerThreshold    = 500.0; //MeV
  fCherenkovTriggerThreshold = 5;//photons???
  

    Reset();
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

  if( fBigcalHCID != -1 ) {
    fBigcalHC = ( BETAG4BigcalHitsCollection* ) ( HCofEvent->GetHC ( fBigcalHCID ) );
  }

  if( fCherenkovHCID != -1 ) {
    fGasCherenkovHC = ( BETAG4PMTHitsCollection* ) ( HCofEvent->GetHC ( fCherenkovHCID ) );
  } else {
     fCherenkovHCID  = SDman->GetCollectionID ( "GasCherenkov/pmt" );
     fGasCherenkovHC = ( BETAG4PMTHitsCollection* ) ( HCofEvent->GetHC ( fCherenkovHCID ) );
  }

//   std::cout << " fBigcalHCID " << fBigcalHCID << "  fCherenkovHCID " << fCherenkovHCID << "\n";

  /// Loop over bigcal and add up the energy for each trigger group
  BETAG4BigcalHit * bcHit;
  BIGCALGeometryCalculator * bigcalGeoCalc = BIGCALGeometryCalculator::GetCalculator();
  G4double energyTemp;
  fNBigcalHits=0;
  for ( int gg =0;gg<1744;gg++ )
  {
    bcHit      = (*fBigcalHC)[gg];
    energyTemp = bcHit->GetDepositedEnergy()/ (bigcalGeoCalc->GetCalibrationCoefficient(bcHit->fCellID));;

    if(energyTemp > 0.01) { ///10 MeV Block Threshold?
      fNBigcalHits++;
      fTriggerGroupEnergy[fSimulationManager->fBigcalDetector->fGeoCalc->GetTriggerGroup(gg+1)-1] += energyTemp;
    }
  }

  fNCherenkovHits = 0;
  /// Loop over gas cherenkov
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
 fBigcalFired=true;

  for(int i=0;i<4;i++) {
    if( fTriggerGroupEnergy[i] > fBigcalTriggerThreshold ) {
      fBigcalFired=true;
      fNumberOfTriggeredGroups++;
    }
  }

  if(fBigcalFired) {

// pi0 trigger bits
     if(fNumberOfTriggeredGroups>1) {
       fSimulationManager->fEvents->TRIG->fCodaType = 5; // bigcal types
       fSimulationManager->fEvents->TRIG->fTriggerBits[2] = true;  
/*       fTriggerEvent->fTriggerBits[3] = true;*/
       fIsTriggered=true;
     }

// Beta trigger bits
    if(fCherenkovFired) {
       fSimulationManager->fEvents->TRIG->fCodaType = 5; // bigcal types
       fSimulationManager->fEvents->TRIG->fTriggerBits[1] = true;//b1trig
       fSimulationManager->fEvents->TRIG->fTriggerBits[3] = true;//b2trig
       fIsTriggered=true;
    }


  }

}
//__________________________________________________________________

void BETAG4DAQReadout::ReadOut() {
// Get pointers
  G4String colName;
  G4SDManager* SDman = G4SDManager::GetSDMpointer();
  G4RunManager* fRM = G4RunManager::GetRunManager();
  const G4Event* currentEvent = fRM->GetCurrentEvent();
  G4HCofThisEvent* HCofEvent = currentEvent->GetHCofThisEvent();

//   BETAG4MonteCarloEvent * mcEvent = fSimulationManager->fEvents->MC;

  BETAFakePlaneHit* aHit;
  TClonesArray * planeHits;
  InSANEFakePlaneHit * bHit;
///////////////////////////////////////////////////////
// // Monte Carlo Thrown Event
//     fSimulationManager->fEvents->MC->fEnergyThrown = fSimulationManager->generator->fCurrentEnergy;
//     fSimulationManager->fEvents->MC->fThetaThrown = generator->fCurrentTheta;
//     fSimulationManager->fEvents->MC->fPhiThrown = generator->fCurrentPhi;
//     fSimulationManager->fEvents->MC->fParticleThrown = 1;
//     fSimulationManager->fEvents->fRunNumber = fSimulationManager->fRunNumber;
///////////////////////////////////////////////////////
/// Forward Tracker Fake plane readout
//   std::cout << " tracker " << mcEvent->GetTrackerPlaneHitsArray() << "\n";

   planeHits = fFTPlaneHits;
   if(!planeHits) printf(" ERROR NULL mcEvent->fTrackerPlaneHits!\n");
   else {
   if( fTrackerFakePlaneHCID!= -1 ) {
     fTrackerFakePlaneHC = ( BETAFakePlaneHitsCollection* ) ( HCofEvent->GetHC ( fTrackerFakePlaneHCID ) );
/*    printf(" Tracker Plane Entries : %d",fTrackerFakePlaneHC->entries() );*/
      for ( int i=0;i<(fTrackerFakePlaneHC->entries());i++ ) {
         aHit = ( *fTrackerFakePlaneHC ) [i];

         bHit = new((*planeHits)[i]) InSANEFakePlaneHit();

         bHit->fPID = aHit->fPID;
         bHit->fEnergy = aHit->fEnergy;
         bHit->fLocalPosition.SetXYZ(aHit->fLocalPosition.x()/cm,aHit->fLocalPosition.y()/cm,aHit->fLocalPosition.z()/cm);
         bHit->fPosition.SetXYZ(aHit->fPosition.x()/cm,aHit->fPosition.y()/cm,aHit->fPosition.z()/cm);
         bHit->fMomentum.SetXYZ(aHit->fMomentum.x()/(MeV/(299792458.0*m/s)),aHit->fMomentum.y()/(MeV/(299792458.0*m/s)),aHit->fMomentum.z()/(MeV/(299792458.0*m/s)));
         bHit->fTheta = aHit->fPosition.theta();
         bHit->fPhi = aHit->fPosition.phi();

// }
      }
   }
   }

///////////////////////////////////////////////////////
/// Bigcal Hit Fake plane readout
   planeHits = fBCPlaneHits;
   if(!planeHits) printf(" ERROR NULL mcEvent->fBigcalPlaneHits!\n");
   else{
   if( fBigcalFakePlaneHCID != -1 ) {
      fBigcalFakePlaneHC = ( BETAFakePlaneHitsCollection* ) ( HCofEvent->GetHC ( fBigcalFakePlaneHCID ) );
/*    printf(" Tracker Plane Entries : %d",fTrackerFakePlaneHC->entries() );*/
      for ( int i=0;i<(fBigcalFakePlaneHC->entries());i++ ) {
         aHit = ( *fBigcalFakePlaneHC ) [i];

         bHit = new((*planeHits)[i]) InSANEFakePlaneHit();
         bHit->fPID = aHit->fPID;
         bHit->fEnergy = aHit->fEnergy;
         bHit->fLocalPosition.SetXYZ(
               aHit->fLocalPosition.x()/cm,
               aHit->fLocalPosition.y()/cm,
               aHit->fLocalPosition.z()/cm );
         bHit->fPosition.SetXYZ(
               aHit->fPosition.x()/cm,
               aHit->fPosition.y()/cm,
               aHit->fPosition.z()/cm  );
         bHit->fMomentum.SetXYZ(
               aHit->fMomentum.x()/(MeV/(299792458.0*m/s)),
               aHit->fMomentum.y()/(MeV/(299792458.0*m/s)),
               aHit->fMomentum.z()/(MeV/(299792458.0*m/s)) );
         bHit->fTheta = aHit->fPosition.theta();
         bHit->fPhi = aHit->fPosition.phi();
      }
  }
  }


}


