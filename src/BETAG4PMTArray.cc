#include "G4LogicalVolume.hh"
#include "G4Tubs.hh"
#include "BETADetectorConstruction.hh"
#include "BETASimulationManager.hh"
#include "BETAG4PMTArray.hh"
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

class BETADetectorConstruction;
using namespace std;

BETAG4PMTArray::BETAG4PMTArray ( G4String  name )
      : G4VSensitiveDetector ( name )
{
   fCountAllPhotons = true;
   fSavePhotonPositions = false;
   fNumberOfChannels = 12;
   G4String HCname;
   detname = name;
   collectionName.insert ( HCname="pmt" );
   HCID = -1;
   BETADetectorConstruction * construction = BETASimulationManager::GetInstance()->fConstruction;
   fDiscriminatorThreshold = 2; // photoelectrons

//   G4double tubeDiameter=4.0*2.54*cm;
//   G4double tubeLength=15.0*2.54*cm;
// 
//  G4double  MUtubeID=3.01*2.54*cm;
//  G4double  MUtubeLength=14.0*2.54*cm;
//  G4double  MUtubeThickness=4.0*mm;
// 
//   G4double PMTtubeDiameter=3.0*2.54*cm;
//   G4double PMTtubeLength=3.0*2.54*cm;
// 
//   G4Tubs * PMTcontainer =  new G4Tubs("PMTcontainer", 0., tubeDiameter/2.0, tubeLength/2.0, 0., 7.);
//   G4Tubs * PMT =  new G4Tubs("PMT", 0., PMTtubeDiameter/2.0, PMTtubeLength/2.0, 0., 7.);
//   G4Tubs * MU =  new G4Tubs("MU", 0., MUtubeID/2.0, tubeLength, 0., 7.);
//   G4Tubs * MU2 =  new G4Tubs("MU2", 0., (MUtubeID+MUtubeThickness)/2.0, tubeLength/2.0, 0., 7.);
//           G4RotationMatrix unitrm;
// 
//  //   fit perfectly with the real can. Do it for the cavity too.
//    G4VSolid* MUPipe =
//       new G4SubtractionSolid("MUPipe", MU2, MU,G4Transform3D(unitrm,G4ThreeVector(0.,0.,0.));
// 
// fPMTLogicalVolume=0;
//   G4LogicalVolume * container_log = new G4LogicalVolume(PMTcontainer, construction->NitrogenGas, "PipeCav1");
//   G4LogicalVolume * MUPipe_log = new G4LogicalVolume(MUPipe, ((G4NistManager*)G4NistManager::Instance())->FindOrBuildMaterial ( "G4_Fe" ), "MUPipe_log");
//    new G4PVPlacement(G4Transform3D(unitrm,G4ThreeVector()),MUPipe_log, "PMT+mumetal",container_log, false, 0);
//  fPMTLogicalVolume=container_log;

}
//_______________________________________________________________//
BETAG4PMTArray::~BETAG4PMTArray() {
;
}

//_______________________________________________________________//

void BETAG4PMTArray::Initialize ( G4HCofThisEvent* hitsCollectionOfThisEvent )
{

   fHitsCollection =
      new BETAG4PMTHitsCollection ( detname, collectionName[0] );
   if ( HCID < 0 )
   {
      HCID = G4SDManager::GetSDMpointer()->GetCollectionID ( fHitsCollection );
   }
   // Add collection to the event
   hitsCollectionOfThisEvent->AddHitsCollection ( HCID, fHitsCollection );

   // Initialise hits

   for (int i=0; i<GetNumberOfChannels(); i++ )
   {
      BETAG4PMTHit * aHit = new BETAG4PMTHit ( i+1);
      fHitsCollection->insert ( aHit );
   }

}

//________________________________________________________________//
G4bool BETAG4PMTArray::ProcessHits ( G4Step* aStep, G4TouchableHistory* ) {
  G4Track * theTrack = aStep->GetTrack();
  if( theTrack->GetDefinition() == G4OpticalPhoton::OpticalPhotonDefinition()  &&
      theTrack->GetNextVolume()->GetName() == "tank_phys" )
    {
/// \todo there has to be a better way than this. Need to remove strings!!!
      G4int pmtNumber  = theTrack->GetVolume()->GetCopyNo();
      BETAG4PMTHit* aHit = ( *fHitsCollection ) [pmtNumber-1];
      aHit->AddPhoton();
      if( aHit->GetNumberOfPhotoElectrons() > fDiscriminatorThreshold  && aHit->fTimingHit == false) {
        aHit->fTiming = theTrack->GetGlobalTime()/ns;
        aHit->fTimingHit = true;
      }

   }
   return true;
}

void BETAG4PMTArray::EndOfEvent ( G4HCofThisEvent* )
{
   ;
}

G4double BETAG4PMTArray::QE ( G4double photonEnergy )
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
