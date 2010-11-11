#include "BETAFrontTracker.hh"
#include "G4HCofThisEvent.hh"

#include "G4SDManager.hh"
#include "G4Step.hh"
#include "G4TouchableHistory.hh"
#include "G4Track.hh"
#include "G4ParticleDefinition.hh"
#include "G4Electron.hh"
#include "G4RandomDirection.hh"
#include "Randomize.hh"
#include "fstream"

// CONSTRUCTOR
BETAFrontTracker::BETAFrontTracker ( G4String  name )
      :G4VSensitiveDetector ( name )
{
   G4String HCname;
   detname = name;
   collectionName.insert ( HCname="FrontTracker" );
   HCID = -1;

}

// DESTRUCTOR
BETAFrontTracker::~BETAFrontTracker() {
   ;
}


//////////////////////////////
void BETAFrontTracker::Initialize ( G4HCofThisEvent* hitsCollectionOfThisEvent )
{

   // HandsOn5: Creating hit collection
   // Create a new collection

////////////////////////////
   fHitsCollection =
      new BETAFrontTrackerHitsCollection ( detname, collectionName[0] );
   if ( HCID < 0 )
   {
      HCID = G4SDManager::GetSDMpointer()->GetCollectionID ( fHitsCollection );
   }

   // Add collection to the event
   hitsCollectionOfThisEvent->AddHitsCollection ( HCID, fHitsCollection );

   // Initialise hits
   G4int i ( 0 );

// 8 PMTs
   /*  for (i=0; i<8; i++) {
       BETAPMTHit* aHit = new BETAPMTHit(i);
       fHitsCollection->insert(aHit);
     }*/
///////////////////////////

}
//////////////////////

G4bool BETAFrontTracker::ProcessHits ( G4Step* aStep, G4TouchableHistory* )
{
   G4Track * theTrack = aStep->GetTrack();
///////////////////////////////////////////////////////////////////////////////
/*theTrack->GetDefinition() == G4Electron::ElectronDefinition() */
    G4String aName;
    if( (theTrack->GetVolume()->GetLogicalVolume()->GetName() == "horizBar_log" ||
        theTrack->GetVolume()->GetLogicalVolume()->GetName() == "vertBar_log" )&&(
        theTrack->GetNextVolume()->GetLogicalVolume()->GetName() == "horizBarScore_log" ||
        theTrack->GetNextVolume()->GetLogicalVolume()->GetName() == "vertBarScore_log" ))
//trackerY1_phys
    {
      aName=theTrack->GetVolume()->GetMotherLogical()->GetName();
//      G4cout << " SHOULD REGISTER HIT in physical volume "<< aName << G4endl;
      BETAFrontTrackerHit* aHit = new BETAFrontTrackerHit();
      fHitsCollection->insert ( aHit );
      aHit->cellNumber = theTrack->GetVolume()->GetCopyNo();
      if(aName=="trackerX1_log") aHit->layerNumber = 1;
      else if(aName=="trackerY1_log") aHit->layerNumber = 2;
      else if(aName=="trackerY2_log") aHit->layerNumber = 3;
      G4StepPoint* preStepPoint = aStep->GetPreStepPoint();
      G4TouchableHistory* theTouchable
      = ( G4TouchableHistory* ) ( preStepPoint->GetTouchable() );
      G4ThreeVector worldPosition = preStepPoint->GetPosition();
      aHit->localPos = theTouchable->GetHistory()->GetTopTransform().TransformPoint ( worldPosition ) ;
      aHit->worldPos = worldPosition;
      aHit->worldMom = theTrack->GetMomentum();

   }

   return true;
}

void BETAFrontTracker::EndOfEvent ( G4HCofThisEvent* )
{
   ;
}

