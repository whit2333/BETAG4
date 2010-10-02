#ifndef BETAMirrorHIT_HH
#define BETAMirrorHIT_HH

#include "G4Allocator.hh"
#include "G4LogicalVolume.hh"
#include "G4RotationMatrix.hh"
#include "G4ThreeVector.hh"
#include "G4THitsCollection.hh"
#include "G4Transform3D.hh"
#include "G4VHit.hh"



class BETAMirrorHit : public G4VHit {

public:
  
  // Constructors
  BETAMirrorHit();
  BETAMirrorHit(G4int id);

  // Destructor
  virtual ~BETAMirrorHit();
  
  inline void *operator new(size_t);
  inline void operator delete(void *aHit);

  // Methods
  virtual void Draw();
  virtual void Print();

//  void SetLocalPosition(G4ThreeVector*pos) {localPos
  // Add a Photon
  inline void AddPhoton() {fPhotons++;}
  inline G4int GetPhotons() const {return fPhotons;}
  G4int tubeNumber;
  G4int mirrorNumber;
  G4ThreeVector  localPos;
private:
  
  // Data members

  G4int fPhotons;
//  G4ThreeVector fPosition;
//  G4RotationMatrix fRotation;
//  const G4LogicalVolume* pLogicalVolume;
  
  
};

typedef G4THitsCollection<BETAMirrorHit> BETAMirrorHitsCollection;

extern G4Allocator<BETAMirrorHit> BETAMirrorHitAllocator;

inline void* BETAMirrorHit::operator new(size_t)
{
  void* aHit;
  aHit = (void*)BETAMirrorHitAllocator.MallocSingle();
  return aHit;
}

inline void BETAMirrorHit::operator delete(void* aHit)
{
  BETAMirrorHitAllocator.FreeSingle((BETAMirrorHit*) aHit);
}

#endif


