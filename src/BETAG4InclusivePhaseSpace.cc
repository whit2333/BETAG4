#include "BETAG4InclusivePhaseSpace.hh"
#include "TMath.h"
#define PI 3.141592654

BETAG4InclusivePhaseSpace::BETAG4InclusivePhaseSpace() :
  fMaxTheta(PI*60.0/180.0),fMinTheta(PI*20.0/180.0),
  fMaxPhi(45.0*PI/180.0),fMinPhi(-45.0*PI/180.0),
  fMaxEnergy(6.0),fMinEnergy(1.0)
{

}

BETAG4InclusivePhaseSpace::~BETAG4InclusivePhaseSpace() {

}

double BETAG4InclusivePhaseSpace::GetPhaseSpaceVolume() 
{
return(((TMath::Power(fMinEnergy,3) - TMath::Power(fMaxEnergy,3))*(fMinPhi - fMaxPhi)*(TMath::Cos(fMinTheta) - TMath::Cos(fMaxTheta)))/3.);
}

