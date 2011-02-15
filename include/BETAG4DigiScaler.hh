#ifndef BETAG4DigiScaler_HH
#define BETAG4DigiScaler_HH

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
class BETAG4DigiScaler : public G4VDigi {
  public:
/**
 *  Constructor
 */
  BETAG4DigiScaler(G4int channel=-1) {
    fChannelNumber = channel;
  };


/**
 *  Destructor
 */
  ~BETAG4DigiScaler(){
  };

  void *operator new(size_t);

  void operator delete(void *aDigi);

  // Methods
  virtual void Draw(){
  };

  virtual void Print(){
  };

  G4int fChannelNumber;
  G4int fScalerValue;

private:

};
//_______________________________________________________//

typedef G4TDigiCollection<BETAG4DigiScaler> BETAG4DigiScalerCollection;

extern G4Allocator<BETAG4DigiScaler> BETAG4DigiScalerAllocator;

inline void * BETAG4DigiScaler::operator new(size_t)
{
  void* aDigi;
  aDigi = (void*)BETAG4DigiScalerAllocator.MallocSingle();
  return aDigi;
}

inline void BETAG4DigiScaler::operator delete(void* aDigi)
{
  BETAG4DigiScalerAllocator.FreeSingle((BETAG4DigiScaler*) aDigi);
}

#endif


