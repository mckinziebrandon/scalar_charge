/*==================================================================
Filename:       jackknife_analysis.cxx
Author:         Brandon McKinzie
Date Created:   July 02, 2015
Description:    input is basic jackknife samples on correlation functions
                output is jk sample of scalar charge ratio formula
==================================================================*/
#include <iostream>
#include <fstream>
#include <string>
#include <limits>
#include <cstdlib>

#include "my_functions.h"

void get_jackknife_ratios()
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
        jth_C2_hist_real[j]     = (TH1F*)f->Get(Form("C2_real_hist_j_%d;1", j));
        jth_C2_hist_imag[j]     = (TH1F*)f->Get(Form("C2_imag_hist_j_%d;1", j));

        jth_C3_u_hist_real[j]   = (TH1F*)f->Get(Form("C3_u_real_hist_j_%d;1", j));
        jth_C3_u_hist_imag[j]   = (TH1F*)f->Get(Form("C3_u_imag_hist_j_%d;1", j));
        jth_C3_d_hist_real[j]   = (TH1F*)f->Get(Form("C3_d_real_hist_j_%d;1", j));
        jth_C3_d_hist_imag[j]   = (TH1F*)f->Get(Form("C3_d_imag_hist_j_%d;1", j));

        jth_ratio_hist_real[j]  = new TH1F(Form("%d_ratio_hist_real", j) , "hist", 64, -0.5, 63.5);
        jth_ratio_hist_imag[j]  = new TH1F(Form("%d_ratio_hist_imag", j) , "hist", 64, -0.5, 63.5);
    }

    // generates set of jackknife scalar charges (ratios) hists
    Complex C2, C3_up_quark, C3_down_quark, scalar_charge;
    for (int t = 0; t < nTimes; t++)
    {
        for (int j = 0; j < nFiles; j++)
        {
            C2.SetReal(jth_C2_hist_real[j]->GetBinContent(10));    
            C2.SetImag(jth_C2_hist_imag[j]->GetBinContent(10));    

            C3_up_quark.SetReal(jth_C3_u_hist_real[j]->GetBinContent(t+1));
            C3_up_quark.SetImag(jth_C3_u_hist_imag[j]->GetBinContent(t+1));
            
            C3_down_quark.SetReal(jth_C3_d_hist_real[j]->GetBinContent(t+1));
            C3_down_quark.SetImag(jth_C3_d_hist_imag[j]->GetBinContent(t+1));

            scalar_charge = (C3_up_quark - C3_down_quark) / C2;

            jth_ratio_hist_real[j]->SetBinContent(t+1, scalar_charge.GetReal());
            jth_ratio_hist_imag[j]->SetBinContent(t+1, scalar_charge.GetImag());
            
            if (t == nTimes-1)
            {
                jth_ratio_hist_real[j]->Write();
                jth_ratio_hist_imag[j]->Write();
            }
        }
    }
}
