#include <fstream.h>
#include <TMath.h>
#include <TGraph.h>
#include <TStyle.h>
#include <TCanvas.h>

#ifndef __CINT__
#include "TVirtualFitter.h"
#include "TMath.h"
#include "TStopwatch.h"
#include <stdlib.h>
#include <stdio.h>

//______________________________________________________________________________
void timing(const int size) ;


#endif


void timing(const int size) {


	gROOT->Reset();
//	const int size = 1000;

	Double_t E[size], x[size],y[size],z[size], time[size],dump1,dump2,dump3,dump4;
	Int_t i;
	char xUnit[5],yUnit[5],zUnit[5],EUnit[5];
	ifstream input_file ;

int numGraphs = 4;
int G;

	input_file.open("photons.dat");
	hpx    = new TH1F("hpx","This is the n vs t distribution",100,48,49);
///OPEN FILE and Read data into

	for ( i=0;i<size;i++ ) {
			input_file >>E[i]>>EUnit >> x[i] >>y[i]  >> z[i] >> time[i]  ;
			
			hpx->Fill(time[i]);


			}

	input_file.close();



	TCanvas *c = new TCanvas("c","CANVAS",200,10,400,800) ;

  c->GetFrame()->SetFillColor(21);
  c->GetFrame()->SetBorderSize(6);
  c->GetFrame()->SetBorderMode(-1);

hpx->DrawNormalized();

//	c->Divide(1,2);
	//Create Graphs
//c->cd(1);
	TGraph *gr1 = new TGraph(size,y,x);
	gr1->SetTitle("Positions ");
//	gr1->Draw("A*") ;
//c->cd(2);
//	TGraph *gr2 = new TGraph(size,r,Texact);
//	gr2->SetTitle("Exact");
//	gr2->Draw("AC") ;
/*c->cd(3);
	TGraph *gr3 = new TGraph(size,time, M2);
	gr3->SetTitle("Magnetization: #frac{k_{b} T_{c}}{J} = 2.15");
	gr3->Draw("AC");
c->cd(4);
	TGraph *gr4 = new TGraph(size,time,M3);
	gr4->SetTitle("Magnetization: #frac{k_{b} T_{c}}{J} = 2.25");
	gr4->Draw("AC") ;
*/

	//Fit
/*	TF1  *f1 = new TF1("f1","(x-[0])*(x-[0])/[2]+[1]",-212,212);
	f1->SetParameters(0,-240,1536);
	gr1->Fit(f1);
*/
//	gStyle->SetPalette(1);
//	f1->SetTitle("Parabolic Mirror");
//	f1->Draw("P")

//	gr2->Draw("same P0") ;
//	TGraph *gr2 = new TGraph(size,Vx,Vy);	
}
