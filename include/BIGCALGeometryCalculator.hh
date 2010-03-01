/**
 * \class BIGCALGeometryCalculator
 *
 * \addtogroup BIGCAL
 *
 *
 * \brief Singleton class for defining BigCal Geometries.
 *
 * This is a singleton class containing a connection to the MySQL database where
 * the various geometry parameters are stored. It also handles the calibration
 * coefficients for each block. 
 *
 * Contact: whit@jlab.org
 *
 *
 */
#ifndef BIGCALGC_h
#define BIGCALGC_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TObject.h>
#include <TSQLRow.h>
#include <TSQLServer.h>
#include <TSQLResult.h>


class BIGCALGeometryCalculator  :public TObject {
public :
   static BIGCALGeometryCalculator* GetCalculator();
//   BIGCALGeometryCalculator();
virtual   ~BIGCALGeometryCalculator();

   double getProtCellX(int i);// coords centered in protvino frame
   double getProtCellY(int j);

   double getRCSCellX(int i);// coords centered in rcs frame
   double getRCSCellY(int j);

   double getRCSCellX_BCCoords(int i);// coords centered in BC frame
   double getRCSCellY_BCCoords(int j);

   double getProtCellX_BCCoords(int i);// coords centered in BC frame
   double getProtCellY_BCCoords(int j);

   double rcsXSize,rcsYSize,protXSize,protYSize;

   double rcsProtYSeparation; // center to center;
   double rcsProtXSeparation; // horizontal displacement

   double rcsCellSize;
   double protCellSize;

   double rcsCellZSize;
   double protCellZSize;

   double bcVerticalOffset ;
   double bcYSize ;
   double bcXSize ;

   double bigcalFace;

   int protNx, protNy, rcsNy,rcsNx;

   double getBlockXij_BCCoords(int i,int j) ;
   double getBlockYij_BCCoords(int i,int j) ;


// evenly divide the mirrors starting in the middle
   double cherenkovMirrorXSize[8];
   double cherenkovMirrorYSize[8];
   double cherenkovMirrorPositionX[8];
   double cherenkovMirrorPositionY[8];

   double bc_cc[56*32];

private :

	   BIGCALGeometryCalculator();  // Private so that it can  not be called
//	   BIGCALGeometryCalculator(BIGCALGeometryCalculator const&){};             // copy constructor is private
//	   BIGCALGeometryCalculator& operator=(BIGCALGeometryCalculator const&){};  // assignment operator is private
   static BIGCALGeometryCalculator* bcGeoCalcInstance;

   double arcsCellSize;
   double aprotCellSize;
   ClassDef(BIGCALGeometryCalculator,3)
};


#endif
