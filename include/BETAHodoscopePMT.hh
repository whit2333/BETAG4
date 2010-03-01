//
// ********************************************************************
// * DISCLAIMER                                                       *
// *                                                                  *
// * The following disclaimer summarizes all the specific disclaimers *
// * of contributors to this software. The specific disclaimers,which *
// * govern, are listed with their locations in:                      *
// *   http://cern.ch/geant4/license                                  *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.                                                             *
// *                                                                  *
// * This  code  implementation is the  intellectual property  of the *
// * GEANT4 collaboration.                                            *
// * By copying,  distributing  or modifying the Program (or any work *
// * based  on  the Program)  you indicate  your  acceptance of  this *
// * statement, and all its terms.                                    *
// ********************************************************************
//
// $Id:$
//
// Jane Tinslay - adapted from A01 example
//
#ifndef BETAHodoscopePMT_HH
#define BETAHodoscopePMT_HH
#include "fstream"
#include <gsl/gsl_interp.h>
#include <gsl/gsl_errno.h>
#include <gsl/gsl_spline.h>

#include "G4VSensitiveDetector.hh"
#include "BETAHodoscopePMTHit.hh"

class G4HCofThisEvent;
class G4TouchableHistory;
class G4Step;

class BETAHodoscopePMT : public G4VSensitiveDetector {

public:

  // Constructor
  BETAHodoscopePMT( G4String name);

  // Destructor
  virtual ~BETAHodoscopePMT();
  
  // Methods
  virtual void Initialize(G4HCofThisEvent* hitsCollectionOfThisEvent);
  
  virtual G4bool ProcessHits(G4Step* aStep,G4TouchableHistory* history);

  virtual void EndOfEvent(G4HCofThisEvent* hitsCollectionOfThisEvent);
  
private:
  
  G4double QE(G4double photonEnergy);
  G4double lambda[15];
  G4double sensitivity[15];
  // Data members
  G4int HCID;
  BETAHodoscopePMTHitsCollection* fHitsCollection;
  G4String  detname;
  G4int pmt;
gsl_interp * alloc;

};




#endif

