#include "BETAPMT.hh"
#include "G4HCofThisEvent.hh"
#include "G4SDManager.hh"
#include "G4Step.hh"
#include "G4TouchableHistory.hh"
#include "G4Track.hh"
#include "G4ParticleDefinition.hh"
#include "G4OpticalPhoton.hh"
#include "G4RandomDirection.hh"
#include "Randomize.hh"
#include "fstream"
#include <gsl/gsl_interp.h>
#include <gsl/gsl_errno.h>
#include <gsl/gsl_spline.h>

using namespace std;

BETAPMT::BETAPMT ( G4String  name )
      :G4VSensitiveDetector ( name )
{
   countAllPhotons = true; // or use QE from input data below.
   savePhotonPositions = false;
   G4String HCname;
   detname = name;
   collectionName.insert ( HCname="pmt" );
   HCID = -1;


   ifstream input_file ;
   input_file.open ( "cathodeSensitivity.dat" );
   for ( int i = 0;i < 15;i++ )
   {
      input_file >> lambda[i] >> sensitivity[i] ;
   }
   input_file.close();



}
//_______________________________________________________________//
BETAPMT::~BETAPMT() {
;
}

//_______________________________________________________________//

void BETAPMT::Initialize ( G4HCofThisEvent* hitsCollectionOfThisEvent )
{
/// \todo initialize fineite number of hits (one for each tube) and then accumulate the data
   fHitsCollection =
      new BETAPMTHitsCollection ( detname, collectionName[0] );
   if ( HCID < 0 )
   {
      HCID = G4SDManager::GetSDMpointer()->GetCollectionID ( fHitsCollection );
   }
   // Add collection to the event
   hitsCollectionOfThisEvent->AddHitsCollection ( HCID, fHitsCollection );
}

//________________________________________________________________//
G4bool BETAPMT::ProcessHits ( G4Step* aStep, G4TouchableHistory* ) 
{
   G4Track * theTrack = aStep->GetTrack();
/// COUNT The number of reflected photons from the mirrors
/*    if ( theTrack->GetDefinition() == G4OpticalPhoton::OpticalPhotonDefinition()  &&
            ( theTrack->GetVolume()->GetName() == "Far - Mirror" ||  theTrack->GetVolume()->GetName() == "Near - Mirror" ) &&
            theTrack->GetNextVolume()->GetName() == "tank_phys" )
    {

    }
   */
//std::cout <<  "tester\n";

/// check that it is an optical photon and going to make it to the quartz/photocathode interface
   if ( theTrack->GetDefinition() == G4OpticalPhoton::OpticalPhotonDefinition()  &&
        theTrack->GetNextVolume()->GetName() == "tank_phys" )
   {
      BETAPMTHit * faceCountHit = new BETAPMTHit();
      fHitsCollection->insert ( faceCountHit );

      if (!countAllPhotons) { // calculate QE
         if ( G4UniformRand() < QE ( theTrack->GetTotalEnergy() /eV ) )   // Need to make 0.2 a function of wavelength
         {
            //G4cout <<  "qe " << QE(theTrack->GetTotalEnergy()/eV ) << G4endl;
            // Get PMT #
            pmt = theTrack->GetVolume()->GetCopyNo();
//G4cout << "       TEST    "<< pmt << G4endl;
            BETAPMTHit* aHit = new BETAPMTHit ( pmt -1 );
            fHitsCollection->insert ( aHit );
            aHit->Gtime = theTrack->GetGlobalTime();
            if (savePhotonPositions) {
               // Get position
               G4StepPoint* preStepPoint = aStep->GetPreStepPoint();
               G4TouchableHistory* theTouchable
               = ( G4TouchableHistory* ) ( preStepPoint->GetTouchable() );
               G4ThreeVector worldPos = preStepPoint->GetPosition();
               aHit->localPos = theTouchable->GetHistory()->GetTopTransform
                                ().TransformPoint ( worldPos ) ;

            }
         }

      } else { // we are counting all photons
         pmt = theTrack->GetVolume()->GetCopyNo();
//G4cout << "       TEST    "<< pmt << G4endl;
         BETAPMTHit* aHit = new BETAPMTHit ( pmt );
//         fHitsCollection->insert ( aHit );
         aHit->Gtime = theTrack->GetGlobalTime();
         if (savePhotonPositions) {
            // Get position
            G4StepPoint* preStepPoint = aStep->GetPreStepPoint();
            G4TouchableHistory* theTouchable
            = ( G4TouchableHistory* ) ( preStepPoint->GetTouchable() );
            G4ThreeVector worldPos = preStepPoint->GetPosition();
            aHit->localPos = theTouchable->GetHistory()->GetTopTransform
                             ().TransformPoint ( worldPos ) ;

         }
         fHitsCollection->insert ( aHit );

      }

   } // end of face count

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

void BETAPMT::EndOfEvent ( G4HCofThisEvent* )
{
   ;
}

G4double BETAPMT::QE ( G4double photonEnergy )
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
