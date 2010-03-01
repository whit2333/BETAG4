//
// ********************************************************************
// * DISCLAIMER                                                       *
// *                                                                  *
// * The following disclaimer summarizes all the specific disclaimers *
// * of contributors to this software. The specific disclaimers,which *
// * govern, are listed with their locations in:                      *
// *   http://cern.ch/geant4/license                                  *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.                                                             *
// *                                                                  *
// * This  code  implementation is the  intellectual property  of the *
// * GEANT4 collaboration.                                            *
// * By copying,  distributing  or modifying the Program (or any work *
// * based  on  the Program)  you indicate  your  acceptance of  this *
// * statement, and all its terms.                                    *
// ********************************************************************
//
// $Id:$
//
// Jane Tinslay - adapted from A01 example
//
#include "BETAFrontTracker.hh"
#include "G4HCofThisEvent.hh"
// HandsOn5: Hit collection
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
   if ( theTrack->GetDefinition() == G4Electron::ElectronDefinition()  &&
        theTrack->GetNextVolume()->GetName() == "frontTrackerBar" )
   {
      /*G4cout << " SHOULD REGISTER HIT" << G4endl;*/
      BETAFrontTrackerHit* aHit = new BETAFrontTrackerHit();
      fHitsCollection->insert ( aHit );

      aHit->cellNumber = theTrack->GetVolume()->GetCopyNo();

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

