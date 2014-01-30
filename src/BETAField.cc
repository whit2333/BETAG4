#include "BETAField.hh"
#include <iostream>
#include <fstream>
#include <iomanip>
#include "globals.hh"
#include "G4ThreeVector.hh"
#include "TGraph2D.h"
#include "TGraph.h"
#include "TROOT.h"
#include "TFile.h"
#include "TNetFile.h"
#include "TRandom.h"
#include "TTree.h"
#include "TBranch.h"
#include "TClonesArray.h"
#include "TStopwatch.h"
#include "TH1F.h"
#include "TObjArray.h"
#include "TH1.h"
#include "TH2.h"
#include "TProfile.h"
#include "TRint.h"
#include "TApplication.h"
#include "TCanvas.h"
#include "TStyle.h"


//___________________________________________________________________
BETAField::BETAField() {
   fUVAMagnet = new UVAOxfordMagneticField();
}
//___________________________________________________________________

BETAField::~BETAField()
{
  if(fUVAMagnet) delete fUVAMagnet ;
}
//___________________________________________________________________

void BETAField::GetFieldValue ( const double Point[4],double *Bfield ) const
{
  const double newPoint[4]={Point[0]/cm,Point[1]/cm,Point[2]/cm,Point[3]/cm};

  fUVAMagnet->GetFieldValue(newPoint,Bfield);
  Bfield[0]=Bfield[0]*tesla;
  Bfield[1]=Bfield[1]*tesla;
  Bfield[2]=Bfield[2]*tesla;
}
//___________________________________________________________________


