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
#include "G4Step.hh"
#include "globals.hh"


//______________________________________________________________________________
BETASteppingAction::BETASteppingAction() {
   fConsecutiveSteps=0;
   fLastTrackIdInTracker=-100;
   fLastStepNumberInTracker=-100;
}
//______________________________________________________________________________
BETASteppingAction::~BETASteppingAction()
{
}
//______________________________________________________________________________
void BETASteppingAction::UserSteppingAction ( const G4Step * theStep )
{
   G4Track * theTrack = theStep->GetTrack();

   // check if it is alive
   if ( theTrack->GetTrackStatus() !=fAlive ) {
      return;
   }
/*
// Going to kill optical photon track if its step length is repeatedly a 1e+03 fm and 
// Inside tracker
   if( theTrack->GetDefinition() == G4OpticalPhoton::OpticalPhotonDefinition()  &&
      (  theTrack->GetVolume()->GetName() == "trackerX1_phys" ||
        theTrack->GetVolume()->GetName() == "trackerY1_phys" ||
        theTrack->GetVolume()->GetName() == "trackerY2_phys" ) )
   {
     if(theTrack->GetTrackID() == fLastTrackIdInTracker && 
        theTrack->GetCurrentStepNumber() == fLastStepNumberInTracker+1 )
     { 
       if(theTrack->GetStepLength() < 2000.0e-15*m)
       {
         fConsecutiveSteps++;
         if(fConsecutiveSteps > 50)  
         {
           theTrack->SetTrackStatus(fStopAndKill);
//           G4cout << " track killed \n ";
           fConsecutiveSteps=0;
         }
       } else {
         fConsecutiveSteps=0;
       }
     } else {
        fConsecutiveSteps=0;
     }
   } else {
      fConsecutiveSteps=0;
   }
         fLastTrackIdInTracker=theTrack->GetTrackID();
         fLastStepNumberInTracker=theTrack->GetCurrentStepNumber();
*/
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





