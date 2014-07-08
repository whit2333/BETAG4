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
//#include <gsl/gsl_interp.h>
//#include <gsl/gsl_errno.h>
//#include <gsl/gsl_spline.h>

class BETADetectorConstruction;
//using namespace std;

//______________________________________________________________________________
BETAG4PMTArray::BETAG4PMTArray ( G4String  name, G4int Nchan, G4String colname) : 
   G4VSensitiveDetector ( name )
//   fHCID(-1),
//   fCountAllPhotons(true),
//   fSavePhotonPositions(false),
//   fNumberOfChannels(Nchan),
//   fDiscriminatorThreshold(2)
{
   fHCID = -1;
   fCountAllPhotons = true;
   fSavePhotonPositions = false;
   fNumberOfChannels = Nchan;
   fCollectionName = colname;
   collectionName.insert( colname );
}
//______________________________________________________________________________
BETAG4PMTArray::~BETAG4PMTArray() {
}
//______________________________________________________________________________
void BETAG4PMTArray::Initialize ( G4HCofThisEvent* hitsCollectionOfThisEvent ) {

   fHitsCollection = new BETAG4PMTHitsCollection ( GetName() , fCollectionName );
   fHCID = G4SDManager::GetSDMpointer()->GetCollectionID ( fHitsCollection );
   hitsCollectionOfThisEvent->AddHitsCollection ( fHCID, fHitsCollection );

   // Initialise hits
   for (int i=0; i < GetNumberOfChannels(); i++ ) {
      BETAG4PMTHit * aHit = new BETAG4PMTHit ( i+1 );
      fHitsCollection->insert( aHit );
   }
}
//______________________________________________________________________________
G4bool BETAG4PMTArray::ProcessHits ( G4Step* aStep, G4TouchableHistory* ) {

   G4Track * theTrack = aStep->GetTrack();

   //if( theTrack->GetDefinition() == G4OpticalPhoton::OpticalPhotonDefinition()  &&
   //      theTrack->GetNextVolume()->GetName() == "tank_phys" )
   if ( theTrack->GetDefinition() == G4OpticalPhoton::OpticalPhotonDefinition() 
        && aStep->GetPreStepPoint()->GetStepStatus()== fGeomBoundary ) 
   {
      G4int copyNo  = theTrack->GetVolume()->GetCopyNo();
      //       std::cout << "pmt number: " << pmtNumber << "\n";
      BETAG4PMTHit* aHit = ( *fHitsCollection ) [copyNo];
      aHit->AddPhoton();
      if( aHit->GetNumberOfPhotoElectrons() > fDiscriminatorThreshold  && aHit->fTimingHit == false) {
         aHit->fTiming = theTrack->GetGlobalTime()/ns;
         aHit->fTimingHit = true;
      }
      aStep->GetTrack()->SetTrackStatus(fStopAndKill);
   }
   return true;
}
//______________________________________________________________________________
void BETAG4PMTArray::EndOfEvent ( G4HCofThisEvent* ) {

   //std::cout << GetFullPathName() << std::endl;
   //for(int i = 0; i<fHitsCollection->entries(); i++) {
   //   BETAG4PMTHit * aHit =  (*fHitsCollection)[i];
   //   if( aHit->fPhotons > 0 ) std::cout << i  << " : " << aHit->fPhotons << std::endl;
   //}
}
//______________________________________________________________________________
G4double BETAG4PMTArray::QE ( G4double photonEnergy ) {
   //alloc = gsl_interp_alloc ( gsl_interp_linear, 15 );
   //gsl_interp_init ( alloc, lambda, sensitivity, 15 );
   //G4double wlINnm = ( 0.000001240824/ ( photonEnergy ) ) /1.0e-9;
   //G4double res = gsl_interp_eval ( alloc, lambda, sensitivity, wlINnm, 0 ) *1240.824/wlINnm/1000.0;
   G4double res = 0.0;
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
