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

/**  A PMT hit contains all accumulated photons for a single PMT.
 */
class BETAG4PMTHit : public G4VHit {

   public:
      BETAG4PMTHit(G4int id = -1);
      ~BETAG4PMTHit();

      inline void *operator new(size_t);

      inline void operator delete(void *aHit);

      void Draw();

      void Print();

      void AddPhoton() {fPhotons++;}

      G4int GetNumberOfPhotons() const {return fPhotons;}

      G4double GetNumberOfPhotoElectrons() { return (((G4double)fPhotons)*fQuantumEfficiency);}


      G4int fTubeNumber;
      G4double fTiming;

      G4ThreeVector  localPos;
      bool fTimingHit;

      //  void SetLocalPosition(G4ThreeVector*pos) {localPos
      // Add a Photon
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

      /**
       * over ride operator for std::sort
       */
      // inline bool operator < ( const BETAG4PMTHit & a) const
      //     {
      //         return (Gtime < a.Gtime);
      //     }

      G4int fPhotons;

   private:
      G4double     fQuantumEfficiency;

      // Data members

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


