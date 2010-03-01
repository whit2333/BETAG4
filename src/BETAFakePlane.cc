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
#include "BETAFakePlane.hh"
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
#include "G4PionMinus.hh"
#include "G4PionPlus.hh"
#include "G4Gamma.hh"
#include "G4RandomDirection.hh"
#include "Randomize.hh"
#include "fstream"
#include <gsl/gsl_interp.h>
#include <gsl/gsl_errno.h>
#include <gsl/gsl_spline.h>
// CONSTRUCTOR
BETAFakePlane::BETAFakePlane ( G4String  name )
      :G4VSensitiveDetector ( name )
{
   G4String HCname;
   detname = name;
   collectionName.insert ( HCname="FakePlane" );
   HCID = -1;

}

// DESTRUCTOR
BETAFakePlane::~BETAFakePlane() {}

//////////////////////////////
void BETAFakePlane::Initialize ( G4HCofThisEvent* hitsCollectionOfThisEvent )
{

   fHitsCollection =
      new BETAFakePlaneHitsCollection ( detname, collectionName[0] );
   if ( HCID < 0 )
   {
      HCID = G4SDManager::GetSDMpointer()->GetCollectionID ( fHitsCollection );
   }

   hitsCollectionOfThisEvent->AddHitsCollection ( HCID, fHitsCollection );

}

//////////////////////

G4bool BETAFakePlane::ProcessHits ( G4Step* aStep, G4TouchableHistory* )
{

   G4Track * theTrack = aStep->GetTrack();
   int pid;
   double energy = theTrack->GetTotalEnergy();

   if (energy/MeV >10) {     // 10 MeV is bigcal thrshold

      if ( theTrack->GetDefinition() == G4Electron::ElectronDefinition()  &&
           ( theTrack->GetVolume()->GetName() == "planeBehindTracker_phys"  ) &&       theTrack->GetNextVolume()->GetName() == "BETADetectorphys" )
      {
         pid = 1;
      }
      else if ( theTrack->GetDefinition() == G4Positron::PositronDefinition()  &&
                ( theTrack->GetVolume()->GetName() == "planeBehindTracker_phys"  ) &&
                theTrack->GetNextVolume()->GetName() == "BETADetectorphys" )
      {
         pid = 2;
      }
      else if ( theTrack->GetDefinition() == G4PionPlus::PionPlusDefinition()  &&
                ( theTrack->GetVolume()->GetName() == "planeBehindTracker_phys"  ) &&
                theTrack->GetNextVolume()->GetName() == "BETADetectorphys" )
      {
         pid = 3;
      }
      else if ( theTrack->GetDefinition() == G4PionMinus::PionMinusDefinition()  &&
                ( theTrack->GetVolume()->GetName() == "planeBehindTracker_phys"  ) &&
                theTrack->GetNextVolume()->GetName() == "BETADetectorphys" )
      {
         pid = 4;
      }
      else if ( theTrack->GetDefinition() == G4Gamma::GammaDefinition()  &&
                ( theTrack->GetVolume()->GetName() == "planeBehindTracker_phys"  ) &&
                theTrack->GetNextVolume()->GetName() == "BETADetectorphys" )
      {
         pid = 5;
      } else {
         pid = 99;
      }

      BETAFakePlaneHit* aHit = new BETAFakePlaneHit ( pid );
      fHitsCollection->insert ( aHit );

      G4StepPoint* preStepPoint = aStep->GetPreStepPoint();
      G4TouchableHistory* theTouchable
      = ( G4TouchableHistory* ) ( preStepPoint->GetTouchable() );

      aHit->worldPos = preStepPoint->GetPosition();
      aHit->localPos = theTouchable->GetHistory()->GetTopTransform().TransformPoint ( aHit->worldPos ) ;
      aHit->momentum =  theTrack->GetMomentum();
      aHit->energy     =    theTrack->GetTotalEnergy();

   } // end of energy threshold

   return true;
}

void BETAFakePlane::EndOfEvent ( G4HCofThisEvent* )
{
   ;
}
