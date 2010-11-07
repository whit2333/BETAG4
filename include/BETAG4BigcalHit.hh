#ifndef BETAG4BigcalHit_HH
#define BETAG4BigcalHit_HH

#include "G4Allocator.hh"
#include "G4LogicalVolume.hh"
#include "G4RotationMatrix.hh"
#include "G4ThreeVector.hh"
#include "G4THitsCollection.hh"
#include "G4Transform3D.hh"
#include "G4VHit.hh"

class G4AttDef;
class G4AttValue;

class BETAG4BigcalHit : public G4VHit {

public:
  
  // Constructors
  BETAG4BigcalHit();
  BETAG4BigcalHit(G4int id);

  // Destructor
  virtual ~BETAG4BigcalHit();
  
  inline void *operator new(size_t);
  inline void operator delete(void *aHit);

  // Methods
  virtual void Draw();

  virtual const std::map<G4String,G4AttDef>* GetAttDefs() const;
  virtual std::vector<G4AttValue>* CreateAttValues() const;

  virtual void Print();

  // Deposited energy
  inline void AddDepositedEnergy(G4double energy) {fDepositedEnergy += energy;}
  inline G4double GetDepositedEnergy() const {return fDepositedEnergy;}

  // Position vector
  inline void SetPosition(G4ThreeVector position) {fPosition = position;}
  inline G4ThreeVector GetPosition() const {return fPosition;}

  // Rotation matrix
  inline void SetRotation(G4RotationMatrix rotation) {fRotation = rotation;}
  inline G4RotationMatrix GetRotation() const {return fRotation;}

  // Logical volume
  inline void SetLogicalVolume(G4LogicalVolume* volume) {pLogicalVolume = volume;}
  inline const G4LogicalVolume* GetLogicalVolume() const {return pLogicalVolume;}
  
private:
  
  /// Cell Number
  G4int fCellNumber;
  G4int fCellID;
  G4double fTiming;
  G4double fDepositedEnergy;
  G4ThreeVector fPosition;
  G4RotationMatrix fRotation;
  const G4LogicalVolume* pLogicalVolume;
  
};

typedef G4THitsCollection<BETAG4BigcalHit> BETAG4BigcalHitsCollection;

extern G4Allocator<BETAG4BigcalHit> BETAG4BigcalHitAllocator;

inline void* BETAG4BigcalHit::operator new(size_t)
{
  void* aHit;
  aHit = (void*)BETAG4BigcalHitAllocator.MallocSingle();
  return aHit;
}

inline void BETAG4BigcalHit::operator delete(void* aHit)
{
  BETAG4BigcalHitAllocator.FreeSingle((BETAG4BigcalHit*) aHit);
}

#endif


