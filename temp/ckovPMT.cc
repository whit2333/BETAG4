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
#include "BETAPMT.hh"
#include "G4HCofThisEvent.hh"
// HandsOn5: Hit collection
#include "G4SDManager.hh"
#include "G4Step.hh"    
#include "G4TouchableHistory.hh"
#include "G4Track.hh"

// CONSTRUCTOR
BETAPMT::BETAPMT(const G4String& name)
  :G4VSensitiveDetector(name)
{
  collectionName.insert("PMTCollection");
  fHitsCollectionID = -1;
}

// DESTRUCTOR
BETAPdMT::~BETAPMT() {}

//////////////////////////////
void BETAPMT::Initialize(G4HCofThisEvent* hitsCollectionOfThisEvent)
{
  // HandsOn5: Creating hit collection
  // Create a new collection
  hitsCollectionOfThisEvent =
    new BETAPMTHitsCollection(name, collectionName[0]);
 
  if(fHitsCollectionID < 0)
    fHitsCollectionID = G4SDManager::GetSDMpointer()->GetCollectionID(hitsCollectionOfThisEvent);
 
  // Add collection to the event
  hitsCollectionOfThisEvent->AddHitsCollection(fHitsCollectionID, hitsCollectionOfThisEvent);
 
  // Initialise hits
  G4int i(0);
 
// 8 PMTs
  for (i=0; i<8; i++) {
    BETAPMTHit* aHit = new BETAPMTHit(i);
    hitsCollectionOfThisEvent->insert(aHit);
  }
}

//////////////////////

G4bool BETAPMT::ProcessHits(G4Step* aStep, G4TouchableHistory*)
{
/// check that it is an optical photon and going to make it to the quartz/photocathode interface
 G4Track * theTrack = aStep->GetTrack();
if( theTrack->GetDefinition() == G4OpticalPhoton::OpticalPhotonDefinition()  &&  
   theTrack->GetNextVolume()->GetName() == "pmtFace_phys" )
  {
	 BETAPMTHit* aHit = (*hitsCollectionOfThisEvent)[ theTrack->GetNextVolume()->GetCopyNo()-1 ];
	aHit->AddPhoton;

  }

 /* // HandsOn5: Accumulating hit data
  // Get energy deposited in this step
  
  G4doubdepositedEnergy le = aStep->GetTotalEnergyDeposit();
  if (0 == depositedEnergy) return true;
 
 
  // Get volume and copy number
  G4StepPoint* preStepPoint = aStep->GetPreStepPoint();
  G4TouchableHistory* theTouchable = (G4TouchableHistory*)(preStepPoint->GetTouchable());
 
  G4VPhysicalVolume* thePhysical = theTouchable->GetVolume();
  G4int copyNo = thePhysical->GetCopyNo();
 
  // Get corresponding hit
  BETAPMTHit* aHit = (*fHitsCollection)[copyNo];
 
  // Check to see if this is the first time the hit has been updated
  if (!(aHit->GetLogicalVolume())) {
 
    // Set volume information
    aHit->SetLogicalVolume(thePhysical->GetLogicalVolume());
 
    G4AffineTransform aTrans = theTouchable->GetHistory()->GetTopTransform();
    aTrans.Invert();
 
    aHit->SetRotation(aTrans.NetRotation());
    aHit->SetPosition(aTrans.NetTranslation());
  }
 
  // Accumulate energy deposition  
  aHit->AddDepositedEnergy(depositedEnergy);
*/
  return true;
}

void BETAPMT::EndOfEvent(G4HCofThisEvent*) {}
