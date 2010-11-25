#ifndef BETAG4InclusiveDiffXSec_H
#define BETAG4InclusiveDiffXSec_H 1 

#include "Math/IFunction.h"
//#include "Fit/ParameterSettings.h"
//#include "Fit/FitResult.h"

class BETAG4InclusiveDiffXSec : public ROOT::Math::IBaseFunctionMultiDim 
{
public:

  BETAG4InclusiveDiffXSec();

  virtual ~BETAG4InclusiveDiffXSec();

  virtual BETAG4InclusiveDiffXSec* Clone() const;

   unsigned int NDim() const { return fnDim; }


 double operator() (const double * x)  ;

//   RooArgList* GetFloatParamList() { return _floatParamList; }
//   RooArgList* GetConstParamList() { return _constParamList; }
//   RooArgList* GetInitFloatParamList() { return _initFloatParamList; }
//   RooArgList* GetInitConstParamList() { return _initConstParamList; }

//   void SetEvalErrorWall(Bool_t flag) { _doEvalErrorWall = flag ; }
//   void SetPrintEvalErrors(Int_t numEvalErrors) { _printEvalErrors = numEvalErrors ; }
//   Bool_t SetLogFile(const char* inLogfile);
//   ofstream* GetLogFile() { return _logfile; }
//   void SetVerbose(Bool_t flag=kTRUE) { _verbose = flag ; }
// 
//   Double_t& GetMaxFCN() { return _maxFCN; }
//   Int_t GetNumInvalidNLL() { return _numBadNLL; }
// 
//   Bool_t Synchronize(std::vector<ROOT::Fit::ParameterSettings>& parameters, 
// 		     Bool_t optConst, Bool_t verbose);
//   void BackProp(const ROOT::Fit::FitResult &results);  
//   void ApplyCovarianceMatrix(TMatrixDSym& V); 

 private:

//   Double_t GetPdfParamVal(Int_t index);
//   Double_t GetPdfParamErr(Int_t index);
//   void SetPdfParamErr(Int_t index, Double_t value);
//   void ClearPdfParamAsymErr(Int_t index);
//   void SetPdfParamErr(Int_t index, Double_t loVal, Double_t hiVal);
// 
//   inline Bool_t SetPdfParamVal(const Int_t &index, const Double_t &value) const;


   double DoEval(const double * x) const;  

private:

  int fnDim;

  
//   RooAbsReal *_funct;
//   RooMinimizer *_context;

//   mutable double _maxFCN;
//   mutable int _numBadNLL;
//   mutable int _printEvalErrors;
//   Bool_t _doEvalErrorWall;

//   ofstream *_logfile;
//   bool _verbose;

//   RooArgList* _floatParamList;
//   RooArgList* _constParamList;
//   RooArgList* _initFloatParamList;
//   RooArgList* _initConstParamList;

};


#endif

