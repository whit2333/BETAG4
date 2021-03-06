#ifndef BETAG4DigiADC_HH
#define BETAG4DigiADC_HH

#include "G4Allocator.hh"
#include "G4LogicalVolume.hh"
#include "G4RotationMatrix.hh"
#include "G4ThreeVector.hh"
#include "G4TDigiCollection.hh"
#include "G4Transform3D.hh"
#include "G4VDigi.hh"
#include <iostream>
#include <algorithm>
#include <vector>

/**  Simulates an ADC channel.
 */
class BETAG4DigiADC : public G4VDigi {

   public:
      G4int    fChannelNumber;
      G4int    fADCValue;
      G4double fTrueValue;

   public:
      BETAG4DigiADC(G4int channel=-1);
      virtual ~BETAG4DigiADC();

      void* operator new(size_t);
      void  operator delete(void *aDigi);
      void Draw();
      void Print();

};
//______________________________________________________________________________

typedef G4TDigiCollection<BETAG4DigiADC> BETAG4DigiADCCollection;

extern G4Allocator<BETAG4DigiADC> BETAG4DigiADCAllocator;

//______________________________________________________________________________
inline void * BETAG4DigiADC::operator new(size_t)
{
   void* aDigi;
   aDigi = (void*)BETAG4DigiADCAllocator.MallocSingle();
   return aDigi;
}
//______________________________________________________________________________
inline void BETAG4DigiADC::operator delete(void* aDigi)
{
   BETAG4DigiADCAllocator.FreeSingle((BETAG4DigiADC*) aDigi);
}

#endif


