//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
//
// $Id: BETAEventAction.cc,v 1.3 2006/06/29 17:54:20 gunter Exp $
// GEANT4 tag $Name: geant4-08-01 $
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
#include "globals.hh"
#include "BETAAnalysisManager.hh"

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

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

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

}

BETAEventAction::~BETAEventAction()
{


   delete messenger;


}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void BETAEventAction::BeginOfEventAction ( const G4Event* )
{

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void BETAEventAction::EndOfEventAction ( const G4Event* evt )
{



   // Diagnostics

   if ( verboseLevel==0 || evt->GetEventID() % verboseLevel != 0 ) return;

   G4PrimaryParticle* primary = evt->GetPrimaryVertex ( 0 )->GetPrimary ( 0 );
   G4cout << G4endl
   << ">>> Event " << evt->GetEventID() << " >>> Simulation truth : "
   << primary->GetG4code()->GetParticleName()
   << " " << primary->GetMomentum() << G4endl;


}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
