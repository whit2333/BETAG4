#include "BETAG4BigcalSD.hh"
#include "G4HCofThisEvent.hh"
#include "G4SDManager.hh"
#include "G4Step.hh"
#include "G4TouchableHistory.hh"
#include "G4Track.hh"

//______________________________________________________________________________
BETAG4BigcalSD::BETAG4BigcalSD(const G4String& name):G4VSensitiveDetector(name){
   collectionName.insert ("blockEdep" );
   collectionName.insert ("blockTimingGroup" );
   collectionName.insert ("blockTriggerGroup" );

   fHitsCollectionID = -1;
   fTimingHCID       = -1;
   fTriggerHCID      = -1;

   fHitsCollection = 0;
   fTimingHC       = 0; 
   fTriggerHC      = 0;
   fGeoCalc = BIGCALGeometryCalculator::GetCalculator();
}
//______________________________________________________________________________
BETAG4BigcalSD::~BETAG4BigcalSD() { 
}
//______________________________________________________________________________
void BETAG4BigcalSD::Initialize(G4HCofThisEvent* hitsCollectionOfThisEvent){

   // Create new collections
   fHitsCollection =
      new BETAG4BigcalHitsCollection ( SensitiveDetectorName, collectionName[0] );
   if ( fHitsCollectionID < 0 )
      fHitsCollectionID = G4SDManager::GetSDMpointer()->GetCollectionID ( fHitsCollection );
   hitsCollectionOfThisEvent->AddHitsCollection ( fHitsCollectionID, fHitsCollection );

   fTimingHC =
      new BETAG4BigcalHitsCollection ( SensitiveDetectorName, collectionName[1] );
   if ( fTimingHCID < 0 )
      fTimingHCID = G4SDManager::GetSDMpointer()->GetCollectionID ( fTimingHC );
   hitsCollectionOfThisEvent->AddHitsCollection ( fTimingHCID, fTimingHC );

   fTriggerHC =
      new BETAG4BigcalHitsCollection ( SensitiveDetectorName, collectionName[1] );
   if ( fTriggerHCID < 0 )
      fTriggerHCID = G4SDManager::GetSDMpointer()->GetCollectionID ( fTriggerHC );
   hitsCollectionOfThisEvent->AddHitsCollection ( fTriggerHCID, fTriggerHC );

   // Initialise hits
   // For each block
   for( int i=0; i<1744; i++ ) {
      BETAG4BigcalHit* aHit = new BETAG4BigcalHit ( i+1 );
      fHitsCollection->insert ( aHit );
   }

   // For the smallest timing groups, sums of 8 (4 columns x 56 rows = 224 groups)
   for( int i=0; i< 56*4 ; i++ ) {
      BETAG4BigcalHit* aHit = new BETAG4BigcalHit ( i+1);
      aHit->fType = 1;
      fTimingHC->insert ( aHit );
   }

   // For the trigger subgroups (sums of 64)
   for( int i=0; i<19*2; i++ ) {
      BETAG4BigcalHit* aHit = new BETAG4BigcalHit ( i+1 );
      aHit->fType = 2;
      fTriggerHC->insert ( aHit );
   }
}
//______________________________________________________________________________
G4bool BETAG4BigcalSD::ProcessHits ( G4Step* aStep, G4TouchableHistory* ) {

   G4Track * theTrack = aStep->GetTrack();

   // Get energy deposited in this step
   G4double depositedEnergy = aStep->GetTotalEnergyDeposit();
   if( 0 == depositedEnergy ) return true;

   // Get volume and copy number
   G4StepPoint* preStepPoint        = aStep->GetPreStepPoint();
   G4TouchableHistory* theTouchable = ( G4TouchableHistory* ) ( preStepPoint->GetTouchable() );

   G4VPhysicalVolume* thePhysical   = theTouchable->GetVolume();
   G4int copyNo                     = thePhysical->GetCopyNo();
   G4int blockNo                    = copyNo+1;

   int timingGroupNo    = fGeoCalc->GetGroupNumber(blockNo);
   int triggerGroupNo   = fGeoCalc->GetSumOf64GroupNumber(blockNo)[0];
   int triggerGroupNo2  = fGeoCalc->GetSumOf64GroupNumber(blockNo)[1];

   //std::cout << "timingGroupNo  = " << timingGroupNo << std::endl;
   // Get corresponding hit
   BETAG4BigcalHit* aHit = ( *fHitsCollection ) [copyNo];
   BETAG4BigcalHit* bHit = ( *fTimingHC ) [timingGroupNo-1];
   BETAG4BigcalHit* cHit = ( *fTriggerHC ) [triggerGroupNo-1];
   BETAG4BigcalHit* dHit = 0;
   if(triggerGroupNo2 != -1) dHit = ( *fTriggerHC ) [triggerGroupNo2-1];

   // Check to see if this is the first time the hit has been updated
   if ( ! ( aHit->GetLogicalVolume() ) )
   {
      // Set volume information
      aHit->SetLogicalVolume ( thePhysical->GetLogicalVolume() );
      G4AffineTransform aTrans = theTouchable->GetHistory()->GetTopTransform();
      aTrans.Invert();
      aHit->fNHits++;
      aHit->SetRotation ( aTrans.NetRotation() );
      aHit->SetPosition ( aTrans.NetTranslation() );
   }
   if ( ! ( bHit->GetLogicalVolume() ) )
   {
      // Set volume information
      bHit->SetLogicalVolume ( thePhysical->GetLogicalVolume() );
      G4AffineTransform aTrans = theTouchable->GetHistory()->GetTopTransform();
      aTrans.Invert();
      bHit->fNHits++;
      bHit->SetRotation ( aTrans.NetRotation() );
      bHit->SetPosition ( aTrans.NetTranslation() );
   }
   if ( ! ( cHit->GetLogicalVolume() ) )
   {
      // Set volume information
      cHit->SetLogicalVolume ( thePhysical->GetLogicalVolume() );
      G4AffineTransform aTrans = theTouchable->GetHistory()->GetTopTransform();
      aTrans.Invert();
      cHit->fNHits++;
      cHit->SetRotation ( aTrans.NetRotation() );
      cHit->SetPosition ( aTrans.NetTranslation() );
   }
   if(dHit) if ( ! ( dHit->GetLogicalVolume() ) )
   {
      // Set volume information
      dHit->SetLogicalVolume ( thePhysical->GetLogicalVolume() );
      G4AffineTransform aTrans = theTouchable->GetHistory()->GetTopTransform();
      aTrans.Invert();
      dHit->fNHits++;
      dHit->SetRotation ( aTrans.NetRotation() );
      dHit->SetPosition ( aTrans.NetTranslation() );
   }

   // Accumulate energy deposition
   aHit->AddDepositedEnergy ( depositedEnergy/MeV );
   bHit->AddDepositedEnergy ( depositedEnergy/MeV );
   cHit->AddDepositedEnergy ( depositedEnergy/MeV );
   if(dHit) dHit->AddDepositedEnergy ( depositedEnergy/MeV );

   // if the energy is above the threshold to fire a discriminator (10MeV)
   // and has not had a time recorded, then record the time
   if( aHit->GetDepositedEnergy() > 10.0 && (!aHit->fTimingHit) ) {
     aHit->fTiming = theTrack->GetGlobalTime()/ns;
     aHit->fTimingHit = true;
   }

   if( (!bHit->fTimingHit) && (bHit->GetDepositedEnergy() > 100.0)  ) {
     bHit->fTiming = theTrack->GetGlobalTime()/ns;
     bHit->fTimingHit = true;
   }

   if( (!cHit->fTimingHit) && (cHit->GetDepositedEnergy() > 500.0)  ) {
     cHit->fTiming = theTrack->GetGlobalTime()/ns;
     cHit->fTimingHit = true;
   }
   if(dHit) if( (!dHit->fTimingHit) && (dHit->GetDepositedEnergy() > 500.0)  ) {
     dHit->fTiming = theTrack->GetGlobalTime()/ns;
     dHit->fTimingHit = true;
   }

   return true;
}
//______________________________________________________________________________
void BETAG4BigcalSD::EndOfEvent ( G4HCofThisEvent* ) {
}
//______________________________________________________________________________

