#define rawBETAEvent_cxx
#include "rawBETAEvent.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>

/**
 *
 * rawBETAEvent Class is to be used to convert raw or simulation data to the same
 * raw data format, mainly BETAEvent. 
 * 
 * \note 
 * This event class creates very large entries (no dynamic memory allocation)
 * It is meant to be used in transition or for the montecarlo simulations.  
 *
 */

ClassImp(rawBETAEvent)

rawBETAEvent::rawBETAEvent() : fChain(0)
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
 std::cout << "  Created new rawBETAEvent \n " ;

}

rawBETAEvent::~rawBETAEvent()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}


