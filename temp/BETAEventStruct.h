//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Sun Aug  2 01:35:50 2009 by ROOT version 5.18/00
// from TTree h9502/sane_ntuple
// found on file: ntup/sane72994.1.1.root
//////////////////////////////////////////////////////////

#ifndef BETAEvent_h
#define BETAEvent_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TSelector.h>

class BETAEvent : public TSelector {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain

   // Declaration of leaf types
   Int_t           bc_prot_nhit;
   Int_t           bc_prot_ix[900];   //[bc_prot_nhit]
   Int_t           bc_prot_iy[900];   //[bc_prot_nhit]
   Int_t           bc_prot_adc[900];   //[bc_prot_nhit]
   Int_t           bc_rcs_nhit;
   Int_t           bc_rcs_ix[900];   //[bc_rcs_nhit]
   Int_t           bc_rcs_iy[900];   //[bc_rcs_nhit]
   Int_t           bc_rcs_adc[900];   //[bc_rcs_nhit]
   Double_t        tcharge;
   Double_t        charge2s;
   Double_t        polarea;
   Int_t           hel_p_scaler;
   Int_t           hel_n_scaler;
   Int_t           hel_p_trig;
   Int_t           hel_n_trig;
   Double_t        dtime_p;
   Double_t        dtime_n;
   Float_t         hms_p;
   Float_t         hms_e;
   Float_t         hms_theta;
   Float_t         hms_phi;
   Float_t         hsxfp_s;
   Float_t         hsyfp_s;
   Float_t         hsxpfp_s;
   Float_t         hsypfp_s;
   Float_t         hms_xtar;
   Float_t         hms_ytar;
   Float_t         hms_yptar;
   Float_t         hms_xptar;
   Float_t         hms_delta;
   Float_t         hms_start;
   Float_t         hsshtrk_s;
   Float_t         hsshsum_s;
   Float_t         hsbeta_s;
   Float_t         rast_x;
   Float_t         rast_y;
   Float_t         slow_rast_x;
   Float_t         slow_rast_y;
   Float_t         sem_x;
   Float_t         sem_y;
   Int_t           i_helicity;
   Float_t         hms_cer_npe1;
   Float_t         hms_cer_npe2;
   Float_t         hms_cer_adc1;
   Float_t         hms_cer_adc2;
   Float_t         T_trghms;
   Float_t         T_trgbig;
   Float_t         T_trgpi0;
   Float_t         T_trgbeta;
   Float_t         T_trgcoin1;
   Float_t         T_trgcoin2;

   // List of branches
   TBranch        *b_bc_prot_nhit;   //!
   TBranch        *b_bc_prot_ix;   //!
   TBranch        *b_bc_prot_iy;   //!
   TBranch        *b_bc_prot_adc;   //!
   TBranch        *b_bc_rcs_nhit;   //!
   TBranch        *b_bc_rcs_ix;   //!
   TBranch        *b_bc_rcs_iy;   //!
   TBranch        *b_bc_rcs_adc;   //!
   TBranch        *b_tcharge;   //!
   TBranch        *b_charge2s;   //!
   TBranch        *b_polarea;   //!
   TBranch        *b_hel_p_scaler;   //!
   TBranch        *b_hel_n_scaler;   //!
   TBranch        *b_hel_p_trig;   //!
   TBranch        *b_hel_n_trig;   //!
   TBranch        *b_dtime_p;   //!
   TBranch        *b_dtime_n;   //!
   TBranch        *b_hms_p;   //!
   TBranch        *b_hms_e;   //!
   TBranch        *b_hms_theta;   //!
   TBranch        *b_hms_phi;   //!
   TBranch        *b_hsxfp_s;   //!
   TBranch        *b_hsyfp_s;   //!
   TBranch        *b_hsxpfp_s;   //!
   TBranch        *b_hsypfp_s;   //!
   TBranch        *b_hms_xtar;   //!
   TBranch        *b_hms_ytar;   //!
   TBranch        *b_hms_yptar;   //!
   TBranch        *b_hms_xptar;   //!
   TBranch        *b_hms_delta;   //!
   TBranch        *b_hms_start;   //!
   TBranch        *b_hsshtrk_s;   //!
   TBranch        *b_hsshsum_s;   //!
   TBranch        *b_hsbeta_s;   //!
   TBranch        *b_rast_x;   //!
   TBranch        *b_rast_y;   //!
   TBranch        *b_slow_rast_x;   //!
   TBranch        *b_slow_rast_y;   //!
   TBranch        *b_sem_x;   //!
   TBranch        *b_sem_y;   //!
   TBranch        *b_i_helicity;   //!
   TBranch        *b_hms_cer_npe1;   //!
   TBranch        *b_hms_cer_npe2;   //!
   TBranch        *b_hms_cer_adc1;   //!
   TBranch        *b_hms_cer_adc2;   //!
   TBranch        *b_T_trghms;   //!
   TBranch        *b_T_trgbig;   //!
   TBranch        *b_T_trgpi0;   //!
   TBranch        *b_T_trgbeta;   //!
   TBranch        *b_T_trgcoin1;   //!
   TBranch        *b_T_trgcoin2;   //!

   BETAEvent(TTree * /*tree*/ =0) { }
   virtual ~BETAEvent() { }
   virtual Int_t   Version() const { return 2; }
   virtual void    Begin(TTree *tree);
   virtual void    SlaveBegin(TTree *tree);
   virtual void    Init(TTree *tree);
   virtual Bool_t  Notify();
   virtual Bool_t  Process(Long64_t entry);
   virtual Int_t   GetEntry(Long64_t entry, Int_t getall = 0) { return fChain ? fChain->GetTree()->GetEntry(entry, getall) : 0; }
   virtual void    SetOption(const char *option) { fOption = option; }
   virtual void    SetObject(TObject *obj) { fObject = obj; }
   virtual void    SetInputList(TList *input) { fInput = input; }
   virtual TList  *GetOutputList() const { return fOutput; }
   virtual void    SlaveTerminate();
   virtual void    Terminate();

   ClassDef(BETAEvent,0);
};

#endif

#ifdef BETAEvent_cxx
void BETAEvent::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("bc_prot_nhit", &bc_prot_nhit, &b_bc_prot_nhit);
   fChain->SetBranchAddress("bc_prot_ix", bc_prot_ix, &b_bc_prot_ix);
   fChain->SetBranchAddress("bc_prot_iy", bc_prot_iy, &b_bc_prot_iy);
   fChain->SetBranchAddress("bc_prot_adc", bc_prot_adc, &b_bc_prot_adc);
   fChain->SetBranchAddress("bc_rcs_nhit", &bc_rcs_nhit, &b_bc_rcs_nhit);
   fChain->SetBranchAddress("bc_rcs_ix", bc_rcs_ix, &b_bc_rcs_ix);
   fChain->SetBranchAddress("bc_rcs_iy", bc_rcs_iy, &b_bc_rcs_iy);
   fChain->SetBranchAddress("bc_rcs_adc", bc_rcs_adc, &b_bc_rcs_adc);
   fChain->SetBranchAddress("tcharge", &tcharge, &b_tcharge);
   fChain->SetBranchAddress("charge2s", &charge2s, &b_charge2s);
   fChain->SetBranchAddress("polarea", &polarea, &b_polarea);
   fChain->SetBranchAddress("hel_p_scaler", &hel_p_scaler, &b_hel_p_scaler);
   fChain->SetBranchAddress("hel_n_scaler", &hel_n_scaler, &b_hel_n_scaler);
   fChain->SetBranchAddress("hel_p_trig", &hel_p_trig, &b_hel_p_trig);
   fChain->SetBranchAddress("hel_n_trig", &hel_n_trig, &b_hel_n_trig);
   fChain->SetBranchAddress("dtime_p", &dtime_p, &b_dtime_p);
   fChain->SetBranchAddress("dtime_n", &dtime_n, &b_dtime_n);
   fChain->SetBranchAddress("hms_p", &hms_p, &b_hms_p);
   fChain->SetBranchAddress("hms_e", &hms_e, &b_hms_e);
   fChain->SetBranchAddress("hms_theta", &hms_theta, &b_hms_theta);
   fChain->SetBranchAddress("hms_phi", &hms_phi, &b_hms_phi);
   fChain->SetBranchAddress("hsxfp_s", &hsxfp_s, &b_hsxfp_s);
   fChain->SetBranchAddress("hsyfp_s", &hsyfp_s, &b_hsyfp_s);
   fChain->SetBranchAddress("hsxpfp_s", &hsxpfp_s, &b_hsxpfp_s);
   fChain->SetBranchAddress("hsypfp_s", &hsypfp_s, &b_hsypfp_s);
   fChain->SetBranchAddress("hms_xtar", &hms_xtar, &b_hms_xtar);
   fChain->SetBranchAddress("hms_ytar", &hms_ytar, &b_hms_ytar);
   fChain->SetBranchAddress("hms_yptar", &hms_yptar, &b_hms_yptar);
   fChain->SetBranchAddress("hms_xptar", &hms_xptar, &b_hms_xptar);
   fChain->SetBranchAddress("hms_delta", &hms_delta, &b_hms_delta);
   fChain->SetBranchAddress("hms_start", &hms_start, &b_hms_start);
   fChain->SetBranchAddress("hsshtrk_s", &hsshtrk_s, &b_hsshtrk_s);
   fChain->SetBranchAddress("hsshsum_s", &hsshsum_s, &b_hsshsum_s);
   fChain->SetBranchAddress("hsbeta_s", &hsbeta_s, &b_hsbeta_s);
   fChain->SetBranchAddress("rast_x", &rast_x, &b_rast_x);
   fChain->SetBranchAddress("rast_y", &rast_y, &b_rast_y);
   fChain->SetBranchAddress("slow_rast_x", &slow_rast_x, &b_slow_rast_x);
   fChain->SetBranchAddress("slow_rast_y", &slow_rast_y, &b_slow_rast_y);
   fChain->SetBranchAddress("sem_x", &sem_x, &b_sem_x);
   fChain->SetBranchAddress("sem_y", &sem_y, &b_sem_y);
   fChain->SetBranchAddress("i_helicity", &i_helicity, &b_i_helicity);
   fChain->SetBranchAddress("hms_cer_npe1", &hms_cer_npe1, &b_hms_cer_npe1);
   fChain->SetBranchAddress("hms_cer_npe2", &hms_cer_npe2, &b_hms_cer_npe2);
   fChain->SetBranchAddress("hms_cer_adc1", &hms_cer_adc1, &b_hms_cer_adc1);
   fChain->SetBranchAddress("hms_cer_adc2", &hms_cer_adc2, &b_hms_cer_adc2);
   fChain->SetBranchAddress("T_trghms", &T_trghms, &b_T_trghms);
   fChain->SetBranchAddress("T_trgbig", &T_trgbig, &b_T_trgbig);
   fChain->SetBranchAddress("T_trgpi0", &T_trgpi0, &b_T_trgpi0);
   fChain->SetBranchAddress("T_trgbeta", &T_trgbeta, &b_T_trgbeta);
   fChain->SetBranchAddress("T_trgcoin1", &T_trgcoin1, &b_T_trgcoin1);
   fChain->SetBranchAddress("T_trgcoin2", &T_trgcoin2, &b_T_trgcoin2);
}

Bool_t BETAEvent::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

#endif // #ifdef BETAEvent_cxx
