#ifndef BETAField_H
#define BETAField_H 1
#include "globals.hh"

#include "CLHEP/Units/SystemOfUnits.h"
#include "CLHEP/Units/PhysicalConstants.h"
using namespace CLHEP;

#include "G4MagneticField.hh"
#include "Riostream.h"
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
#include "UVAOxfordMagneticField.h"
#include "TMath.h"
#include <vector>
#include "G4RotationMatrix.hh"
#include "G4ThreeVector.hh"
#define fieldDataPoints 31161

/**
 *  A wrapper class for G4MagneticField implementation. Implements the UVA Polarized Ammonia target magnetic field
 */
class BETAField :public G4MagneticField /*, public UVAOxfordMagneticField */
{
  public:
    BETAField();
    ~BETAField();

    UVAOxfordMagneticField * fUVAMagnet;

   void SetScaleFactor(double scale) { fUVAMagnet->SetScaleFactor(scale); };
   double GetScaleFactor() {return( (double)fUVAMagnet->GetScaleFactor() ); };

/// Calls GetFieldValue 
  void GetFieldValue( const  double Point[4],
                               double *Bfield ) const;

/**
 * Creates a plot of the field component given in the argument
 * component can be "z","Z","r", or "R"
 */
    void LookAtField(char * component);

//    void TestInterpolation();
//    void ReadDataFile();
//    void setPolarizationAngle(G4double angle) {polAngle = angle;}
//    void switchPolarization();
/*    G4ThreeVector fBVector;*/
  private:
  struct fieldDataPoint 
  {
   G4double z,r,Bz,Br;
  };

   double RawBZ[fieldDataPoints];
   double RawBR[fieldDataPoints];
   double * BzRpoint[221];
   double * BrRpoint[221];

   std::vector<fieldDataPoint> fieldData;
    G4double ** BzFieldRaw;
    G4double ** BrFieldRaw;
    G4double *** BF;
//     G4double * locations[fieldDataPoints*fieldDataPoints];
//     G4double zandrValue[2*fieldDataPoints*fieldDataPoints];
//     G4double * * R[fieldDataPoints];
    G4int numPoints;
    G4double* * Field;
};

#endif
