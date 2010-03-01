//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Mon Nov 23 09:52:32 2009 by ROOT version 5.25/02
// from TTree Detectors/Detector Tree
// found on file: ../data/beta.run.23851.root
//////////////////////////////////////////////////////////

#ifndef DQ1_h
#define DQ1_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TSelector.h>
#include <TH2F.h>
#include <TH1F.h>
#include <TCanvas.h>

class DQ1 : public TSelector {
public :

TH2F * mc_e_VS_mc_x ;
TH2F * mc_e_VS_mc_y ;
TH2F * mc_x_VS_mc_y ;
TH1F * energyDiff;
TH1F * thetaInit;
TH1F * phiInit;

   TTree          *fChain;   //!pointer to the analyzed TTree or TChain

   // Declaration of leaf types
 //BETAEvent       *aBetaEvent;
   UInt_t          fUniqueID;
   UInt_t          fBits;
   Int_t           bc_prot_nhit;
   Int_t           bc_prot_ix[100];
   Int_t           bc_prot_iy[100];
   Int_t           bc_prot_adc[100];
   Int_t           bc_rcs_nhit;
   Int_t           bc_rcs_ix[100];
   Int_t           bc_rcs_iy[100];
   Int_t           bc_rcs_adc[100];
   Int_t           y1t_hit;
   Int_t           y1t_row[100];
   Int_t           y1t_tdc[100];
   Float_t         y1t_y[100];
   Int_t           y2t_hit;
   Int_t           y2t_row[100];
   Int_t           y2t_tdc[100];
   Float_t         y2t_y[100];
   Int_t           x1t_hit;
   Int_t           x1t_row[100];
   Int_t           x1t_tdc[100];
   Float_t         x1t_x[900];
   Int_t           cer_hit;
   Int_t           cer_num[50];
   Int_t           cer_tdc[50];
   Int_t           cer_adcc[50];
   Int_t           ceradc_hit;
   Int_t           ceradc_num[15];
   Int_t           cer_adc[15];
   Int_t           luc_hit;
   Int_t           luc_row[90];
   Int_t           ladc_pos[90];
   Int_t           ladc_neg[90];
   Int_t           ltdc_pos[90];
   Int_t           ltdc_neg[90];
   Float_t         luc_y[90];
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
   Int_t           mc_nhit_init;
   Float_t         mc_xpos_init[4];
   Float_t         mc_ypos_init[4];
   Float_t         mc_zpos_init[4];
   Float_t         mc_theta_init[4];
   Float_t         mc_phi_init[4];
   Float_t         mc_e_init[4];
   Float_t         mc_pid_init[4];
   Int_t           mc_nhit_bcplane;
   Float_t         mc_x_bcplane[10];
   Float_t         mc_y_bcplane[10];
   Float_t         mc_pid_bcplane[10];
   Float_t         mc_theta_bcplane[10];
   Float_t         mc_e_bcplane[10];
   Float_t         mc_xmom_bcplane[10];
   Float_t         mc_ymom_bcplane[10];
   Float_t         mc_zmom_bcplane[10];

   // List of branches
   TBranch        *b_aBetaEvent_fUniqueID;   //!
   TBranch        *b_aBetaEvent_fBits;   //!
   TBranch        *b_aBetaEvent_bc_prot_nhit;   //!
   TBranch        *b_aBetaEvent_bc_prot_ix;   //!
   TBranch        *b_aBetaEvent_bc_prot_iy;   //!
   TBranch        *b_aBetaEvent_bc_prot_adc;   //!
   TBranch        *b_aBetaEvent_bc_rcs_nhit;   //!
   TBranch        *b_aBetaEvent_bc_rcs_ix;   //!
   TBranch        *b_aBetaEvent_bc_rcs_iy;   //!
   TBranch        *b_aBetaEvent_bc_rcs_adc;   //!
   TBranch        *b_aBetaEvent_y1t_hit;   //!
   TBranch        *b_aBetaEvent_y1t_row;   //!
   TBranch        *b_aBetaEvent_y1t_tdc;   //!
   TBranch        *b_aBetaEvent_y1t_y;   //!
   TBranch        *b_aBetaEvent_y2t_hit;   //!
   TBranch        *b_aBetaEvent_y2t_row;   //!
   TBranch        *b_aBetaEvent_y2t_tdc;   //!
   TBranch        *b_aBetaEvent_y2t_y;   //!
   TBranch        *b_aBetaEvent_x1t_hit;   //!
   TBranch        *b_aBetaEvent_x1t_row;   //!
   TBranch        *b_aBetaEvent_x1t_tdc;   //!
   TBranch        *b_aBetaEvent_x1t_x;   //!
   TBranch        *b_aBetaEvent_cer_hit;   //!
   TBranch        *b_aBetaEvent_cer_num;   //!
   TBranch        *b_aBetaEvent_cer_tdc;   //!
   TBranch        *b_aBetaEvent_cer_adcc;   //!
   TBranch        *b_aBetaEvent_ceradc_hit;   //!
   TBranch        *b_aBetaEvent_ceradc_num;   //!
   TBranch        *b_aBetaEvent_cer_adc;   //!
   TBranch        *b_aBetaEvent_luc_hit;   //!
   TBranch        *b_aBetaEvent_luc_row;   //!
   TBranch        *b_aBetaEvent_ladc_pos;   //!
   TBranch        *b_aBetaEvent_ladc_neg;   //!
   TBranch        *b_aBetaEvent_ltdc_pos;   //!
   TBranch        *b_aBetaEvent_ltdc_neg;   //!
   TBranch        *b_aBetaEvent_luc_y;   //!
   TBranch        *b_aBetaEvent_tcharge;   //!
   TBranch        *b_aBetaEvent_charge2s;   //!
   TBranch        *b_aBetaEvent_polarea;   //!
   TBranch        *b_aBetaEvent_hel_p_scaler;   //!
   TBranch        *b_aBetaEvent_hel_n_scaler;   //!
   TBranch        *b_aBetaEvent_hel_p_trig;   //!
   TBranch        *b_aBetaEvent_hel_n_trig;   //!
   TBranch        *b_aBetaEvent_dtime_p;   //!
   TBranch        *b_aBetaEvent_dtime_n;   //!
   TBranch        *b_aBetaEvent_hms_p;   //!
   TBranch        *b_aBetaEvent_hms_e;   //!
   TBranch        *b_aBetaEvent_hms_theta;   //!
   TBranch        *b_aBetaEvent_hms_phi;   //!
   TBranch        *b_aBetaEvent_hsxfp_s;   //!
   TBranch        *b_aBetaEvent_hsyfp_s;   //!
   TBranch        *b_aBetaEvent_hsxpfp_s;   //!
   TBranch        *b_aBetaEvent_hsypfp_s;   //!
   TBranch        *b_aBetaEvent_hms_xtar;   //!
   TBranch        *b_aBetaEvent_hms_ytar;   //!
   TBranch        *b_aBetaEvent_hms_yptar;   //!
   TBranch        *b_aBetaEvent_hms_xptar;   //!
   TBranch        *b_aBetaEvent_hms_delta;   //!
   TBranch        *b_aBetaEvent_hms_start;   //!
   TBranch        *b_aBetaEvent_hsshtrk_s;   //!
   TBranch        *b_aBetaEvent_hsshsum_s;   //!
   TBranch        *b_aBetaEvent_hsbeta_s;   //!
   TBranch        *b_aBetaEvent_rast_x;   //!
   TBranch        *b_aBetaEvent_rast_y;   //!
   TBranch        *b_aBetaEvent_slow_rast_x;   //!
   TBranch        *b_aBetaEvent_slow_rast_y;   //!
   TBranch        *b_aBetaEvent_sem_x;   //!
   TBranch        *b_aBetaEvent_sem_y;   //!
   TBranch        *b_aBetaEvent_i_helicity;   //!
   TBranch        *b_aBetaEvent_hms_cer_npe1;   //!
   TBranch        *b_aBetaEvent_hms_cer_npe2;   //!
   TBranch        *b_aBetaEvent_hms_cer_adc1;   //!
   TBranch        *b_aBetaEvent_hms_cer_adc2;   //!
   TBranch        *b_aBetaEvent_T_trghms;   //!
   TBranch        *b_aBetaEvent_T_trgbig;   //!
   TBranch        *b_aBetaEvent_T_trgpi0;   //!
   TBranch        *b_aBetaEvent_T_trgbeta;   //!
   TBranch        *b_aBetaEvent_T_trgcoin1;   //!
   TBranch        *b_aBetaEvent_T_trgcoin2;   //!
   TBranch        *b_aBetaEvent_mc_nhit_init;   //!
   TBranch        *b_aBetaEvent_mc_xpos_init;   //!
   TBranch        *b_aBetaEvent_mc_ypos_init;   //!
   TBranch        *b_aBetaEvent_mc_zpos_init;   //!
   TBranch        *b_aBetaEvent_mc_theta_init;   //!
   TBranch        *b_aBetaEvent_mc_phi_init;   //!
   TBranch        *b_aBetaEvent_mc_e_init;   //!
   TBranch        *b_aBetaEvent_mc_pid_init;   //!
   TBranch        *b_aBetaEvent_mc_nhit_bcplane;   //!
   TBranch        *b_aBetaEvent_mc_x_bcplane;   //!
   TBranch        *b_aBetaEvent_mc_y_bcplane;   //!
   TBranch        *b_aBetaEvent_mc_pid_bcplane;   //!
   TBranch        *b_aBetaEvent_mc_theta_bcplane;   //!
   TBranch        *b_aBetaEvent_mc_e_bcplane;   //!
   TBranch        *b_aBetaEvent_mc_xmom_bcplane;   //!
   TBranch        *b_aBetaEvent_mc_ymom_bcplane;   //!
   TBranch        *b_aBetaEvent_mc_zmom_bcplane;   //!

   DQ1(TTree * /*tree*/ =0) { }
   virtual ~DQ1() { }
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

   ClassDef(DQ1,0);
};

#endif

#ifdef DQ1_cxx
void DQ1::Init(TTree *tree)
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

   fChain->SetBranchAddress("fUniqueID", &fUniqueID, &b_aBetaEvent_fUniqueID);
   fChain->SetBranchAddress("fBits", &fBits, &b_aBetaEvent_fBits);
   fChain->SetBranchAddress("bc_prot_nhit", &bc_prot_nhit, &b_aBetaEvent_bc_prot_nhit);
   fChain->SetBranchAddress("bc_prot_ix[100]", bc_prot_ix, &b_aBetaEvent_bc_prot_ix);
   fChain->SetBranchAddress("bc_prot_iy[100]", bc_prot_iy, &b_aBetaEvent_bc_prot_iy);
   fChain->SetBranchAddress("bc_prot_adc[100]", bc_prot_adc, &b_aBetaEvent_bc_prot_adc);
   fChain->SetBranchAddress("bc_rcs_nhit", &bc_rcs_nhit, &b_aBetaEvent_bc_rcs_nhit);
   fChain->SetBranchAddress("bc_rcs_ix[100]", bc_rcs_ix, &b_aBetaEvent_bc_rcs_ix);
   fChain->SetBranchAddress("bc_rcs_iy[100]", bc_rcs_iy, &b_aBetaEvent_bc_rcs_iy);
   fChain->SetBranchAddress("bc_rcs_adc[100]", bc_rcs_adc, &b_aBetaEvent_bc_rcs_adc);
   fChain->SetBranchAddress("y1t_hit", &y1t_hit, &b_aBetaEvent_y1t_hit);
   fChain->SetBranchAddress("y1t_row[100]", y1t_row, &b_aBetaEvent_y1t_row);
   fChain->SetBranchAddress("y1t_tdc[100]", y1t_tdc, &b_aBetaEvent_y1t_tdc);
   fChain->SetBranchAddress("y1t_y[100]", y1t_y, &b_aBetaEvent_y1t_y);
   fChain->SetBranchAddress("y2t_hit", &y2t_hit, &b_aBetaEvent_y2t_hit);
   fChain->SetBranchAddress("y2t_row[100]", y2t_row, &b_aBetaEvent_y2t_row);
   fChain->SetBranchAddress("y2t_tdc[100]", y2t_tdc, &b_aBetaEvent_y2t_tdc);
   fChain->SetBranchAddress("y2t_y[100]", y2t_y, &b_aBetaEvent_y2t_y);
   fChain->SetBranchAddress("x1t_hit", &x1t_hit, &b_aBetaEvent_x1t_hit);
   fChain->SetBranchAddress("x1t_row[100]", x1t_row, &b_aBetaEvent_x1t_row);
   fChain->SetBranchAddress("x1t_tdc[100]", x1t_tdc, &b_aBetaEvent_x1t_tdc);
   fChain->SetBranchAddress("x1t_x[900]", x1t_x, &b_aBetaEvent_x1t_x);
   fChain->SetBranchAddress("cer_hit", &cer_hit, &b_aBetaEvent_cer_hit);
   fChain->SetBranchAddress("cer_num[50]", cer_num, &b_aBetaEvent_cer_num);
   fChain->SetBranchAddress("cer_tdc[50]", cer_tdc, &b_aBetaEvent_cer_tdc);
   fChain->SetBranchAddress("cer_adcc[50]", cer_adcc, &b_aBetaEvent_cer_adcc);
   fChain->SetBranchAddress("ceradc_hit", &ceradc_hit, &b_aBetaEvent_ceradc_hit);
   fChain->SetBranchAddress("ceradc_num[15]", ceradc_num, &b_aBetaEvent_ceradc_num);
   fChain->SetBranchAddress("cer_adc[15]", cer_adc, &b_aBetaEvent_cer_adc);
   fChain->SetBranchAddress("luc_hit", &luc_hit, &b_aBetaEvent_luc_hit);
   fChain->SetBranchAddress("luc_row[90]", luc_row, &b_aBetaEvent_luc_row);
   fChain->SetBranchAddress("ladc_pos[90]", ladc_pos, &b_aBetaEvent_ladc_pos);
   fChain->SetBranchAddress("ladc_neg[90]", ladc_neg, &b_aBetaEvent_ladc_neg);
   fChain->SetBranchAddress("ltdc_pos[90]", ltdc_pos, &b_aBetaEvent_ltdc_pos);
   fChain->SetBranchAddress("ltdc_neg[90]", ltdc_neg, &b_aBetaEvent_ltdc_neg);
   fChain->SetBranchAddress("luc_y[90]", luc_y, &b_aBetaEvent_luc_y);
   fChain->SetBranchAddress("tcharge", &tcharge, &b_aBetaEvent_tcharge);
   fChain->SetBranchAddress("charge2s", &charge2s, &b_aBetaEvent_charge2s);
   fChain->SetBranchAddress("polarea", &polarea, &b_aBetaEvent_polarea);
   fChain->SetBranchAddress("hel_p_scaler", &hel_p_scaler, &b_aBetaEvent_hel_p_scaler);
   fChain->SetBranchAddress("hel_n_scaler", &hel_n_scaler, &b_aBetaEvent_hel_n_scaler);
   fChain->SetBranchAddress("hel_p_trig", &hel_p_trig, &b_aBetaEvent_hel_p_trig);
   fChain->SetBranchAddress("hel_n_trig", &hel_n_trig, &b_aBetaEvent_hel_n_trig);
   fChain->SetBranchAddress("dtime_p", &dtime_p, &b_aBetaEvent_dtime_p);
   fChain->SetBranchAddress("dtime_n", &dtime_n, &b_aBetaEvent_dtime_n);
   fChain->SetBranchAddress("hms_p", &hms_p, &b_aBetaEvent_hms_p);
   fChain->SetBranchAddress("hms_e", &hms_e, &b_aBetaEvent_hms_e);
   fChain->SetBranchAddress("hms_theta", &hms_theta, &b_aBetaEvent_hms_theta);
   fChain->SetBranchAddress("hms_phi", &hms_phi, &b_aBetaEvent_hms_phi);
   fChain->SetBranchAddress("hsxfp_s", &hsxfp_s, &b_aBetaEvent_hsxfp_s);
   fChain->SetBranchAddress("hsyfp_s", &hsyfp_s, &b_aBetaEvent_hsyfp_s);
   fChain->SetBranchAddress("hsxpfp_s", &hsxpfp_s, &b_aBetaEvent_hsxpfp_s);
   fChain->SetBranchAddress("hsypfp_s", &hsypfp_s, &b_aBetaEvent_hsypfp_s);
   fChain->SetBranchAddress("hms_xtar", &hms_xtar, &b_aBetaEvent_hms_xtar);
   fChain->SetBranchAddress("hms_ytar", &hms_ytar, &b_aBetaEvent_hms_ytar);
   fChain->SetBranchAddress("hms_yptar", &hms_yptar, &b_aBetaEvent_hms_yptar);
   fChain->SetBranchAddress("hms_xptar", &hms_xptar, &b_aBetaEvent_hms_xptar);
   fChain->SetBranchAddress("hms_delta", &hms_delta, &b_aBetaEvent_hms_delta);
   fChain->SetBranchAddress("hms_start", &hms_start, &b_aBetaEvent_hms_start);
   fChain->SetBranchAddress("hsshtrk_s", &hsshtrk_s, &b_aBetaEvent_hsshtrk_s);
   fChain->SetBranchAddress("hsshsum_s", &hsshsum_s, &b_aBetaEvent_hsshsum_s);
   fChain->SetBranchAddress("hsbeta_s", &hsbeta_s, &b_aBetaEvent_hsbeta_s);
   fChain->SetBranchAddress("rast_x", &rast_x, &b_aBetaEvent_rast_x);
   fChain->SetBranchAddress("rast_y", &rast_y, &b_aBetaEvent_rast_y);
   fChain->SetBranchAddress("slow_rast_x", &slow_rast_x, &b_aBetaEvent_slow_rast_x);
   fChain->SetBranchAddress("slow_rast_y", &slow_rast_y, &b_aBetaEvent_slow_rast_y);
   fChain->SetBranchAddress("sem_x", &sem_x, &b_aBetaEvent_sem_x);
   fChain->SetBranchAddress("sem_y", &sem_y, &b_aBetaEvent_sem_y);
   fChain->SetBranchAddress("i_helicity", &i_helicity, &b_aBetaEvent_i_helicity);
   fChain->SetBranchAddress("hms_cer_npe1", &hms_cer_npe1, &b_aBetaEvent_hms_cer_npe1);
   fChain->SetBranchAddress("hms_cer_npe2", &hms_cer_npe2, &b_aBetaEvent_hms_cer_npe2);
   fChain->SetBranchAddress("hms_cer_adc1", &hms_cer_adc1, &b_aBetaEvent_hms_cer_adc1);
   fChain->SetBranchAddress("hms_cer_adc2", &hms_cer_adc2, &b_aBetaEvent_hms_cer_adc2);
   fChain->SetBranchAddress("T_trghms", &T_trghms, &b_aBetaEvent_T_trghms);
   fChain->SetBranchAddress("T_trgbig", &T_trgbig, &b_aBetaEvent_T_trgbig);
   fChain->SetBranchAddress("T_trgpi0", &T_trgpi0, &b_aBetaEvent_T_trgpi0);
   fChain->SetBranchAddress("T_trgbeta", &T_trgbeta, &b_aBetaEvent_T_trgbeta);
   fChain->SetBranchAddress("T_trgcoin1", &T_trgcoin1, &b_aBetaEvent_T_trgcoin1);
   fChain->SetBranchAddress("T_trgcoin2", &T_trgcoin2, &b_aBetaEvent_T_trgcoin2);
   fChain->SetBranchAddress("mc_nhit_init", &mc_nhit_init, &b_aBetaEvent_mc_nhit_init);
   fChain->SetBranchAddress("mc_xpos_init[4]", mc_xpos_init, &b_aBetaEvent_mc_xpos_init);
   fChain->SetBranchAddress("mc_ypos_init[4]", mc_ypos_init, &b_aBetaEvent_mc_ypos_init);
   fChain->SetBranchAddress("mc_zpos_init[4]", mc_zpos_init, &b_aBetaEvent_mc_zpos_init);
   fChain->SetBranchAddress("mc_theta_init[4]", mc_theta_init, &b_aBetaEvent_mc_theta_init);
   fChain->SetBranchAddress("mc_phi_init[4]", mc_phi_init, &b_aBetaEvent_mc_phi_init);
   fChain->SetBranchAddress("mc_e_init[4]", mc_e_init, &b_aBetaEvent_mc_e_init);
   fChain->SetBranchAddress("mc_pid_init[4]", mc_pid_init, &b_aBetaEvent_mc_pid_init);
   fChain->SetBranchAddress("mc_nhit_bcplane", &mc_nhit_bcplane, &b_aBetaEvent_mc_nhit_bcplane);
   fChain->SetBranchAddress("mc_x_bcplane[10]", mc_x_bcplane, &b_aBetaEvent_mc_x_bcplane);
   fChain->SetBranchAddress("mc_y_bcplane[10]", mc_y_bcplane, &b_aBetaEvent_mc_y_bcplane);
   fChain->SetBranchAddress("mc_pid_bcplane[10]", mc_pid_bcplane, &b_aBetaEvent_mc_pid_bcplane);
   fChain->SetBranchAddress("mc_theta_bcplane[10]", mc_theta_bcplane, &b_aBetaEvent_mc_theta_bcplane);
   fChain->SetBranchAddress("mc_e_bcplane[10]", mc_e_bcplane, &b_aBetaEvent_mc_e_bcplane);
   fChain->SetBranchAddress("mc_xmom_bcplane[10]", mc_xmom_bcplane, &b_aBetaEvent_mc_xmom_bcplane);
   fChain->SetBranchAddress("mc_ymom_bcplane[10]", mc_ymom_bcplane, &b_aBetaEvent_mc_ymom_bcplane);
   fChain->SetBranchAddress("mc_zmom_bcplane[10]", mc_zmom_bcplane, &b_aBetaEvent_mc_zmom_bcplane);
}

Bool_t DQ1::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

#endif // #ifdef DQ1_cxx
