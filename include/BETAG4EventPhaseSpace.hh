#ifndef BETAG4EventPhaseSpace_h
#define BETAG4EventPhaseSpace_h 1

#include "globals.hh"


/** ABC for an event phase space
 *
 *   \ingroup EventGen
 */
class BETAG4EventPhaseSpace 
{
  public:
  BETAG4EventPhaseSpace() {;};
virtual ~BETAG4EventPhaseSpace(){;};

/**
 *  Concrete implementation should calculate the volume
 */
  virtual double GetPhaseSpaceVolume() = 0;

  virtual int GetDimension() {return(0);} ;


  double fMaxEnergy,fMinEnergy;
  double fMaxTheta, fMinTheta;
  double fMaxPhi,   fMinPhi;

  private:


};


#endif

