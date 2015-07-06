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

#include "../my_functions.h"

void analysis()
{
    using std::cout;
    using std::endl;

    TFile * f = new TFile("jackknife.root");
    TFile * outFile = new TFile("analysis.root", "RECREATE");
    outFile->cd();

    TH1F * jth_C2_hist_real[nFiles];    
    TH1F * jth_C2_hist_imag[nFiles];    
    TH1F * jth_C3_hist_real[nFiles];    
    TH1F * jth_C3_hist_imag[nFiles];    

    for (int j = 0; j < nFiles; j++)
    {
        jth_C2_hist_real[j] = (TH1F*)f->Get(Form("C2_real_hist_j_%d;1", j));
        jth_C2_hist_imag[j] = (TH1F*)f->Get(Form("C2_imag_hist_j_%d;1", j));
        jth_C3_hist_real[j] = (TH1F*)f->Get(Form("C3_real_hist_j_%d;1", j));
        jth_C3_hist_imag[j] = (TH1F*)f->Get(Form("C3_imag_hist_j_%d;1", j));
    }

    // get <c3(t)>
    Float_t bin_content_real(0);
    Float_t bin_content_imag(0);
    Float_t bin_magnitude(0);
    TH1F * C3_hist = new TH1F("C3_hist", "C3_hist", 64, -0.5, 63.5);
    for (int t = 0; t < nTimes-10; t++)
    {
        bin_content_real = 0;
        bin_content_imag = 0;
        bin_magnitude = 0;

        for (int j = 0; j < 160; j++)
        {
            bin_content_real += jth_C3_hist_real[j]->GetBinContent(t+1);
            bin_content_imag += jth_C3_hist_imag[j]->GetBinContent(t+1);

        }
    
        cout << bin_content_real << "; " << bin_content_imag << endl;
//        bin_content_real = bin_content_real ;
 //       bin_content_imag = bin_content_imag / Float_t(10);
        bin_magnitude = bin_content_real;

        C3_hist->SetBinContent(t+1, bin_magnitude);

    }

    C3_hist->Draw();

    outFile->Write();
}
