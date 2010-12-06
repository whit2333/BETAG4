#ifndef BETAFrontTrackerHIT_HH
#define BETAFrontTrackerHIT_HH

#include "G4Allocator.hh"
#include "G4LogicalVolume.hh"
#include "G4RotationMatrix.hh"
#include "G4ThreeVector.hh"
#include "G4THitsCollection.hh"
#include "G4Transform3D.hh"
#include "G4VHit.hh"



class BETAFrontTrackerHit : public G4VHit {

public:
  
  // Constructors
  BETAFrontTrackerHit();
  BETAFrontTrackerHit(G4int id);

  // Destructor
  virtual ~BETAFrontTrackerHit();
  
  inline void *operator new(size_t);
  inline void operator delete(void *aHit);

  // Methods
  virtual void Draw();

  virtual void Print();

//  void SetLocalPosition(G4ThreeVector*pos) {localPos
  // Add a Photon
  inline void AddPhoton() {fPhotons++;}
  inline G4int GetPhotons() const {return fPhotons;}
/* Eventually add wavelength.....
  // Position vector
  inline void SetPosition(G4ThreeVector position) {fPosition = position;}
  inline G4ThreeVector GetPosition() const {return fPosition;}

  // Rotation matrix
  inline void SetRotation(G4RotationMatrix rotation) {fRotation = rotation;}
  inline G4RotationMatrix GetRotation() const {return fRotation;}

  // Logical volume
  inline void SetLogicalVolume(G4LogicalVolume* volume) {pLogicalVolume = volume;}
  inline const G4LogicalVolume* GetLogicalVolume() const {return pLogicalVolume;}
  */

  G4int cellNumber;
  G4int layerNumber;
  G4int mirrorNumber;
  G4ThreeVector  localPos;
  G4ThreeVector  worldPos;
  G4ThreeVector  worldMom;
  G4int fPhotons;


private:
  
  // Data members

//  G4ThreeVector fPosition;
//  G4RotationMatrix fRotation;
//  const G4LogicalVolume* pLogicalVolume;
  
  
};

typedef G4THitsCollection<BETAFrontTrackerHit> BETAFrontTrackerHitsCollection;

extern G4Allocator<BETAFrontTrackerHit> BETAFrontTrackerHitAllocator;

inline void* BETAFrontTrackerHit::operator new(size_t)
{
  void* aHit;
  aHit = (void*)BETAFrontTrackerHitAllocator.MallocSingle();
  return aHit;
}

inline void BETAFrontTrackerHit::operator delete(void* aHit)
{
  BETAFrontTrackerHitAllocator.FreeSingle((BETAFrontTrackerHit*) aHit);
}

#endif


