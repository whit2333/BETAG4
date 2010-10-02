#ifndef BETASteppingVerbose_h
#define BETASteppingVerbose_h 1

class BETASteppingVerbose;

#include "G4SteppingVerbose.hh"

//____________________________________________________
class BETASteppingVerbose : public G4SteppingVerbose
{
 public:   

   BETASteppingVerbose();
  ~BETASteppingVerbose();

   void StepInfo();
   void TrackingStarted();

};


//____________________________________________________
#endif

