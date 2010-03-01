//
// ********************************************************************
// * DISCLAIMER                                                       *
// *                                                                  *
// * The following disclaimer summarizes all the specific disclaimers *
// * of contributors to this software. The specific disclaimers,which *
// * govern, are listed with their locations in:                      *
// *   http://cern.ch/geant4/license                                  *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.                                                             *
// *                                                                  *
// * This  code  implementation is the  intellectual property  of the *
// * GEANT4 collaboration.                                            *
// * By copying,  distributing  or modifying the Program (or any work *
// * based  on  the Program)  you indicate  your  acceptance of  this *
// * statement, and all its terms.                                    *
// ********************************************************************
//
// $Id:$
//
// Jane Tinslay - adapted from A01 example
//
#ifndef BETAHodoscopeCellParameterisation_HH
#define BETAHodoscopeCellParameterisation_HH

#include "G4VPVParameterisation.hh"
#include "globals.hh"
#include "G4RotationMatrix.hh"
#include "G4Material.hh"
#include <vector>
#include "G4SDManager.hh"
#include "BETAHodoscopePMT.hh"

class G4VPhysicalVolume;

class BETAHodoscopeCellParameterisation : public G4VPVParameterisation {

public:

  // Constructor
  BETAHodoscopeCellParameterisation(G4Material * aMaterial);

  // Destructor
  virtual ~BETAHodoscopeCellParameterisation();

  // Method
  virtual void ComputeTransformation(const G4int copyNo,G4VPhysicalVolume *physVol) const;
  virtual G4VSolid* ComputeSolid(const G4int copyNo, G4VPhysicalVolume* physVol);
  virtual G4Material* ComputeMaterial // material, sensitivity, visAtt
     (const G4int copyNo, G4VPhysicalVolume* physVol,
         const G4VTouchable *parentTouch);
private:
G4VSensitiveDetector* HodoscopePMTs;
  G4RotationMatrix hodBarRotateRemove1;
  G4RotationMatrix hodBarRotateRemove2;
  G4Material * theMaterial;
  G4RotationMatrix * rotate1;
  G4RotationMatrix * rotate2;
  G4VSolid * theSolid;
  G4VSolid * hodoscopeBar;
  G4VSolid * PMTinquotes;
  G4double theta;
  // Data members
  std::vector<G4double> xCell;
  std::vector<G4double> yCell;
  std::vector<G4double> zCell;
};

#endif


