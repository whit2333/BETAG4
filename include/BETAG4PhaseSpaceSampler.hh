#ifndef BETAG4PhaseSpaceSampler_H
#define BETAG4PhaseSpaceSampler_H 1
#include "BETAG4EventPhaseSpace.hh"

#include "BETAG4EventPhaseSpace.hh"

class BETAG4PhaseSpaceSampler  {
public: 
BETAG4PhaseSpaceSampler(BETAG4EventPhaseSpace *);
~BETAG4PhaseSpaceSampler();

int SampleEvent();

BETAG4EventPhaseSpace * fPhaseSpace;
};

#endif

