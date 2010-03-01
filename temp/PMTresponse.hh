#ifndef BETAField_H
#define BETAField_H 1
#include "PMTresponse.hh"
#include <fstream.h>  

#define fieldDataPoints 61

class PMTresponse 
{
  public:
    PMTresponse();
    ~PMTresponse();
    void GetNumberPhotoelectrons();

  private:
    ofstream output_file;
   
};

#endif
