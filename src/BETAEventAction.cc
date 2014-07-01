#include "BETAEventAction.hh"
#include "globals.hh"
#include "G4Event.hh"
#include "G4EventManager.hh"
#include "G4TrajectoryContainer.hh"
#include "G4Trajectory.hh"
#include "G4VVisManager.hh"
#include "G4ios.hh"
#include "G4RandomDirection.hh"
#include "Randomize.hh"
#include "G4MultiFunctionalDetector.hh"
#include "CLHEP/Vector/ThreeVector.h"
#include "G4ThreeVector.hh"
#include "G4VVisManager.hh"
#include "G4SDManager.hh"

#include "BETASimulationManager.hh"
//#include "BETAFrontTrackerHit.hh"
//#include "BETAProtvinoCalorimeterHit.hh"
//#include "BETARCSCalorimeterHit.hh"
//#include "BETAEventActionMessenger.hh"

//______________________________________________________________________________
BETAEventAction::BETAEventAction() {

   verboseLevel = 0;
   //messenger = new BETAEventActionMessenger ( this );
   //moved junk to RunAction :  BeginOfRunAction
   G4VVisManager* pVVisManager = G4VVisManager::GetConcreteInstance();
   if ( pVVisManager )
   {
      //    pVVisManager ->Draw();
   }
   fSimulationManager = BETASimulationManager::GetInstance();

}
//______________________________________________________________________________
BETAEventAction::~BETAEventAction() {
}
//______________________________________________________________________________
void BETAEventAction::BeginOfEventAction ( const G4Event* ) {
   fSimulationManager->fEvents->Clear();
}
//______________________________________________________________________________
void BETAEventAction::EndOfEventAction ( const G4Event* evt ) {
   //    G4String colName;
   //    G4SDManager* SDman = G4SDManager::GetSDMpointer();
   //    G4HCofThisEvent * HCE = evt->GetHCofThisEvent();


   // Diagnostics

   if ( verboseLevel==0 || evt->GetEventID() % verboseLevel != 0 ) return;

   G4PrimaryParticle* primary = evt->GetPrimaryVertex ( 0 )->GetPrimary ( 0 );
   G4cout << G4endl
   << ">>> Event " << evt->GetEventID() << " >>> Simulation truth : "
   << primary->GetG4code()->GetParticleName()
   << " " << primary->GetMomentum() << G4endl;

}
//______________________________________________________________________________

