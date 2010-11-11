#ifndef BETAG4PMTHIT_HH
#define BETAG4PMTHIT_HH

#include "G4Allocator.hh"
#include "G4LogicalVolume.hh"
#include "G4RotationMatrix.hh"
#include "G4ThreeVector.hh"
#include "G4THitsCollection.hh"
#include "G4Transform3D.hh"
#include "G4VHit.hh"
#include <iostream>
#include <algorithm>
#include <vector>

/**
 *  A PMT hit for a single photon 
 *  NOTE: this might not be the best way for dealing with the light, that is,
 *  counting every photon. However, it allows us to gain more information like 
 *  positioning, time, and energy. 
 *  This can then be used to present the data in a more realistic way. 
 *  For example, looking at the time difference in photons hitting the tube, we can conclude that for suffuciently large times
 *  there will be two TDC hits. 
 */
class BETAG4PMTHit : public G4VHit {

public:
// Constructors
  BETAG4PMTHit();
  BETAG4PMTHit(G4int tubeNumber);


  // Destructor
  virtual ~BETAG4PMTHit();
  
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
//  G4int mirrorNumber;
  G4ThreeVector  localPos;
  G4double Gtime;

/**
 * over ride operator for std::sort
 */
// inline bool operator < ( const BETAG4PMTHit & a) const
//     {
//         return (Gtime < a.Gtime);
//     }


private:
  
  // Data members

  G4int fPhotons;
//  G4ThreeVector fPosition;
//  G4RotationMatrix fRotation;
//  const G4LogicalVolume* pLogicalVolume;
  
  
};



typedef G4THitsCollection<BETAG4PMTHit> BETAG4PMTHitsCollection;

/*
//struct aHitTimeCompare{
	bool operator()( BETAG4PMTHit * a, BETAG4PMTHit * b) {
		return (a->Gtime/ns)<(b->Gtime/ns);
	}
//} BETAG4PMTHitTimeComparezzz;
//...
//std::sort(taxpayers.begin(),taxpayers.end(),BETAG4PMTHitTimeCompare);
*/
extern G4Allocator<BETAG4PMTHit> BETAG4PMTHitAllocator;

inline void* BETAG4PMTHit::operator new(size_t)
{
  void* aHit;
  aHit = (void*)BETAG4PMTHitAllocator.MallocSingle();
  return aHit;
}

inline void BETAG4PMTHit::operator delete(void* aHit)
{
  BETAG4PMTHitAllocator.FreeSingle((BETAG4PMTHit*) aHit);
}

#endif


