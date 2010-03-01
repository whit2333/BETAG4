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
#include "BETARCSCalorimeter.hh"
#include "G4HCofThisEvent.hh"
// HandsOn5: Hit collection
#include "G4SDManager.hh"
#include "G4Step.hh"
#include "G4TouchableHistory.hh"
#include "G4Track.hh"

BETARCSCalorimeter::BETARCSCalorimeter ( const G4String& name )
      :G4VSensitiveDetector ( name )
{
   collectionName.insert ( "RCSCalorimeterCollection" );
   fHitsCollectionID = -1;
}

BETARCSCalorimeter::~BETARCSCalorimeter() {}

void BETARCSCalorimeter::Initialize ( G4HCofThisEvent* hitsCollectionOfThisEvent )
{
   // HandsOn5: Creating hit collection
   // Create a new collection
   fHitsCollection =
      new BETARCSCalorimeterHitsCollection ( SensitiveDetectorName, collectionName[0] );

   if ( fHitsCollectionID < 0 )
      fHitsCollectionID = G4SDManager::GetSDMpointer()->GetCollectionID ( fHitsCollection );

   // Add collection to the event
   hitsCollectionOfThisEvent->AddHitsCollection ( fHitsCollectionID, fHitsCollection );

   // Initialise hits
   G4int i ( 0 );

   for ( i=0; i<720; i++ )
   {
      BETARCSCalorimeterHit* aHit = new BETARCSCalorimeterHit ( i );
      fHitsCollection->insert ( aHit );
   }
}

G4bool BETARCSCalorimeter::ProcessHits ( G4Step* aStep, G4TouchableHistory* )
{
   // HandsOn5: Accumulating hit data
   // Get energy deposited in this step
   G4double depositedEnergy = aStep->GetTotalEnergyDeposit();
   if ( 0. == depositedEnergy ) return true;


   // Get volume and copy number
   G4StepPoint* preStepPoint = aStep->GetPreStepPoint();
   G4TouchableHistory* theTouchable = ( G4TouchableHistory* ) ( preStepPoint->GetTouchable() );

   G4VPhysicalVolume* thePhysical = theTouchable->GetVolume();
   G4int copyNo = thePhysical->GetCopyNo();

   // Get corresponding hit
   BETARCSCalorimeterHit* aHit = ( *fHitsCollection ) [copyNo];

   // Check to see if this is the first time the hit has been updated
   if ( ! ( aHit->GetLogicalVolume() ) )
   {

      // Set volume information
      aHit->SetLogicalVolume ( thePhysical->GetLogicalVolume() );

      G4AffineTransform aTrans = theTouchable->GetHistory()->GetTopTransform();
      aTrans.Invert();

      aHit->SetRotation ( aTrans.NetRotation() );
      aHit->SetPosition ( aTrans.NetTranslation() );
   }

   // Accumulate energy deposition
   aHit->AddDepositedEnergy ( depositedEnergy );
   return true;
}

void BETARCSCalorimeter::EndOfEvent ( G4HCofThisEvent* ) {}
