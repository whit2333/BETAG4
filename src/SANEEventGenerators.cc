#include "SANEEventGenerators.hh"


//____________________________________________________________________
ConeEventGenerator::ConeEventGenerator() {
      fCentralTheta = 40.0;
      fCentralPhi = 0.0;
      fCentralEnergy = 2.00;
      fDeltaTheta = 2.0;
      fDeltaPhi = 20.0;
      fDeltaEnergy = 0.400; //GeV
      fUpstreamPosition = 0.0;//cm
   }

ConeEventGenerator::~ConeEventGenerator() {
}