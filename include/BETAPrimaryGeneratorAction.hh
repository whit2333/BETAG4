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
// $Id: BETAPrimaryGeneratorAction.hh,v 1.6 2006/06/29 17:54:04 gunter Exp $
// GEANT4 tag $Name: geant4-08-01 $
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef BETAPrimaryGeneratorAction_h
#define BETAPrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "globals.hh"

class G4ParticleGun;
class G4Event;
class BETAPrimaryGeneratorMessenger;
class G4ParticleDefinition;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class BETAPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
  public:
    BETAPrimaryGeneratorAction();
   ~BETAPrimaryGeneratorAction();

  public:
    void GeneratePrimaries(G4Event*);

    void SetOptPhotonPolar();
    void SetOptPhotonPolar(G4double);
    void setIsotropic(G4int);
    void setSigmaMomentum(G4double);
    void setMomentum(G4double);
    void setElectronPionRatio(G4double);
    void setPiZeroRatio(G4double);

    void setPartTheta ( G4double P );
    G4double getPartTheta ();
    void setPartPhi ( G4double P );
    G4double getPartPhi ();
    G4double theta_particle,phi_particle;
    G4double momentum;
    G4double sigmaMomentum;
    G4double eventTheta;
    G4double eventPhi;
    G4double eventEnergy;
    G4double eventMomentum;

  private:

	bool background;
	bool goodElectron;
	bool backgroundAndElectron;

    G4double sigmaTheta, sigmaPhi;

    G4ParticleGun* particleGun;
    BETAPrimaryGeneratorMessenger* gunMessenger;
    G4ParticleDefinition* electron;
    G4ParticleDefinition* pionminus;
    G4ParticleDefinition* pionplus;
    G4ParticleDefinition* pionzero;
    G4ParticleDefinition* kaon;
    G4ParticleDefinition* proton;

    G4double sigmaAngle;
	G4double ElectronPionRatio;
	G4double Pi0Ratio;
    G4bool randomizePrimary;
     G4int iso; 


  public:

    G4ParticleGun* Gun() {return particleGun;}
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif /*BETAPrimaryGeneratorAction_h*/
