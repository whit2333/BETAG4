#include "BETAFakePlane.hh"
#include "G4HCofThisEvent.hh"
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
// #include <gsl/gsl_interp.h>
// #include <gsl/gsl_errno.h>
// #include <gsl/gsl_spline.h>
// CONSTRUCTOR
BETAFakePlane::BETAFakePlane ( G4String  name , G4String colname  )
      :G4VSensitiveDetector ( name )
{
   G4String HCname;// = name;
   detname = name;
   collectionName.insert ( HCname = colname );
   HCID = -1;
   fSensitiveVolume=0;

}

// DESTRUCTOR
BETAFakePlane::~BETAFakePlane() {

}
//_________________________________________________________

void BETAFakePlane::Initialize ( G4HCofThisEvent* hitsCollectionOfThisEvent ) {
   fHitsCollection =
      new BETAFakePlaneHitsCollection ( detname, collectionName[0] );
   if ( HCID < 0 )
   {
      HCID = G4SDManager::GetSDMpointer()->GetCollectionID ( fHitsCollection );
   }
   hitsCollectionOfThisEvent->AddHitsCollection ( HCID, fHitsCollection );
}
//_________________________________________________________

G4bool BETAFakePlane::ProcessHits ( G4Step* aStep, G4TouchableHistory* )  {
   G4Track * theTrack = aStep->GetTrack();
   G4StepPoint* point2 = aStep->GetPostStepPoint();
   double energy = theTrack->GetTotalEnergy();
//G4cout << "Process Fake Plane Hit\n";
   if(energy/MeV > 0.50) {     // 0.5 MeV is cherenkov threshold

// check that it is in the sensitive volme and is leaving the volume
      if( ( theTrack->GetVolume() == this->GetSensitiveVolume() ) && 
           ( point2->GetStepStatus() == fGeomBoundary   ) )  { 

//    std::cout << " track at boundary " << this->GetSensitiveVolume()->GetName() << " with energy " << energy << "\n";
/*         pid = theTrack->GetDefinition()->GetPDGEncoding();*/
      BETAFakePlaneHit* aHit = new BETAFakePlaneHit ( theTrack->GetDefinition()->GetPDGEncoding() );
      fHitsCollection->insert ( aHit );
      G4StepPoint* preStepPoint = aStep->GetPreStepPoint();
      G4TouchableHistory* theTouchable
         = ( G4TouchableHistory* ) ( preStepPoint->GetTouchable() );

      aHit->fPosition      = preStepPoint->GetPosition(); // World position
      aHit->fLocalPosition = theTouchable->GetHistory()->GetTopTransform().TransformPoint ( aHit->fPosition ) ;
      aHit->fMomentum      =  theTrack->GetMomentum();
      aHit->fEnergy        =  energy/MeV;
    }

   } // end of energy threshold
//G4cout << "Done processing Fake Plane Hit\n";

   return true;
}

void BETAFakePlane::EndOfEvent ( G4HCofThisEvent* )
{
//G4cout << "End of Event from Fake Plane \n";
}
