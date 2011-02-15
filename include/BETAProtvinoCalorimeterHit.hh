#ifndef BETAProtvinoCalorimeterHIT_HH
#define BETAProtvinoCalorimeterHIT_HH

#include "G4Allocator.hh"
#include "G4LogicalVolume.hh"
#include "G4RotationMatrix.hh"
#include "G4ThreeVector.hh"
#include "G4THitsCollection.hh"
#include "G4Transform3D.hh"
#include "G4VHit.hh"

class G4AttDef;
class G4AttValue;

class BETAProtvinoCalorimeterHit : public G4VHit {

public:
  
  // Constructors
  BETAProtvinoCalorimeterHit();
  BETAProtvinoCalorimeterHit(G4int id);

  // Destructor
   ~BETAProtvinoCalorimeterHit();
  
  inline void *operator new(size_t);
  inline void operator delete(void *aHit);

  // Methods
   void Draw();

   const std::map<G4String,G4AttDef>* GetAttDefs() const;
   std::vector<G4AttValue>* CreateAttValues() const;

   void Print();

  // Deposited energy
   void AddDepositedEnergy(G4double energy) {fDepositedEnergy += energy;}
   G4double GetDepositedEnergy() const {return fDepositedEnergy;}

  // Position vector
   void SetPosition(G4ThreeVector position) {fPosition = position;}
   G4ThreeVector GetPosition() const {return fPosition;}

  // Rotation matrix
   void SetRotation(G4RotationMatrix rotation) {fRotation = rotation;}
   G4RotationMatrix GetRotation() const {return fRotation;}

  // Logical volume
   void SetLogicalVolume(G4LogicalVolume* volume) {pLogicalVolume = volume;}
   const G4LogicalVolume* GetLogicalVolume() const {return pLogicalVolume;}
    // Data members
  G4int fCellID;
  G4double fDepositedEnergy;
  G4ThreeVector fPosition;
  G4RotationMatrix fRotation;
  const G4LogicalVolume* pLogicalVolume;
private:

};

typedef G4THitsCollection<BETAProtvinoCalorimeterHit> BETAProtvinoCalorimeterHitsCollection;

extern G4Allocator<BETAProtvinoCalorimeterHit> BETAProtvinoCalorimeterHitAllocator;

inline void* BETAProtvinoCalorimeterHit::operator new(size_t)
{
  void* aHit;
  aHit = (void*)BETAProtvinoCalorimeterHitAllocator.MallocSingle();
  return aHit;
}

inline void BETAProtvinoCalorimeterHit::operator delete(void* aHit)
{
  BETAProtvinoCalorimeterHitAllocator.FreeSingle((BETAProtvinoCalorimeterHit*) aHit);
}

#endif


