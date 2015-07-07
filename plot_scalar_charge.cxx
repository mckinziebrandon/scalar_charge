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

Int_t plot_scalar_charge()
{
    using std::cout;
    using std::endl;

    const Int_t nFiles = 164;

    TFile * f       = new TFile("analysis.root");
    TFile * outFile = new TFile("scalar_charge.root", "RECREATE");
    outFile->cd();

    TH1F * jth_ratio_hist_real[nFiles];
    TH1F * jth_ratio_hist_imag[nFiles];

    TH1F * g_scalar         = new TH1F("g_scalar", "g_scalar", 64, -0.5, 63.5);
    TH1F * g_scalar_imag    = new TH1F("g_scalar_imag", "g_scalar imaginary", 64, -0.5, 63.5);

    // obtain jackknife ratio-histograms from input file
    for (int j = 0; j < nFiles; j++)
    {
        jth_ratio_hist_real[j] = (TH1F*)f->Get(Form("%d_ratio_hist_real;1", j));
        jth_ratio_hist_imag[j] = (TH1F*)f->Get(Form("%d_ratio_hist_imag;1", j));
    }

    // performs final average over bin contents
    Float_t real_avg(0), imag_avg(0);
    Float_t real_err(0), imag_err(0);
    for (int t = 0; t < nTimes; t++)
    {
        real_avg = 0;
        imag_avg = 0;

        // ========= calculate average of jackknife points ===========
        for (int j = 0; j < nFiles; j++)
        {
            real_avg += jth_ratio_hist_real[j]->GetBinContent(t+1); 
            imag_avg += jth_ratio_hist_imag[j]->GetBinContent(t+1); 
        }

        real_avg = real_avg / Float_t(nFiles);
        imag_avg = imag_avg / Float_t(nFiles);

        g_scalar->SetBinContent(t+1, real_avg);
        g_scalar_imag->SetBinContent(t+1, imag_avg);
        // ===========================================================

        // ========================== calculate jackknife errors =============================
        for (int j = 0; j < nFiles; j++)
        {
            real_err += TMath::Power(jth_ratio_hist_real[j]->GetBinContent(t+1) - real_avg, 2);
            imag_err += TMath::Power(jth_ratio_hist_imag[j]->GetBinContent(t+1) - imag_avg, 2);
        }

        real_err = TMath::Sqrt((Float_t)((nFiles-1)/nFiles) * real_err);
        imag_err = TMath::Sqrt((Float_t)((nFiles-1)/nFiles) * imag_err);

        g_scalar->SetBinError(t+1, real_err);
        g_scalar_imag->SetBinError(t+1, imag_err);
        // ===================================================================================
    }

    // ========================== Drawing ==========================
    TCanvas * MyC   = new TCanvas("MyC", "Canvas", 1100, 700);
    TPad * pad1     = new TPad("pad1", "Pad", 0.025, 0.025, 0.975, 0.975);
    pad1->Divide(2, 1);
    gPad->SetTickx();
    gPad->SetTicky();
    pad1->Draw();

    TLegend * legend = new TLegend(0.15, 0.85, 0.40, 0.92);
    TLegend * legend2 = new TLegend(0.15, 0.85, 0.40, 0.92);
    legend->AddEntry(g_scalar,"Re(g_{s})","PE");
    legend2->AddEntry(g_scalar_imag,"Im(g_{s})","PE");

    MyC->cd();
    pad1->cd(1);
    g_scalar->Draw("PE2");
    legend->Draw();

    pad1->cd(2);
    g_scalar_imag->Draw("PPE2");
    legend2->Draw();
    MyC->Update();

    gROOT->ForceStyle();
    return 0;
}
