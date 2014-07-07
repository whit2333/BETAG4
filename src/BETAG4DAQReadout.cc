#include "BETAG4DAQReadout.hh"
#include "BETADetectorConstruction.hh"

BETAG4DAQReadout::BETAG4DAQReadout(G4String modName) : G4VDigitizerModule(modName) {

   fCherenkovADCSumDC =
      new BETAG4DigiADCCollection ( this->GetName(), "cerADCSums" );
   fBigcalADCSumDC =
      new BETAG4DigiADCCollection ( this->GetName(), "bigcalADCSums" );

   fSimulationManager = BETASimulationManager::GetInstance();

   mcEvent       = fSimulationManager->fEvents->MC;
   fFT2PlaneHits = mcEvent->GetTrackerPlane2HitsArray();
   fFTPlaneHits  = mcEvent->GetTrackerPlaneHitsArray();
   fBCPlaneHits  = mcEvent->GetBigcalPlaneHitsArray();

   fBigcalHCID           = -1;
   fCherenkovHCID        = -1;
   fTrackerFakePlaneHCID = -1;
   fBigcalFakePlaneHCID  = -1;

   G4SDManager* SDman = G4SDManager::GetSDMpointer();
   G4String colName;

/*  if(fSimulationManager->fConstruction->usingBigcal)*/
   fBigcalHCID  = SDman->GetCollectionID (colName= "BigCal/blockEdep" );
   if(fBigcalHCID == -1) std::cout << " Collection " << colName << "  NOT FOUND!\n";

/*  if(fSimulationManager->fConstruction->usingGasCherenkov)*/
     fCherenkovHCID  = SDman->GetCollectionID (colName= "GasCherenkov/pmt" );
   if(fCherenkovHCID == -1) std::cout << " Collection " << colName << "  NOT FOUND!\n";

   if(fSimulationManager->GetDetectorConstruction()->usingFakePlaneAtBigcal) {
      fBigcalFakePlaneHCID  = SDman->GetCollectionID (colName= "BIGCALPlane/fakePlane" );
      if(fBigcalFakePlaneHCID == -1) std::cout << " Collection " << colName << "  NOT FOUND!\n";
   }

   if(fSimulationManager->GetDetectorConstruction()->usingFakePlaneAtForwardTracker) {
      fTrackerFakePlaneHCID  = SDman->GetCollectionID (colName= "ForwardTrackerPlane/fakePlane" );
      if(fTrackerFakePlaneHCID == -1) std::cout << " Collection " << colName << "  NOT FOUND!\n";

      fTrackerFakePlane2HCID  = SDman->GetCollectionID (colName= "ForwardTrackerPlane2/fakePlane2" );
      if(fTrackerFakePlane2HCID == -1) std::cout << " Collection " << colName << "  NOT FOUND!\n";
   }


   fBigcalTriggerThreshold    = 200.0; //MeV
   fCherenkovTriggerThreshold = 3;//photons???

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
   //std::cout << " fBigcalHCID " << fBigcalHCID << "  fCherenkovHCID " << fCherenkovHCID << "\n";

   /// Loop over bigcal and add up the energy for each trigger group
   BETAG4BigcalHit * bcHit;
   BIGCALGeometryCalculator * bigcalGeoCalc = BIGCALGeometryCalculator::GetCalculator();
   G4double energyTemp;
   fNBigcalHits=0;
   for ( int gg =0;gg<1744;gg++ )
   {
      bcHit      = (*fBigcalHC)[gg];
      energyTemp = bcHit->GetDepositedEnergy();///(bigcalGeoCalc->GetCalibrationCoefficient(gg+1));;

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

   /// Determine if cherenkov would have fired
   fCherenkovFired=false;
   if( fCherenkovTotal > fCherenkovTriggerThreshold ) {
      fCherenkovFired=true;
      /*      std::cout << "Cherenkov Total " << fCherenkovTotal << " \n";*/
   }

   /// Determine if Bigcal would have fired
   fNumberOfTriggeredGroups=0;
   fBigcalFired=false;
   for(int i=0;i<4;i++) {
      //      std::cout << "Bigcal trigger group " << i+1 << " has energy "  << fTriggerGroupEnergy[i] << " MeV \n";
      if( fTriggerGroupEnergy[i] > fBigcalTriggerThreshold ) {
         fBigcalFired=true;
         fNumberOfTriggeredGroups++;
         //         std::cout << "Bigcal Total " << fNumberOfTriggeredGroups << " \n";
      }
   }


   if(fBigcalFired) {
      /// pi0 trigger bits
      fSimulationManager->fEvents->TRIG->fCodaType = 5; // bigcal types
      if( fNumberOfTriggeredGroups > 1) {
         fSimulationManager->fEvents->TRIG->fTriggerBits[2] = true;
         /*       fTriggerEvent->fTriggerBits[3] = true;*/
         fIsTriggered=true;
      }
      /// Beta trigger bits
      if(fCherenkovFired && fBigcalFired ) {
         fSimulationManager->fEvents->TRIG->fCodaType = 5; // bigcal types
         /*         fSimulationManager->fEvents->TRIG->fTriggerBits[1] = true;//b1trig*/
         fSimulationManager->fEvents->TRIG->fTriggerBits[3] = true;//b2trig
         fIsTriggered=true;
      }
   }

}
//__________________________________________________________________

void BETAG4DAQReadout::ReadOut() {
   // Get pointers
   G4String colName;
   //G4SDManager* SDman = G4SDManager::GetSDMpointer();
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

   if(fTrackerFakePlaneHCID != -1) {
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
               bHit->fMomentum.SetXYZ(aHit->fMomentum.x()/(MeV),aHit->fMomentum.y()/(MeV),aHit->fMomentum.z()/(MeV));
               bHit->fTheta = aHit->fPosition.theta();
               bHit->fPhi = aHit->fPosition.phi();

            }
         }
      }

      // Second 
      planeHits = fFT2PlaneHits;
      if(!planeHits) printf(" ERROR NULL mcEvent->fTrackerPlaneHits!\n");
      else {
         if( fTrackerFakePlane2HCID!= -1 ) {
            fTrackerFakePlane2HC = ( BETAFakePlaneHitsCollection* ) ( HCofEvent->GetHC ( fTrackerFakePlane2HCID ) );
            /*    printf(" Tracker Plane Entries : %d",fTrackerFakePlaneHC->entries() );*/
            for ( int i=0;i<(fTrackerFakePlane2HC->entries());i++ ) {
               aHit = ( *fTrackerFakePlane2HC ) [i];

               bHit = new((*planeHits)[i]) InSANEFakePlaneHit();

               bHit->fPID = aHit->fPID;
               bHit->fEnergy = aHit->fEnergy;
               bHit->fLocalPosition.SetXYZ(aHit->fLocalPosition.x()/cm,aHit->fLocalPosition.y()/cm,aHit->fLocalPosition.z()/cm);
               bHit->fPosition.SetXYZ(aHit->fPosition.x()/cm,aHit->fPosition.y()/cm,aHit->fPosition.z()/cm);
               bHit->fMomentum.SetXYZ(aHit->fMomentum.x()/(MeV),aHit->fMomentum.y()/(MeV),aHit->fMomentum.z()/(MeV));
               bHit->fMomentum4Vector.SetXYZT(aHit->fMomentum.x()/(MeV),aHit->fMomentum.y()/(MeV),aHit->fMomentum.z()/(MeV),aHit->fEnergy);
               bHit->fTheta = aHit->fPosition.theta();
               bHit->fPhi = aHit->fPosition.phi();

            }
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
                  aHit->fMomentum.x()/(MeV),
                  aHit->fMomentum.y()/(MeV),
                  aHit->fMomentum.z()/(MeV) );
            bHit->fTheta = aHit->fPosition.theta();
            bHit->fPhi = aHit->fPosition.phi();
         }
      }
   }


}
//__________________________________________________________________


