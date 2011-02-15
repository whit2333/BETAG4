#ifndef BETASteppingAction_H
#define BETASteppingAction_H 1

#include "globals.hh"
#include "G4UserSteppingAction.hh"

class BETASteppingAction : public G4UserSteppingAction
{
  public:
    BETASteppingAction();
    virtual ~BETASteppingAction();

    virtual void UserSteppingAction(const G4Step*);

  private:
  G4int fLastTrackIdInTracker;
  G4int fConsecutiveSteps;
  G4int fLastStepNumberInTracker;

};

#endif

