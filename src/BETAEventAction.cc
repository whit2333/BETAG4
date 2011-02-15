#include "globals.hh"
#include "BETASimulationManager.hh"
#include "BETAFrontTrackerHit.hh"
#include "BETAProtvinoCalorimeterHit.hh"
#include "BETARCSCalorimeterHit.hh"
#include "BETAEventAction.hh"
#include "BETAEventActionMessenger.hh"
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
//_______________________________________________________//

BETAEventAction::BETAEventAction()
{
   verboseLevel = 0;
   messenger = new BETAEventActionMessenger ( this );

//moved junk to RunAction :  BeginOfRunAction
   G4VVisManager* pVVisManager = G4VVisManager::GetConcreteInstance();
   if ( pVVisManager )
   {
      //    pVVisManager ->Draw();
   }
/*
  fSimulationManager = BETASimulationManager::GetInstance ();
  if(fSimulationManager) {
    fSimulationManager->SetDetectorVerbosity("GasCherenkov",0);
  }
// Get the event generator and detector constructoion so we can write the simulation truths to tree
   runManager = G4RunManager::GetRunManager();
   if(runManager) {
     //printf("\n got run manager \n");
     generator = (BETAPrimaryGeneratorAction *)runManager->GetUserPrimaryGeneratorAction();
   }
   if(runManager) {
     //printf("\n got run manager \n");
     construction = (BETADetectorConstruction *)runManager->GetUserDetectorConstruction();
   }

   G4String HCname;
   G4SDManager* SDman = G4SDManager::GetSDMpointer();

*/
}

BETAEventAction::~BETAEventAction()
{


//   delete messenger;


}

void BETAEventAction::BeginOfEventAction ( const G4Event* )
{

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void BETAEventAction::EndOfEventAction ( const G4Event* evt )
{
//    G4String colName;
// 
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

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
