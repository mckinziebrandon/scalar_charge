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

#include "include/my_functions.h"
#include "TGaxis.h"

Int_t plot_scalar_charge()
{
    using std::cout;
    using std::endl;

    const Int_t nFiles = 164;

    TFile * f       = new TFile("rootFiles/analysis.root");
    TFile * outFile = new TFile("rootFiles/scalar_charge.root", "RECREATE");
    outFile->cd();

    TH1F * jth_ratio_hist_real[nFiles];
    TH1F * jth_ratio_hist_imag[nFiles];

    TH1F * g_scalar         = new TH1F("g_scalar", "g_scalar", 8, -0.5, 7.5);
    TH1F * g_scalar_imag    = new TH1F("g_scalar_imag", "g_scalar imaginary", 8, -0.5, 7.5);

    // obtain jackknife ratio-histograms from input file
    for (int j = 0; j < nFiles; j++)
    {
        jth_ratio_hist_real[j] = (TH1F*)f->Get(Form("%d_ratio_real;1", j));
        jth_ratio_hist_imag[j] = (TH1F*)f->Get(Form("%d_ratio_imag;1", j));
    }

    // performs final average over bin contents
    Complex average, error;
    for (int t = 0; t < 8; t++)
    {
        average.SetReal(AverageOverFiles(jth_ratio_hist_real, t+1, nFiles));
        average.SetImag(AverageOverFiles(jth_ratio_hist_imag, t+1, nFiles));

        error.SetReal(ErrorOverFiles(jth_ratio_hist_real, t+1, nFiles, average.GetReal()));
        error.SetImag(ErrorOverFiles(jth_ratio_hist_imag, t+1, nFiles, average.GetImag()));

        g_scalar->SetBinContent(t+1, average.GetReal());
        g_scalar_imag->SetBinContent(t+1, average.GetImag());

        g_scalar->SetBinError(t+1, error.GetReal());
        g_scalar_imag->SetBinError(t+1, error.GetImag());
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
    g_scalar->Draw("PE1");
    legend->Draw();

    pad1->cd(2);
    g_scalar_imag->Draw("PE1");
    legend2->Draw();
    MyC->Update();

    g_scalar->Write();
    gROOT->ForceStyle();
    return 0;
}
