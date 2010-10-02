#ifndef BETAHodoscopePMTHIT_HH
#define BETAHodoscopePMTHIT_HH

#include "G4Allocator.hh"
#include "G4LogicalVolume.hh"
#include "G4RotationMatrix.hh"
#include "G4ThreeVector.hh"
#include "G4THitsCollection.hh"
#include "G4Transform3D.hh"
#include "G4VHit.hh"



class BETAHodoscopePMTHit : public G4VHit {

public:
  
  // Constructors
  BETAHodoscopePMTHit();
  BETAHodoscopePMTHit(G4int id);

  // Destructor
  virtual ~BETAHodoscopePMTHit();
  
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

typedef G4THitsCollection<BETAHodoscopePMTHit> BETAHodoscopePMTHitsCollection;

extern G4Allocator<BETAHodoscopePMTHit> BETAHodoscopePMTHitAllocator;

inline void* BETAHodoscopePMTHit::operator new(size_t)
{
  void* aHit;
  aHit = (void*)BETAHodoscopePMTHitAllocator.MallocSingle();
  return aHit;
}

inline void BETAHodoscopePMTHit::operator delete(void* aHit)
{
  BETAHodoscopePMTHitAllocator.FreeSingle((BETAHodoscopePMTHit*) aHit);
}

#endif


