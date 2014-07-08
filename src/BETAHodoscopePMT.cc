#include "BETAHodoscopePMT.hh"

#include "G4HCofThisEvent.hh"
#include "G4SDManager.hh"
#include "G4Step.hh"
#include "G4TouchableHistory.hh"
#include "G4Track.hh"
#include "G4ParticleDefinition.hh"
#include "G4OpticalPhoton.hh"
#include "G4RandomDirection.hh"
#include "Randomize.hh"

#include <iostream>
#include <fstream>

using namespace std;

//______________________________________________________________________________
BETAHodoscopePMT::BETAHodoscopePMT ( G4String  name ):G4VSensitiveDetector ( name ) {
   G4String HCname;
   detname = name;
   collectionName.insert ( HCname="lpmt" );
   HCID = -1;
   fDiscriminatorThreshold = 2;
//    std::ifstream input_file ;
//    input_file.open ( "cathodeSensitivity.dat" );
//    for ( int i = 0;i < 15;i++ )
//    {
//       input_file >> lambda[i] >> sensitivity[i] ;
//    }
//    input_file.close();
}
//______________________________________________________________________________
BETAHodoscopePMT::~BETAHodoscopePMT(){
}
//______________________________________________________________________________
void BETAHodoscopePMT::Initialize(G4HCofThisEvent* hitsCollectionOfThisEvent ) {

   fHitsCollection = new BETAHodoscopePMTHitsCollection(detname, collectionName[0] );
   HCID = G4SDManager::GetSDMpointer()->GetCollectionID ( fHitsCollection );
   hitsCollectionOfThisEvent->AddHitsCollection ( HCID, fHitsCollection );

   // Initialise hits
   for (int i=0; i<56; i++) {
      BETAHodoscopePMTHit* aHit = new BETAHodoscopePMTHit(i+1);
      fHitsCollection->insert(aHit);
   }
}
//______________________________________________________________________________
G4bool BETAHodoscopePMT::ProcessHits(G4Step* aStep, G4TouchableHistory* ) {

   G4Track * theTrack = aStep->GetTrack();

   // check that it is an optical photon and going to make it to the quartz/photocathode interface
   if ( theTrack->GetDefinition() == G4OpticalPhoton::OpticalPhotonDefinition() 
        && aStep->GetPreStepPoint()->GetStepStatus()== fGeomBoundary ) {

      barNumber  = theTrack->GetNextVolume()->GetCopyNo() ;
      pmtNumber = theTrack->GetVolume()->GetCopyNo() ;
      //G4cout << "       test    "<< pmt << G4endl;

      BETAHodoscopePMTHit* aHit = ( *fHitsCollection ) [barNumber*2+pmtNumber-1];

      aHit->AddPhoton();
      if( aHit->GetNumberOfPhotons() > fDiscriminatorThreshold  && aHit->fTimingHit == false) {
         aHit->fTiming = theTrack->GetGlobalTime()/ns;
         aHit->fTimingHit = true;
      }

         // Get position
//          G4StepPoint* preStepPoint = aStep->GetPreStepPoint();
//          G4TouchableHistory* theTouchable
//          = ( G4TouchableHistory* ) ( preStepPoint->GetTouchable() );
//          aHit->worldPos = preStepPoint->GetPosition();
//          aHit->localPos = theTouchable->GetHistory()->GetTopTransform
//                           ().TransformPoint ( aHit->worldPos ) ;
      //}
/*            aHit->Gtime = theTrack->GetGlobalTime();*/

//     aStep->GetTrack()->SetTrackStatus(fStopAndKill);

   }


    // Get energy deposited in this step
    /*
    G4doubdepositedEnergy le = aStep->GetTotalEnergyDeposit();
    if (0 == depositedEnergy) return true;


    // Get volume and copy number
    G4StepPoint* preStepPoint = aStep->GetPreStepPoint();
    G4TouchableHistory* theTouchable = (G4TouchableHistory*)(preStepPoint->GetTouchable());

    G4VPhysicalVolume* thePhysical = theTouchable->GetVolume();
    G4int copyNo = thePhysical->GetCopyNo();

    // Get corresponding hit
    BETAHodoscopePMTHit* aHit = (*fHitsCollection)[copyNo];

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
//______________________________________________________________________________
void BETAHodoscopePMT::EndOfEvent ( G4HCofThisEvent* ) {
   ;
}
//______________________________________________________________________________

