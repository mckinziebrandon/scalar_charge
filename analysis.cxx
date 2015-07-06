/*==================================================================
Filename:       jackknife_analysis.cxx
Author:         Brandon McKinzie
Date Created:   July 02, 2015
Description:    jk analysis...
==================================================================*/
#include <iostream>
#include <fstream>
#include <string>
#include <limits>
#include <cstdlib>

#include "my_functions.h"

void analysis()
{
    using std::cout;
    using std::endl;

    const Int_t nFiles = 164;

    TFile * f = new TFile("./file_processing/jackknife.root");
    TFile * outFile = new TFile("analysis.root", "RECREATE");
    outFile->cd();

    TH1F * jth_C2_hist_real[nFiles];    
    TH1F * jth_C2_hist_imag[nFiles];    
    TH1F * jth_C3_u_hist_real[nFiles];    
    TH1F * jth_C3_u_hist_imag[nFiles];    
    TH1F * jth_C3_d_hist_real[nFiles];    
    TH1F * jth_C3_d_hist_imag[nFiles];    

    TH1F * jth_ratio_hist_real[nFiles];
    TH1F * jth_ratio_hist_imag[nFiles];

    TH1F * C2_hist = new TH1F("C2_hist", "C2_hist", 64, -0.5, 63.5);
    TH1F * C3_hist = new TH1F("C3_hist", "C3_hist", 64, -0.5, 63.5);

    for (int j = 0; j < nFiles; j++)
    {
        jth_C2_hist_real[j] = (TH1F*)f->Get(Form("C2_real_hist_j_%d;1", j));
        jth_C2_hist_imag[j] = (TH1F*)f->Get(Form("C2_imag_hist_j_%d;1", j));
        jth_C3_u_hist_real[j] = (TH1F*)f->Get(Form("C3_u_real_hist_j_%d;1", j));
        jth_C3_u_hist_imag[j] = (TH1F*)f->Get(Form("C3_u_imag_hist_j_%d;1", j));
        jth_C3_d_hist_real[j] = (TH1F*)f->Get(Form("C3_d_real_hist_j_%d;1", j));
        jth_C3_d_hist_imag[j] = (TH1F*)f->Get(Form("C3_d_imag_hist_j_%d;1", j));

        jth_ratio_hist_real[j] = new TH1F(Form("%d_ratio_hist_real", j) , "hist", 64, -0.5, 63.5);
        jth_ratio_hist_imag[j] = new TH1F(Form("%d_ratio_hist_imag", j) , "hist", 64, -0.5, 63.5);
    }


    Float_t C3_u_real, C3_u_imag;
    Float_t C3_d_real, C3_d_imag;
    Float_t C2_real, C2_imag;

    Float_t numer_real, numer_imag;
    Float_t denom_real, denom_imag;

    Float_t ratio_real, ratio_imag;

    // generates set of jackknife scalar charges (ratios) hists
    for (int t = 0; t < nTimes; t++)
    {
        for (int j = 0; j < nFiles; j++)
        {
            C2_real = jth_C2_hist_real[j]->GetBinContent(t+1);      // c
            C2_imag = jth_C2_hist_imag[j]->GetBinContent(t+1);      // d

            C3_u_real = jth_C3_u_hist_real[j]->GetBinContent(t+1);  // a1
            C3_u_imag = jth_C3_u_hist_imag[j]->GetBinContent(t+1);  // b1
            C3_d_real = jth_C3_d_hist_real[j]->GetBinContent(t+1);  // a2
            C3_d_imag = jth_C3_d_hist_imag[j]->GetBinContent(t+1);  // b2

            numer_real = C3_u_real - C3_d_real;
            numer_imag = C3_u_imag - C3_d_imag;
            denom_real = C2_real;
            denom_imag = C2_imag;

            ratio_real = (numer_real * denom_real) + (numer_imag * denom_imag);
            ratio_real = 1 / (denom_real*denom_real + denom_imag*denom_imag);

            ratio_imag = (numer_imag * denom_real) - (numer_real * denom_imag);
            ratio_imag = 1 / (denom_real*denom_real + denom_imag*denom_imag);

            jth_ratio_hist_real[j]->SetBinContent(t+1, ratio_real);
            jth_ratio_hist_imag[j]->SetBinContent(t+1, ratio_imag);
            
            if (t == nTimes-1)
            {
                jth_ratio_hist_real[j]->Write();
                jth_ratio_hist_imag[j]->Write();
            }
        }
    }
}
