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


#include <gsl/gsl_interp.h>
#include <gsl/gsl_errno.h>
#include <gsl/gsl_spline.h>

#include <iostream>
#include <fstream>
using namespace std;
// CONSTRUCTOR
BETAHodoscopePMT::BETAHodoscopePMT ( G4String  name )
      :G4VSensitiveDetector ( name )
{
   G4String HCname;
   detname = name;
   collectionName.insert ( HCname="pmt" );
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

// DESTRUCTOR
BETAHodoscopePMT::~BETAHodoscopePMT() {}
//_______________________________________________________//

void BETAHodoscopePMT::Initialize ( G4HCofThisEvent* hitsCollectionOfThisEvent )
{

  fHitsCollection =
      new BETAHodoscopePMTHitsCollection ( detname, collectionName[0] );
   if ( HCID < 0 )
   {
      HCID = G4SDManager::GetSDMpointer()->GetCollectionID ( fHitsCollection );
   }

   // Add collection to the event
   hitsCollectionOfThisEvent->AddHitsCollection ( HCID, fHitsCollection );

   // Initialise hits

  for (int i=0; i<56; i++) {
       BETAHodoscopePMTHit* aHit = new BETAHodoscopePMTHit(i+1);
       fHitsCollection->insert(aHit);
     }
///////////////////////////

}
//_______________________________________________________//

G4bool BETAHodoscopePMT::ProcessHits ( G4Step* aStep, G4TouchableHistory* )
{
   G4Track * theTrack = aStep->GetTrack();

/// check that it is an optical photon and going to make it to the quartz/photocathode interface

   if ( theTrack->GetDefinition() == G4OpticalPhoton::OpticalPhotonDefinition() 
   && aStep->GetPreStepPoint()->GetStepStatus()== fGeomBoundary )
   {

      barNumber  = theTrack->GetNextVolume()->GetCopyNo() ;
      pmtNumber = theTrack->GetVolume()->GetCopyNo() ;
// G4cout << "       test    "<< pmt << G4endl;
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

void BETAHodoscopePMT::EndOfEvent ( G4HCofThisEvent* )
{
   ;
}

G4double BETAHodoscopePMT::QE ( G4double photonEnergy )
{

   alloc = gsl_interp_alloc ( gsl_interp_linear, 15 );
   gsl_interp_init ( alloc, lambda, sensitivity, 15 );
//double gsl_interp_eval (const gsl_interp * interp, const double xa[], const double ya[], double x, gsl_interp_accel * acc)
   G4double wlINnm = ( 0.000001240824/ ( photonEnergy ) ) /1.0e-9;

   G4double res = gsl_interp_eval ( alloc, lambda, sensitivity, wlINnm, 0 ) *1240.824/wlINnm/1000.0;

// sensitivity[gsl_interp_bsearch (lambda,wlINnm, 0, 14)]*1240.824/wlINnm/1000.0;
//G4cout << "photon E = " << photonEnergy <<"l nm = " <<  wlINnm << " eff : " << res<< " eff test: " << sensitivity[gsl_interp_bsearch (lambda,400, 0, 14)]*1240.824/400/1000<<G4endl;

   /*
   std::ofstream outfile("eff.out", std::ios_base::out);
   for(int k = 0;k< 80;k++)
   {
   outfile << (100+k*10) << " " << gsl_interp_eval (alloc, lambda, sensitivity, (100+k*10), 0)*1240.824/wlINnm/1000.0 << G4endl;
   }
   outfile.close();
   */
   if ( res <0 ) return ( 0.0 );
   else return ( res );
}
