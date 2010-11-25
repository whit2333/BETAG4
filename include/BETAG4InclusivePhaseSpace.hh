#ifndef BETAG4InclusivePhaseSpace_h
#define BETAG4InclusivePhaseSpace_h 1
#include "BETAG4EventPhaseSpace.hh"
#include "globals.hh"

/// \brief ABC for event phase space

/**
 *  ABC for an event phase space
 */
class BETAG4InclusivePhaseSpace : public BETAG4EventPhaseSpace
{
  public:

  BETAG4InclusivePhaseSpace();

  virtual ~BETAG4InclusivePhaseSpace();

  inline virtual int GetDimension() {return(3);}

/**
 *  returns the value of  \f$ \int p^2 sin(\theta) d\theta d\phi dp \f$
 */ 
  virtual double GetPhaseSpaceVolume();


  double fMaxEnergy,fMinEnergy;
  double fMaxTheta, fMinTheta;
  double fMaxPhi,   fMinPhi;


};

#endif

