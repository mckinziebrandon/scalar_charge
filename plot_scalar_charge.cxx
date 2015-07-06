/*==================================================================
Filename:       .cxx
Author:         Brandon McKinzie
Date Created:   July 06, 2015
Description:    jk analysis...
==================================================================*/
#include <iostream>
#include <fstream>
#include <string>
#include <limits>
#include <cstdlib>

#include "my_functions.h"

void plot_scalar_charge()
{
    using std::cout;
    using std::endl;

    const Int_t nFiles = 164;

    TFile * f = new TFile("analysis.root");
    TFile * outFile = new TFile("scalar_charge.root", "RECREATE");
    outFile->cd();

    TH1F * jth_ratio_hist_real[nFiles];
    TH1F * jth_ratio_hist_imag[nFiles];

    TH1F * g_scalar = new TH1F("g_scalar", "g_scalar", 64, -0.5, 63.5);
    TH1F * g_scalar_imag = new TH1F("g_scalar_imag", "g_scalar imaginary", 64, -0.5, 63.5);

    for (int j = 0; j < nFiles; j++)
    {
        jth_ratio_hist_real[j] = (TH1F*)f->Get(Form("%d_ratio_hist_real;1", j));
        jth_ratio_hist_imag[j] = (TH1F*)f->Get(Form("%d_ratio_hist_imag;1", j));
    }

    Float_t r_magnitude(0), i_magnitude(0);
    for (int t = 0; t < nTimes; t++)
    {
        r_magnitude = 0;
        i_magnitude = 0;
        for (int j = 0; j < nFiles; j++)
        {
            r_magnitude += jth_ratio_hist_real[j]->GetBinContent(t+1); 
            i_magnitude += jth_ratio_hist_imag[j]->GetBinContent(t+1); 
        }

        r_magnitude = r_magnitude / Float_t(nFiles);
        i_magnitude = i_magnitude / Float_t(nFiles);

        g_scalar->SetBinContent(t+1, r_magnitude);
        g_scalar_imag->SetBinContent(t+1, i_magnitude);
    }
    g_scalar->Draw("LP");
}
