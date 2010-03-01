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
#ifndef BETAFakePlaneHIT_HH
#define BETAFakePlaneHIT_HH

#include "G4Allocator.hh"
#include "G4LogicalVolume.hh"
#include "G4RotationMatrix.hh"
#include "G4ThreeVector.hh"
#include "G4THitsCollection.hh"
#include "G4Transform3D.hh"
#include "G4VHit.hh"



class BETAFakePlaneHit : public G4VHit {

public:
  
  // Constructors
  BETAFakePlaneHit();
  BETAFakePlaneHit(G4int id);

  // Destructor
  virtual ~BETAFakePlaneHit();
  
  inline void *operator new(size_t);
  inline void operator delete(void *aHit);

  // Methods
  virtual void Draw();
  virtual void Print();

  G4int pid;
  G4double energy;
  G4ThreeVector  localPos;
  G4ThreeVector worldPos;
  G4ThreeVector  momentum;
private:
  
  // Data members
//  G4ThreeVector fPosition;
//  G4RotationMatrix fRotation;
//  const G4LogicalVolume* pLogicalVolume;
  
  
};

typedef G4THitsCollection<BETAFakePlaneHit> BETAFakePlaneHitsCollection;

extern G4Allocator<BETAFakePlaneHit> BETAFakePlaneHitAllocator;

inline void* BETAFakePlaneHit::operator new(size_t)
{
  void* aHit;
  aHit = (void*)BETAFakePlaneHitAllocator.MallocSingle();
  return aHit;
}

inline void BETAFakePlaneHit::operator delete(void* aHit)
{
  BETAFakePlaneHitAllocator.FreeSingle((BETAFakePlaneHit*) aHit);
}

#endif


