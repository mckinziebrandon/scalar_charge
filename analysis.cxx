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

    TH1F * jth_ratio_hist[nFiles];

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
    }

    Float_t bin_content_real(0);
    Float_t bin_content_imag(0);
    Float_t bin_magnitude(0);
    Float_t r_norm_factor(0);
    Float_t i_norm_factor(0);

    // C2
    for (int t = 0; t < nTimes; t++)
    {
        bin_content_real = 0;
        bin_content_imag = 0;
        bin_magnitude = 0;

        for (int j = 0; j < nFiles; j++)
        {
            bin_content_real += jth_C2_hist_real[j]->GetBinContent(t+1);
            bin_content_imag += jth_C2_hist_imag[j]->GetBinContent(t+1);
        }
    
        bin_content_real = bin_content_real / Float_t(nFiles);
        bin_content_imag = bin_content_imag / Float_t(nFiles);
        bin_magnitude = TMath::Sqrt(bin_content_real*bin_content_real + bin_content_imag*bin_content_imag);

        if (t == 9)
        {
            r_norm_factor = bin_content_real;
            i_norm_factor = bin_content_imag;
        }

        C2_hist->SetBinContent(t+1, bin_magnitude);
    }

    // C3
    for (int t = 0; t < nTimes; t++)
    {
        bin_content_real = 0;
        bin_content_imag = 0;
        bin_magnitude = 0;

        for (int j = 0; j < nFiles; j++)
        {
            bin_content_real += jth_C3_u_hist_real[j]->GetBinContent(t+1);
            bin_content_real -= jth_C3_d_hist_real[j]->GetBinContent(t+1);

            bin_content_imag += jth_C3_u_hist_imag[j]->GetBinContent(t+1);
            bin_content_imag -= jth_C3_d_hist_imag[j]->GetBinContent(t+1);
        }
    
        bin_content_real = bin_content_real / Float_t(nFiles);
        bin_content_imag = bin_content_imag / Float_t(nFiles);

        // complex division
        bin_magnitude = TMath::Sqrt(TMath::Power((bin_content_real * r_norm_factor) + (bin_content_imag * i_norm_factor), 2) 
                                  + TMath::Power((bin_content_imag * r_norm_factor) - (bin_content_real * i_norm_factor), 2));
        bin_magnitude *= 1 / ((TMath::Power(r_norm_factor, 2)) + (TMath::Power(i_norm_factor, 2)));

        C3_hist->SetBinContent(t+1, bin_magnitude);
    }

    // get ratio histogram
    TH1F * gscalar_hist = (TH1F*)C3_hist->Clone("gscalar_hist");
    gscalar_hist->SetTitle("Isovector Scalar Charge g_{s}; timeslice; g_{s}(t)");
    gscalar_hist->GetXaxis()->CenterTitle();
    gscalar_hist->GetYaxis()->CenterTitle();
//  gscalar_hist->Scale(1/C2_hist->GetBinContent(10));

    TCanvas * MyC = new TCanvas();
    MyC->cd();

    TLegend * legend = new TLegend(0.60, 0.70, 0.88, 0.88);
    legend->AddEntry(gscalar_hist, "<N(#vec{P})|#bar{u}u-#bar{d}d|N(#vec{P})>  ", "P");

    gscalar_hist->Draw("CP");
    legend->Draw();


    outFile->Write();
}
