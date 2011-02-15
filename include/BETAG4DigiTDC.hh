#ifndef BETAG4DigiTDC_HH
#define BETAG4DigiTDC_HH

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

/**
 *
 */
class BETAG4DigiTDC : public G4VDigi {
  public:
/** C'tor */
  BETAG4DigiTDC(G4int channel=-1) {
    fChannelNumber = channel;
    fChannelsFirstHit=false;
    fTrueValue=0.0;
    fTDCValue=0;
    fADCValue=0;
    fNumberOfHits = 0;
  };

/** d'tor */
  ~BETAG4DigiTDC(){
  };

/** */

  void *operator new(size_t);

  void operator delete(void *aDigi);

/** Draw Hit : not implemented */
  virtual void Draw(){
  };

/** Print Hit */
  virtual void Print(){
    std::cout << "Channel " << fChannelNumber << " : bit " << fTDCValue << " \n";
    std::cout << fNumberOfHits << " hits   "  <<  " :     " << fTrueValue << " ns\n";
  };
  G4int fNumberOfHits;
  G4int fChannelNumber;
  G4int fTDCValue;
  G4double fTrueValue;
  G4int fADCValue;
  bool  fChannelsFirstHit;
private:

};
//_______________________________________________________//

typedef G4TDigiCollection<BETAG4DigiTDC> BETAG4DigiTDCCollection;

extern G4Allocator<BETAG4DigiTDC> BETAG4DigiTDCAllocator;

inline void * BETAG4DigiTDC::operator new(size_t)
{
  void* aDigi;
  aDigi = (void*)BETAG4DigiTDCAllocator.MallocSingle();
  return aDigi;
}

inline void BETAG4DigiTDC::operator delete(void* aDigi)
{
  BETAG4DigiTDCAllocator.FreeSingle((BETAG4DigiTDC*) aDigi);
}

#endif


