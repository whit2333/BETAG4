#include "BETAG4PhaseSpaceSampler.hh"
#include "BETAG4InclusiveDiffXSec.hh"

#include <TROOT.h>

#include <TObject.h>

#include <iostream>

BETAG4PhaseSpaceSampler::BETAG4PhaseSpaceSampler(BETAG4EventPhaseSpace * aPhaseSpace) {
fPhaseSpace=aPhaseSpace;

}

BETAG4PhaseSpaceSampler::~BETAG4PhaseSpaceSampler() {

}

int BETAG4PhaseSpaceSampler::SampleEvent() 
{

return(0);
}


