#include "BETAG4DigiADC.hh"

G4Allocator<BETAG4DigiADC> BETAG4DigiADCAllocator;

//______________________________________________________________________________
BETAG4DigiADC::BETAG4DigiADC(G4int channel) {
   fChannelNumber = channel;
   fADCValue      = 0;
   fTrueValue     = 0.0;
}
//______________________________________________________________________________
BETAG4DigiADC::~BETAG4DigiADC(){
}
//______________________________________________________________________________
void BETAG4DigiADC::Draw(){
}
//______________________________________________________________________________
void BETAG4DigiADC::Print(){
   std::cout << " ADC " << fChannelNumber << "\t" << fADCValue << "\t" << fTrueValue << std::endl;
}
//______________________________________________________________________________

