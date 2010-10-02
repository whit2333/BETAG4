#include "BETASteppingAction.hh"
#include "G4SteppingManager.hh"
#include "G4Track.hh"
#include "G4Step.hh"
#include "G4StepPoint.hh"
#include "G4TrackStatus.hh"
#include "G4VPhysicalVolume.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleTypes.hh"
#include <fstream>
#include "G4UnitsTable.hh"
#include "G4ParticlePropertyTable.hh"
#include "G4ParticlePropertyData.hh"



BETASteppingAction::BETASteppingAction()
{

}

BETASteppingAction::~BETASteppingAction()
{


}

void BETASteppingAction::UserSteppingAction ( const G4Step * theStep )
{
   G4Track * theTrack = theStep->GetTrack();

   // check if it is alive
   if ( theTrack->GetTrackStatus() !=fAlive ) {
      return;
   }

/// COUNT The number of reflected photons
///////////////////////////////////////////////////////////////////////////////
//  if( theTrack->GetDefinition() == G4OpticalPhoton::OpticalPhotonDefinition()  &&
//      ( theTrack->GetVolume()->GetName() == "Far - Mirror" ||  theTrack->GetVolume()->GetName() == "Near - Mirror" ) &&
//      theTrack->GetNextVolume()->GetName() == "tank_phys")
//   {
// //4cout << "COPY NUMBER " <<  theTrack->GetVolume()->GetCopyNo() << G4endl;
//
// ReflectedPhotons++;
// }





}





