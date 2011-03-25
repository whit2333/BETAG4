#ifndef BETAFakePlaneHIT_HH
#define BETAFakePlaneHIT_HH

#include "G4Allocator.hh"
#include "G4LogicalVolume.hh"
#include "G4RotationMatrix.hh"
#include "G4ThreeVector.hh"
#include "G4THitsCollection.hh"
#include "G4Transform3D.hh"
#include "G4VHit.hh"



/**  Concrete hit class for a very thin (unrealistic) detector plane
 *
 *  The PID is from the numbering scheme of the PDG.
 *  For example:
 *  <ul>
 *  <li>11 : electron</li>
 *  <li>-11 : positron</li>
 *  <li>13 : muon (negative charge) </li>
 *  <li>14 : anti-muon (positive charge) </li>
 *  <li>22 : photon (gamma)</li>
 *  <li>111 : neutral pion</li>
 *  <li>211 : pion+ (positive charge) </li>
 *  <li>-211 : pion- (negative charge) </li>
 *  </ul>
 *  
 */
class BETAFakePlaneHit : public G4VHit {
public:
   G4int         fPID;
/// World Position
   G4ThreeVector fPosition; 
   G4ThreeVector fLocalPosition; 
   G4ThreeVector fMomentum;
   G4double      fEnergy;

public:
  BETAFakePlaneHit(G4int id = 0);
  ~BETAFakePlaneHit();

  inline void *operator new(size_t);
  inline void operator delete(void *aHit);
  virtual void Draw();
  virtual void Print();

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


