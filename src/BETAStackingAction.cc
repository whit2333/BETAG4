#include "BETAStackingAction.hh"

#include "G4ParticleDefinition.hh"
#include "G4ParticleTypes.hh"
#include "G4Track.hh"
#include "G4ios.hh"
#include "G4VPhysicalVolume.hh"
#include "G4SteppingManager.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

BETAStackingAction::BETAStackingAction()
      : gammaCounter ( 0 )
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

BETAStackingAction::~BETAStackingAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4ClassificationOfNewTrack
BETAStackingAction::ClassifyNewTrack ( const G4Track * aTrack )
{
   if ( aTrack->GetDefinition() == G4OpticalPhoton::OpticalPhotonDefinition() )
   {
      // particle is optical photon

      if ( aTrack->GetParentID() >0 )
      {
         // particle is secondary
         gammaCounter++;
      }
   }
   return fUrgent;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void BETAStackingAction::NewStage()
{
//  G4cout << "Number of optical photons produces in this event : "
//         << gammaCounter << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void BETAStackingAction::PrepareNewEvent()
{
   gammaCounter = 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
