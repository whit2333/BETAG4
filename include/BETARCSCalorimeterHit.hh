#ifndef BETARCSCalorimeterHIT_HH
#define BETARCSCalorimeterHIT_HH

#include "G4Allocator.hh"
#include "G4LogicalVolume.hh"
#include "G4RotationMatrix.hh"
#include "G4ThreeVector.hh"
#include "G4THitsCollection.hh"
#include "G4Transform3D.hh"
#include "G4VHit.hh"

class G4AttDef;
class G4AttValue;

class BETARCSCalorimeterHit : public G4VHit {

public:
  
  // Constructors
  BETARCSCalorimeterHit();
  BETARCSCalorimeterHit(G4int id);

  // Destructor
  virtual ~BETARCSCalorimeterHit();
  
  inline void *operator new(size_t);
  inline void operator delete(void *aHit);

  // Methods
  virtual void Draw();

  virtual const std::map<G4String,G4AttDef>* GetAttDefs() const;
  virtual std::vector<G4AttValue>* CreateAttValues() const;

  virtual void Print();

  /// Deposited energy
   void AddDepositedEnergy(G4double energy) {fDepositedEnergy += energy;}
  /// Deposited energy
   G4double GetDepositedEnergy() const {return fDepositedEnergy;}

  ///Position vector
   void SetPosition(G4ThreeVector position) {fPosition = position;}
   G4ThreeVector GetPosition() const {return fPosition;}

  /// Rotation matrix
   void SetRotation(G4RotationMatrix rotation) {fRotation = rotation;}
   G4RotationMatrix GetRotation() const {return fRotation;}

  /// Logical volume
   void SetLogicalVolume(G4LogicalVolume* volume) {pLogicalVolume = volume;}
   const G4LogicalVolume* GetLogicalVolume() const {return pLogicalVolume;}
  
private:
  
  // Data members
  G4int fCellID;
  G4double fDepositedEnergy;
  G4ThreeVector fPosition;
  G4RotationMatrix fRotation;
  const G4LogicalVolume* pLogicalVolume;
  
};

typedef G4THitsCollection<BETARCSCalorimeterHit> BETARCSCalorimeterHitsCollection;

extern G4Allocator<BETARCSCalorimeterHit> BETARCSCalorimeterHitAllocator;

inline void* BETARCSCalorimeterHit::operator new(size_t)
{
  void* aHit;
  aHit = (void*)BETARCSCalorimeterHitAllocator.MallocSingle();
  return aHit;
}

inline void BETARCSCalorimeterHit::operator delete(void* aHit)
{
  BETARCSCalorimeterHitAllocator.FreeSingle((BETARCSCalorimeterHit*) aHit);
}

#endif


