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
// $Id: BETAPrimaryGeneratorMessenger.hh,v 1.2 2006/06/29 17:54:07 gunter Exp $
// GEANT4 tag $Name: geant4-08-01 $
//
// 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef BETAPrimaryGeneratorMessenger_h
#define BETAPrimaryGeneratorMessenger_h 1


#include "G4UImessenger.hh"
#include "globals.hh"

//#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithAnInteger.hh"
class BETAPrimaryGeneratorAction;
class G4UIdirectory;
#include "G4UIcmdWithADouble.hh"


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class BETAPrimaryGeneratorMessenger: public G4UImessenger
{
  public:
    BETAPrimaryGeneratorMessenger(BETAPrimaryGeneratorAction*);
   ~BETAPrimaryGeneratorMessenger();
    
    void SetNewValue(G4UIcommand*, G4String);
    
  private:
    BETAPrimaryGeneratorAction* BETAAction;
    G4UIdirectory*               gunDir; 
    G4UIcmdWithADoubleAndUnit*   polarCmd;
    G4UIcmdWithAnInteger*   isotropic;
    G4UIcmdWithADouble*   momentum;
    G4UIcmdWithADouble*   sigmaMomentum;
    G4UIcmdWithADouble*   sete_piRatio;
    G4UIcmdWithADouble*   setpi0Ratio;
    G4UIcmdWithADouble*   setParticleTheta;
    G4UIcmdWithADouble*   setParticlePhi;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

