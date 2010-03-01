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
#include "BETAMirror.hh"
#include "G4HCofThisEvent.hh"
// HandsOn5: Hit collection
#include "G4SDManager.hh"
#include "G4Step.hh"
#include "G4TouchableHistory.hh"
#include "G4Track.hh"
#include "G4ParticleDefinition.hh"
#include "G4OpticalPhoton.hh"
#include "G4Electron.hh"
#include "G4Positron.hh"
#include "G4RandomDirection.hh"
#include "Randomize.hh"
#include "fstream"
#include <gsl/gsl_interp.h>
#include <gsl/gsl_errno.h>
#include <gsl/gsl_spline.h>
// CONSTRUCTOR
BETAMirror::BETAMirror ( G4String  name )
      :G4VSensitiveDetector ( name )
{
   G4String HCname;
   detname = name;
   collectionName.insert ( HCname="mirrors" );
   HCID = -1;

}

// DESTRUCTOR
BETAMirror::~BETAMirror() {}

//////////////////////////////
void BETAMirror::Initialize ( G4HCofThisEvent* hitsCollectionOfThisEvent )
{

   // HandsOn5: Creating hit collection
   // Create a new collection

////////////////////////////
   fHitsCollection =
      new BETAMirrorHitsCollection ( detname, collectionName[0] );
   if ( HCID < 0 )
   {
      HCID = G4SDManager::GetSDMpointer()->GetCollectionID ( fHitsCollection );
   }

   // Add collection to the event
   hitsCollectionOfThisEvent->AddHitsCollection ( HCID, fHitsCollection );

}

//////////////////////

G4bool BETAMirror::ProcessHits ( G4Step* aStep, G4TouchableHistory* )
{
   G4Track * theTrack = aStep->GetTrack();


/// COUNT The number of reflected photons
///////////////////////////////////////////////////////////////////////////////
   if ( theTrack->GetDefinition() == G4OpticalPhoton::OpticalPhotonDefinition()  &&
        ( theTrack->GetVolume()->GetName() == "Far - Mirror" ||  theTrack->GetVolume()->GetName() == "Near - Mirror" ) &&
        theTrack->GetNextVolume()->GetName() == "tank_phys" )
   {
      // Get Mirror #
      mirror = theTrack->GetVolume()->GetCopyNo();
//G4cout << "       TEST    "<< mirror << G4endl;

      BETAMirrorHit* aHit = new BETAMirrorHit ( mirror );
      fHitsCollection->insert ( aHit );

      // Get position
      G4StepPoint* preStepPoint = aStep->GetPreStepPoint();
      G4TouchableHistory* theTouchable
      = ( G4TouchableHistory* ) ( preStepPoint->GetTouchable() );
      G4ThreeVector worldPos = preStepPoint->GetPosition();
      aHit->localPos = theTouchable->GetHistory()->GetTopTransform
                       ().TransformPoint ( worldPos ) ;
   }
   if ( theTrack->GetDefinition() == G4Electron::ElectronDefinition()  &&
        ( theTrack->GetVolume()->GetName() == "Far - Mirror" ||  theTrack->GetVolume()->GetName() == "Near - Mirror" ) &&
        theTrack->GetNextVolume()->GetName() == "tank_phys" )
   {
      // Get Mirror #
      mirror = theTrack->GetVolume()->GetCopyNo();
//G4cout << "       TEST    "<< mirror << G4endl;

      BETAMirrorHit* aHit = new BETAMirrorHit ( mirror+8 );
      fHitsCollection->insert ( aHit );

      // Get position
      G4StepPoint* preStepPoint = aStep->GetPreStepPoint();
      G4TouchableHistory* theTouchable
      = ( G4TouchableHistory* ) ( preStepPoint->GetTouchable() );
      G4ThreeVector worldPos = preStepPoint->GetPosition();
      aHit->localPos = theTouchable->GetHistory()->GetTopTransform
                       ().TransformPoint ( worldPos ) ;
   }




   return true;
}

void BETAMirror::EndOfEvent ( G4HCofThisEvent* )
{
   ;
}
