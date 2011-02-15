/*!
  \file
 * The class rawBETAEvent is used to hold raw data at the event level. Using this
 * class to hold all the detector data for both real and simulated data allows
 * one to do easy analysis of both.
 */
//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Wed Aug 19 19:21:56 2009 by ROOT version 5.18/00
// from TChain h9502/
//////////////////////////////////////////////////////////

#ifndef rawBETAEvent_h
#define rawBETAEvent_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TObject.h>
#include <TTree.h>
#include <TFile.h>
#include "rawBETAEvent.h"
#include <TBranch.h>
#include "BETAEvent.h"
#include <iostream>

/*!
 *
 * The class rawBETAEvent is used to hold raw data at the event level. Using this
 * class to hold all the detector data for both real and simulated data allows
 * one to do easy analysis of both.
 */
class rawBETAEvent : public TObject {

public :

   rawBETAEvent();
   virtual ~rawBETAEvent();

   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

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

   // BIGCAL DATA
   /*
      NEED TO CHECK OLD SCRIPTS FOR THESE
      Int_t           bc_prot_nhit;
      Int_t           bc_prot_ix[100];   //[bc_prot_nhit]
      Int_t           bc_prot_iy[100];   //[bc_prot_nhit]
      Int_t           bc_prot_adc[100];   //[bc_prot_nhit]
      Int_t           bc_rcs_nhit;
      Int_t           bc_rcs_ix[100];   //[bc_rcs_nhit]
      Int_t           bc_rcs_iy[100];   //[bc_rcs_nhit]
      Int_t           bc_rcs_adc[100];   //[bc_rcs_nhit]
   */

   Int_t           Bigcal_tdc_nhit;
   Int_t           Bigcal_tdc_raw_igroup[1792];   //[bigcal_tdc_nhit]
   Int_t           Bigcal_tdc_raw_irow[1792];   //[bigcal_tdc_nhit]
   Int_t           Bigcal_tdc_raw[1792];   //[bigcal_tdc_nhit]
   
   Int_t           Bigcal_prot_nhit;
   Int_t           Bigcal_prot_ix[1024];   //[Bigcal_prot_nhit]
   Int_t           Bigcal_prot_iy[1024];   //[Bigcal_prot_nhit]
   Int_t           Bigcal_prot_adc_raw[1024];   //[Bigcal_prot_nhit]
   Int_t           Bigcal_rcs_nhit;
   
   Int_t           Bigcal_rcs_ix[720];   //[Bigcal_rcs_nhit]
   Int_t           Bigcal_rcs_iy[720];   //[Bigcal_rcs_nhit]
   Int_t           Bigcal_rcs_adc_raw[720];   //[Bigcal_rcs_nhit]

   Int_t           Bigcal_atrig_nhit;
   Int_t           Bigcal_atrig_igroup[38];   //[Bigcal_atrig_nhit]
   Int_t           Bigcal_atrig_ihalf[38];   //[Bigcal_atrig_nhit]
   Int_t           Bigcal_atrig_adc_raw[38];   //[Bigcal_atrig_nhit]
   Int_t           Bigcal_ttrig_nhit;
   Int_t           Bigcal_ttrig_igroup[336];   //[Bigcal_ttrig_nhit]
   Int_t           Bigcal_ttrig_ihalf[336];   //[Bigcal_ttrig_nhit]
   Int_t           Bigcal_ttrig_adc_raw[336];   //[Bigcal_ttrig_nhit]

   Int_t           y1t_hit;
   Int_t           y1t_row[100];   //[y1t_hit]
   Int_t           y1t_tdc[100];   //[y1t_hit]
   Double_t         y1t_y[100];   //[y1t_hit]
   Int_t           y2t_hit;
   Int_t           y2t_row[100];   //[y2t_hit]
   Int_t           y2t_tdc[100];   //[y2t_hit]
   Double_t         y2t_y[100];   //[y2t_hit]
   Int_t           x1t_hit;
   Int_t           x1t_row[100];   //[x1t_hit]
   Int_t           x1t_tdc[100];   //[x1t_hit]
   Double_t         x1t_x[100];   //[x1t_hit]
   Int_t           cer_hit;
   Int_t           cer_num[50];   //[cer_hit]
   Int_t           cer_tdc[50];   //[cer_hit]
   Int_t           cer_adcc[50];   //[cer_hit]
   Int_t           ceradc_hit;
   Int_t           ceradc_num[15];   //[ceradc_hit]
   Int_t           cer_adc[15];   //[ceradc_hit]
   Int_t           luc_hit;
   Int_t           luc_row[90];   //[luc_hit]
   Int_t           ladc_pos[90];   //[luc_hit]
   Int_t           ladc_neg[90];   //[luc_hit]
   Int_t           ltdc_pos[90];   //[luc_hit]
   Int_t           ltdc_neg[90];   //[luc_hit]
   Double_t         luc_y[90];   //[luc_hit]

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


// Simulated Event Truths

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





   ClassDef(rawBETAEvent,1)
};

#endif



