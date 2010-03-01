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
// $Id: BETARunActionMessenger.cc,v 1.3 2006/06/29 17:54:29 gunter Exp $
// GEANT4 tag $Name: geant4-08-01 $
//
//

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "BETARunActionMessenger.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithoutParameter.hh"
#include "BETARunAction.hh"
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

BETARunActionMessenger::BETARunActionMessenger ( BETARunAction * theRunAction )
      :runAction ( theRunAction )
{
   aidaDir = new G4UIdirectory ( "/aida/" );
   aidaDir->SetGuidance ( "Write aida tree RunAction control" );

   writeTree = new G4UIcmdWithoutParameter ( "/aida/writeTree",this );
   writeTree->SetGuidance ( "Write aida file" );
   writeTree->SetGuidance ( "  angle w.r.t. (k,n) plane" );
//   writeTree->SetParameterName("angle",true);
//   writeTree->SetUnitCategory("Angle");
//   writeTree->SetDefaultValue(-360.0);
//   writeTree->SetDefaultUnit("deg");
   writeTree->AvailableForStates ( G4State_Idle );

   showPlot = new G4UIcmdWithAnInteger ( "/aida/showPlot",this );
   showPlot->SetGuidance ( " Argument 1 shows histograms and graphs" );
   showPlot->SetGuidance ( " Argument 0 does not show anything" );
   showPlot->AvailableForStates ( G4State_Idle );

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

BETARunActionMessenger::~BETARunActionMessenger()
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void BETARunActionMessenger::SetNewValue (
   G4UIcommand* command, G4String newValue )
{
   if ( command == writeTree )
   {
      G4cout << " T1 " <<  G4endl;

   }
   if ( command == showPlot )
   {
      runAction->showPlot ( showPlot->GetNewIntValue ( newValue ) );
   }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

