/*!
  \file
 * The class BETAEvent is used to hold raw data at the event level. Using this
 * class to hold all the detector data for both real and simulated data allows
 * one to do easy analysis of both.
 */


#ifndef BETAEvent_h
#define BETAEvent_h
#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TObject.h>
#include <TTree.h>
#include <TFile.h>
#include <TBranch.h>


/*!
 *
 * The class BETAEvent is used to hold raw data at the event level. Using this
 * class to hold all the detector data for both real and simulated data allows
 * one to do easy analysis of both.
 */
class BETAEvent : public TObject {

public :
   BETAEvent();
   virtual ~BETAEvent();


Int_t fEventNumber;
Int_t fTriggerType;


private : 

 // SANE
 // Not sure if these data members are needed
   Float_t   half_plate;
   Int_t   bgid;
   Int_t   bgtype;
   Int_t   btrigtype;
   Int_t   ngooda;
   Int_t   ngoodt;
   Int_t   ngoodta;
   Int_t   ngoodtt;
   Int_t irowmax, icolmax;
   Float_t  max_adc;

   Long64_t eventNumber;

   // Event Classification flags
   Int_t           analysisLevel;
   Int_t           triggerType;

   // Scaler Info
   Double_t        tcharge;
   Double_t        charge2s;
   Double_t        tcharge_help;
   Double_t        charge2s_help;
   Double_t        tcharge_helm;
   Double_t        charge2s_helm;
   Double_t        polarea;
   Double_t        polarization;
   Int_t           hel_p_scaler;
   Int_t           hel_n_scaler;
   Int_t           hel_p_trig;
   Int_t           hel_n_trig;
   Double_t        dtime_p;
   Double_t        dtime_n;
   
   // Triggers
   Int_t           gen_event_id_number;
   Int_t           gen_event_type;
   Int_t           gen_event_class;
   Int_t           gen_event_roc_summary;
   Int_t           gen_event_sequence_n;
   Int_t           gen_event_trigtype[12];

   Double_t         T_trghms;
   Double_t         T_trgbig;
   Double_t         T_trgpi0;
   Double_t         T_trgbeta;
   Double_t         T_trgcoin1;
   Double_t         T_trgcoin2;

   // HMS DATA
   Double_t         hms_p;
   Double_t         hms_e;
   Double_t         hms_theta;
   Double_t         hms_phi;
   Double_t         hsxfp_s;
   Double_t         hsyfp_s;
   Double_t         hsxpfp_s;
   Double_t         hsypfp_s;
   Double_t         hms_xtar;
   Double_t         hms_ytar;
   Double_t         hms_yptar;
   Double_t         hms_xptar;
   Double_t         hms_delta;
   Double_t         hms_start;
   Double_t         hsshtrk_s;
   Double_t         hsshsum_s;
   Double_t         hsbeta_s;

   // BEAM DATA
   Double_t         rast_x;
   Double_t         rast_y;
   Double_t         slow_rast_x;
   Double_t         slow_rast_y;
   Double_t         sem_x;
   Double_t         sem_y;
   Int_t            i_helicity;
   Double_t         hms_cer_npe1;
   Double_t         hms_cer_npe2;
   Double_t         hms_cer_adc1;
   Double_t         hms_cer_adc2;

public : 

// Simulated Event Truths
/*
   // from PrimaryGeneratorAction
   Int_t           mc_nhit_init;
   Double_t         mc_xpos_init[4];
   Double_t         mc_ypos_init[4];
   Double_t         mc_zpos_init[4];
   Double_t         mc_theta_init[4];
   Double_t         mc_phi_init[4];
   Double_t         mc_e_init[4];
   Double_t         mc_pid_init[4];
   // from DetectorConstruction
   Int_t           mc_nhit_bcplane;
   Double_t         mc_x_bcplane[10];
   Double_t         mc_y_bcplane[10];
   Double_t         mc_pid_bcplane[10];
   Double_t         mc_phi_bcplane[10];
   Double_t         mc_theta_bcplane[10];
   Double_t         mc_e_bcplane[10];
   Double_t         mc_xmom_bcplane[10];
   Double_t         mc_ymom_bcplane[10];
   Double_t         mc_zmom_bcplane[10];

*/


   ClassDef(BETAEvent,5)
};

#endif



