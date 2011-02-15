#include "BIGCALGeometryCalculator.hh"
#include <TObject.h>
#include <iostream>
#include <TSQLServer.h>
#include <TSQLResult.h>

// Global static pointer used to ensure a single instance of the class.
BIGCALGeometryCalculator * BIGCALGeometryCalculator::bcGeoCalcInstance = NULL; 

ClassImp(BIGCALGeometryCalculator)

/** This function is called to create an instance of the class.
 *  Calling the constructor publicly is not allowed. The constructor
 *  is private and is only called by this function.
 */
BIGCALGeometryCalculator * BIGCALGeometryCalculator::GetCalculator() {
if (!bcGeoCalcInstance)   // Only allow one instance of class to be generated.
bcGeoCalcInstance = new BIGCALGeometryCalculator();	 
return bcGeoCalcInstance;
}

BIGCALGeometryCalculator::BIGCALGeometryCalculator(){
   TSQLServer * db = TSQLServer::Connect("mysql://localhost/SANE", "sane", "secret");
   TSQLResult * res;
   TSQLRow * row;
   res = db->Query("SELECT * FROM bigcal_calibration_coef");
   int nrows = res->GetRowCount();
   int nfields = res->GetFieldCount();
   for (int i = 0; i < nrows; i++) 
   {
      row = res->Next();
      for (int j = 2; j < nfields; j++) 
      {
         bc_cc[j-2] = TString(  row->GetField(j)  ).Atof();
      }  
    printf("\n");
    delete row;
   }

   rcsNx = 30;
   rcsNy = 24;
   protNx = 32;
   protNy = 32;
   rcsCellSize = 4.02167 ;
   protCellSize = 3.81 ;
// as viewed from the back towards the target, the cells are right justified.
//
   rcsXSize = rcsCellSize*rcsNx;
   rcsYSize = rcsCellSize*rcsNy;
   protXSize = protCellSize*protNx;
   protYSize = protCellSize*protNy;
   rcsCellZSize = 40.0;
   protCellZSize = 45.0;
   rcsProtYSeparation = protYSize/2.0 + rcsYSize/2.0 ;
   rcsProtXSeparation = (protXSize - rcsXSize)/2.0 ;

   bcVerticalOffset = (protYSize + rcsYSize)/2.0 - rcsYSize; // add to overall vertical placement
   bcYSize = (protYSize + rcsYSize);
   bcXSize = protXSize ;
//bcYSize = (protYSize + rcsYSize);

   bigcalFace = 345.0; // from target


// Cherenkov Mirrors
// evenly divide the mirrors starting in the middle (4, 5, 6 and 7)
// rectangular geometry definitions.... NOT VERY ACCURATE... but good enough for now.
// These defined below will mostly be used for boolean geometry so overlapping isn't
// a major problem, but it would be nice if they were mutually exclusive and covered
// all of bigcal.



// X sizes

   double middleMirrorHeight = 58.0 ;

//Spherical
   cherenkovMirrorXSize[0] = bcXSize/2.0; //cm
   cherenkovMirrorXSize[2] = bcXSize/2.0; //cm
   cherenkovMirrorXSize[4] = bcXSize/2.0; //cm
   cherenkovMirrorXSize[6] = bcXSize/2.0; //cm
//Toroidal
   cherenkovMirrorXSize[1] = bcXSize/2.0; //cm
   cherenkovMirrorXSize[3] = bcXSize/2.0; //cm
   cherenkovMirrorXSize[5] = bcXSize/2.0; //cm
   cherenkovMirrorXSize[7] = bcXSize/2.0; //cm

// Y sizes
//Spherical
   cherenkovMirrorYSize[0] = bcYSize/2.0-middleMirrorHeight; //cm
   cherenkovMirrorYSize[2] = middleMirrorHeight; //cm
   cherenkovMirrorYSize[4] = middleMirrorHeight; //cm
   cherenkovMirrorYSize[6] = bcYSize/2.0-middleMirrorHeight; //cm
//Toroidal
   cherenkovMirrorYSize[1] = bcYSize/2.0-middleMirrorHeight; //cm
   cherenkovMirrorYSize[3] = middleMirrorHeight; //cm
   cherenkovMirrorYSize[5] = middleMirrorHeight; //cm
   cherenkovMirrorYSize[7] = bcYSize/2.0-middleMirrorHeight; //cm


// Viewing from behind ( down stream ) mirror 0 is in the lower right corner.

// X positions
//Spherical
   cherenkovMirrorPositionX[0] = bcXSize/4.0; //cm
   cherenkovMirrorPositionX[2] = bcXSize/4.0; //cm
   cherenkovMirrorPositionX[4] = bcXSize/4.0; //cm
   cherenkovMirrorPositionX[6] = bcXSize/4.0; //cm
//Toroidal
   cherenkovMirrorPositionX[1] = -1.0*bcXSize/4.0; //cm
   cherenkovMirrorPositionX[3] = -1.0*bcXSize/4.0; //cm
   cherenkovMirrorPositionX[5] = -1.0*bcXSize/4.0; //cm
   cherenkovMirrorPositionX[7] = -1.0*bcXSize/4.0; //cm

// Y positions
//Spherical
   cherenkovMirrorPositionY[0] = -1.0*(cherenkovMirrorYSize[2]+cherenkovMirrorYSize[0]/2.0); //cm
   cherenkovMirrorPositionY[2] = -1.0*cherenkovMirrorYSize[2]/2.0; //cm
   cherenkovMirrorPositionY[4] = cherenkovMirrorYSize[4]/2.0; //cm
   cherenkovMirrorPositionY[6] = cherenkovMirrorYSize[4]+cherenkovMirrorYSize[6]/2.0; //cm
//Toroidal
   cherenkovMirrorPositionY[1] = -1.0*(cherenkovMirrorYSize[3]+cherenkovMirrorYSize[1]/2.0); //cm
   cherenkovMirrorPositionY[3] = -1.0*cherenkovMirrorYSize[3]/2.0; //cm
   cherenkovMirrorPositionY[5] = cherenkovMirrorYSize[5]/2.0; //cm
   cherenkovMirrorPositionY[7] = cherenkovMirrorYSize[5]+cherenkovMirrorYSize[7]/2.0; //cm

}

BIGCALGeometryCalculator::~BIGCALGeometryCalculator(){
   ;
}

/*
 * Get the X coordinate (in Prot coords.) of the column (which the interger argument). Coordinates are in
 * an imaginary box centered on the Protvino (lower) section of BigCal
 */
double BIGCALGeometryCalculator::getProtCellX(int i) {
   if (i<1 || i>32) std::cout << " error in prot x index " << i << "\n";
   return( ((double)i)*protCellSize -protXSize/2.0-protCellSize/2.0);
}

/*
 * Get the Y coordinate (in Prot coords.) of the row (which the interger argument). Coordinates are in
 * an imaginary box centered on the Protvino (lower) section of BigCal
 */
double BIGCALGeometryCalculator::getProtCellY(int j) {
   if (j<1 || j>32) std::cout << " error in prot y index " << j << "\n";
   return( ((double)j)*protCellSize -protYSize/2.0-protCellSize/2.0);
}

/*
 * Get the X coordinate (in RCS coords.) of the column (which the interger argument). Coordinates are in
 * an imaginary box centered on the RCS (upper) section of BigCal
 */
double BIGCALGeometryCalculator::getRCSCellX(int i) {
   if (i<1 || i>30) std::cout << " error in rcs x\n";
   return( ((double)i)*rcsCellSize -rcsXSize/2.0-rcsCellSize/2.0);

}

/*
 * Get the Y coordinate (in RCS coords.) of the row (which the interger argument). Coordinates are in
 * an imaginary box centered on the RCS (upper) section of BigCal
 */
double BIGCALGeometryCalculator::getRCSCellY(int j) {
   if (j<1 || j>24) std::cout << " error in rcs y\n";
   return( ((double)j)*rcsCellSize -rcsYSize/2.0-rcsCellSize/2.0);

}


/// Using horizontal center of protvino as BC coordinate x=0
/// even though there is a right justifcation of the blocks
/// NEED TO INSERT BEST SURVEY DATA HERE

/*
 * Get the X coordinate of a cell in the Protvino section in
 * <a href="http://quarks.temple.edu/~whit/SANE/BigCal/docs"> BigCal Coordinate System  </a>
 * of the column (which the interger argument).
 */
double BIGCALGeometryCalculator::getProtCellX_BCCoords(int i) {
   if (i<1 || i>32) std::cout << " \n error in PROT X cell# : " << i;
   return( ((double)i)*protCellSize -protXSize/2.0-protCellSize/2.0);
}

/*
 * Get the Y coordinate of a cell in the Protvino section in
 * <a href="http://quarks.temple.edu/~whit/SANE/BigCal/docs"> BigCal Coordinate System  </a>
 * of the column (which the interger argument).
 */
double BIGCALGeometryCalculator::getProtCellY_BCCoords(int j) {
   if (j<1 || j>32) std::cout << " \n error in PROT Y cell# : " << j;
   return( ((double)j)*protCellSize -protYSize/2.0-protCellSize/2.0+ bcVerticalOffset -protYSize/2.0);

}

/*
 * Get the X coordinate of a cell in the RCS section in
 * <a href="http://quarks.temple.edu/~whit/SANE/BigCal/docs"> BigCal Coordinate System  </a>
 * of the column (which the interger argument).
 */
double BIGCALGeometryCalculator::getRCSCellX_BCCoords(int i) {
   if (i<1 || i>30) std::cout <<  " \n error in RCS  X cell# : " << i;
   return( ((double)i)*rcsCellSize -rcsXSize/2.0-rcsCellSize/2.0+rcsProtXSeparation);
}

/*
 * Get the Y coordinate of a cell in the RCS section in
 * <a href="http://quarks.temple.edu/~whit/SANE/BigCal/docs"> BigCal Coordinate System  </a>
 * of the column (which the interger argument).
 */
double BIGCALGeometryCalculator::getRCSCellY_BCCoords(int j) {
   if (j<1 || j>24) std::cout <<  " \n error in RCS  Y cell# : " << j;
   return( ((double)j)*rcsCellSize -rcsYSize/2.0-rcsCellSize/2.0 + bcVerticalOffset+ rcsYSize/2.0);
}

/*
 * Get the X coordinate of a cell (i,j) in
 * <a href="http://quarks.temple.edu/~whit/SANE/BigCal/docs"> BigCal Coordinate System  </a>
 * of the column (which the interger argument).
 * NOTE: The arguments are for all of bigcal (i,j)=({1-32},{1-56} , however, there are two columns
 * in the rcs section (i={31,32},j={33-56}) where there is actually no block!
 */
double BIGCALGeometryCalculator::getBlockXij_BCCoords(int i,int j) {
   double result;
   if (j>32) { // RCS
      result = getRCSCellX_BCCoords(i);
   } else {   //PROT
      result = getProtCellX_BCCoords(i);
   }
   return( result);
}

/*
 * Get the Y coordinate of a cell (i,j) in
 * <a href="http://quarks.temple.edu/~whit/SANE/BigCal/docs"> BigCal Coordinate System  </a>
 * of the column (which the interger argument).
 * NOTE: The arguments are for all of bigcal (i,j)=({1-32},{1-56} , however, there are two columns
 * in the rcs section (i={31,32},j={33-56}) where there is actually no block!
 */
double BIGCALGeometryCalculator::getBlockYij_BCCoords(int i,int j) {
   double result;
   if (j>32) { // RCS
      result = getRCSCellY_BCCoords(j-32);
      if (i>30) printf(" error ");
   } else { // PROT
      result = getProtCellY_BCCoords(j);
      if (i>32) printf(" error ");
   }
   return( result);
}



