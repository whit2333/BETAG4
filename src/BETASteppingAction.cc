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





