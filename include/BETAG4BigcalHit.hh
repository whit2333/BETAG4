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


/** New Bigcal hit class for GEANT4 Sensitive detector
 *
 */
class BETAG4BigcalHit : public G4VHit {
  public:
  BETAG4BigcalHit(G4int id = -1);
   ~BETAG4BigcalHit();
  
  inline void *operator new(size_t);
  inline void operator delete(void *aHit);

   void Draw();

   const std::map<G4String,G4AttDef>* GetAttDefs() const;
   std::vector<G4AttValue>* CreateAttValues() const;

   void Print();

/** Add Deposited energy
 */
   void AddDepositedEnergy(G4double energy) {fDepositedEnergy += energy;}

/** Get Deposited energy
 */
  G4double GetDepositedEnergy() const {return fDepositedEnergy;}

/**  Set Position vector
 */
   void SetPosition(G4ThreeVector position) {fPosition = position;}

/**  Get Position vector
 */
   G4ThreeVector GetPosition() const {return fPosition;}

/**  Set Rotation matrix
 */
   void SetRotation(G4RotationMatrix rotation) {fRotation = rotation;}

/**  Get Rotation matrix
 */
   G4RotationMatrix GetRotation() const {return fRotation;}

/**  Set Logical Volume
 */
   void SetLogicalVolume(G4LogicalVolume* volume) {pLogicalVolume = volume;}
   const G4LogicalVolume* GetLogicalVolume() const {return pLogicalVolume;}
  
  /// Cell Number
  G4int fCellNumber;
  G4int fCellID;
  G4double fTiming;
  G4double fDepositedEnergy;
  G4ThreeVector fPosition;
  G4RotationMatrix fRotation;
  const G4LogicalVolume* pLogicalVolume;
  G4int fNHits;
  bool fTimingHit;
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


