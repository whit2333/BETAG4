#include "BETAForwardTracker.hh"
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
#include "G4OpticalPhoton.hh"
#include "G4VTouchable.hh"

//____________________________________________________________________________
BETAForwardTracker::BETAForwardTracker(G4String  name):G4VSensitiveDetector(name) {
   G4String HCname;
   detname = name;
   collectionName.insert ( HCname="tracking" );
   HCID = -1;
   fHitsCollection = 0;
}
//_____________________________________________________________________________

BETAForwardTracker::~BETAForwardTracker() {
   //if(fHitsCollection) delete fHitsCollection;
}
//_____________________________________________________________________________

void BETAForwardTracker::Initialize ( G4HCofThisEvent* hitsCollectionOfThisEvent )
{
   
   fHitsCollection =
      new BETAForwardTrackerHitsCollection ( detname, collectionName[0] );
   HCID = G4SDManager::GetSDMpointer()->GetCollectionID ( fHitsCollection );

   // Add collection to the event
   hitsCollectionOfThisEvent->AddHitsCollection ( HCID, fHitsCollection );

   // Initialise hits
   G4int i ( 0 );
}
//______________________________________________________________________________
G4bool BETAForwardTracker::ProcessHits ( G4Step* aStep, G4TouchableHistory* ) {
   G4Track * theTrack = aStep->GetTrack();

   if( theTrack->GetDefinition() == G4OpticalPhoton::OpticalPhotonDefinition() && 
          aStep->GetPreStepPoint()->GetStepStatus() == fGeomBoundary    )
    {

      G4TouchableHandle touch1 = aStep->GetPreStepPoint()->GetTouchableHandle();

//      aName=theTrack->GetVolume()->GetMotherLogical()->GetName();
//      G4cout << " SHOULD REGISTER HIT in physical volume "<< aName << G4endl;

      BETAForwardTrackerHit* aHit = new BETAForwardTrackerHit();
      fHitsCollection->insert ( aHit );
      aHit->cellNumber = touch1->GetCopyNumber(1);

/*      std::cout << "tracker number " << aHit->cellNumber << "\n";*/
      //if(aName=="trackerX1_log") aHit->layerNumber = 1;
      //else if(aName=="trackerY1_log") aHit->layerNumber = 2;
      //else if(aName=="trackerY2_log") aHit->layerNumber = 3;
      aHit->layerNumber = GetScintLayer(aHit->cellNumber);

     /*G4StepPoint* preStepPoint = aStep->GetPreStepPoint();
      G4TouchableHistory* theTouchable
           = ( G4TouchableHistory* ) ( preStepPoint->GetTouchable() );
      G4ThreeVector worldPosition = preStepPoint->GetPosition();

      aHit->localPos = theTouchable->GetHistory()->GetTopTransform().TransformPoint ( worldPosition ) ;
      aHit->worldPos = worldPosition;
      aHit->worldMom = theTrack->GetMomentum();
      */
      aHit->fTiming = theTrack->GetGlobalTime()/ns;
      aHit->fEnergy = theTrack->GetTotalEnergy()/eV;	 
      // Kill the track since we have already counted it. 
      theTrack->SetTrackStatus(fStopAndKill);
   }

   return true;
}

void BETAForwardTracker::EndOfEvent ( G4HCofThisEvent* )
{
//	std::cout << "BETAForwardTracker Hits collection has " << fHitsCollection->entries() << " entries.\n" ;
}

