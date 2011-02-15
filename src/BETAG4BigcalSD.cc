#include "BETAG4BigcalSD.hh"
#include "G4HCofThisEvent.hh"
#include "G4SDManager.hh"
#include "G4Step.hh"
#include "G4TouchableHistory.hh"
#include "G4Track.hh"
//_______________________________________________________//

BETAG4BigcalSD::BETAG4BigcalSD ( const G4String& name )
      : G4VSensitiveDetector ( name )
{
   collectionName.insert ( "bigcal" );
   fHitsCollectionID = -1;
}
//_______________________________________________________//

BETAG4BigcalSD::~BETAG4BigcalSD() { 

}
//_______________________________________________________//

void BETAG4BigcalSD::Initialize ( G4HCofThisEvent* hitsCollectionOfThisEvent )
{
   // Create a new collection
   fHitsCollection =
      new BETAG4BigcalHitsCollection ( SensitiveDetectorName, collectionName[0] );

   if ( fHitsCollectionID < 0 )
      fHitsCollectionID = G4SDManager::GetSDMpointer()->GetCollectionID ( fHitsCollection );

   // Add collection to the event
   hitsCollectionOfThisEvent->AddHitsCollection ( fHitsCollectionID, fHitsCollection );

   // Initialise hits
   G4int i ( 0 );

   for ( i=0; i<1744; i++ )
   {
      BETAG4BigcalHit* aHit = new BETAG4BigcalHit ( i );
      fHitsCollection->insert ( aHit );
   }
}
//_______________________________________________________//

G4bool BETAG4BigcalSD::ProcessHits ( G4Step* aStep, G4TouchableHistory* )
{

   G4Track * theTrack = aStep->GetTrack();

   // Get energy deposited in this step
   G4double depositedEnergy = aStep->GetTotalEnergyDeposit();
   if ( 0 == depositedEnergy ) return true;


   // Get volume and copy number
   G4StepPoint* preStepPoint = aStep->GetPreStepPoint();
   G4TouchableHistory* theTouchable = ( G4TouchableHistory* ) ( preStepPoint->GetTouchable() );

   G4VPhysicalVolume* thePhysical = theTouchable->GetVolume();
   G4int copyNo = thePhysical->GetCopyNo();

   // Get corresponding hit
   BETAG4BigcalHit* aHit = ( *fHitsCollection ) [copyNo];

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

   // Accumulate energy deposition
   aHit->AddDepositedEnergy ( depositedEnergy/MeV );
   // if the energy is above the threshold to fire a discriminator (10MeV)
   // and has not had a time recorded, then record the time
   if( aHit->GetDepositedEnergy() > 10.0 && (!aHit->fTimingHit) ) {
     aHit->fTiming = theTrack->GetGlobalTime()/ns;
     aHit->fTimingHit = true;
   }

   return true;
}
//_______________________________________________________//

void BETAG4BigcalSD::EndOfEvent ( G4HCofThisEvent* ) {


}
//_______________________________________________________//
