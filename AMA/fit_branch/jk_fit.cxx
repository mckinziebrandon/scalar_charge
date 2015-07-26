/*==================================================================
Filename:       jk_fit.cxx
Author:         Brandon McKinzie
Date Created:   July 13, 2015
Description:    fit scalar charge plot using jk methods
==================================================================*/
#include <iostream>
#include <fstream>
#include <string>
#include <limits>
#include <cstdlib>

#include "../include/my_functions.h"

Int_t jk_fit()
{
    using std::cout;
    using std::endl;

    Float_t norm = -(1.5 * (4 - 1.8) + 1); 
    Float_t norm2 = 3.2;
    Float_t fit_val(0), fit_err(0);
    Float_t fits[nJack];

    TFile * f       = new TFile("../rootFiles/ratios.root");
    TFile * f2      = new TFile("../rootFiles/ratios2.root");
    TFile * outFile = new TFile("../rootFiles/jk_fit.root", "RECREATE");
    outFile->cd();

    TH1F * jth_ratio_ama[nJack];
    TH1F * jth_ratio_exact[nFiles];

    TH1F * g_scalar  = new TH1F("g_scalar", ";t;g_{s}(t)", 8, -0.5, 7.5);
    g_scalar->SetMarkerStyle(22);
    g_scalar->GetXaxis()->CenterTitle();
    g_scalar->GetYaxis()->CenterTitle();

    TF1  * f_const   = new TF1("f_const", Const, 3, 5, 1);
    f_const->SetLineStyle(7);
    f_const->SetLineWidth(4);
    f_const->SetLineColorAlpha(kRed+1, 0.5);
    f_const->SetParNames("const");
    f_const->SetParameter("const", 5.);

    // obtain jackknife ratio-histograms from input file
    for (int j = 0; j < nJack; j++)
    {
        // --- Obtain & fit ama jk histograms ---
        jth_ratio_ama[j] = (TH1F*)f->Get(Form("%d_ratio_real;1", j));
        jth_ratio_ama[j]->Draw("PE1");
        jth_ratio_ama[j]->Fit("f_const", "MR0");
        fits[j] = f_const->GetParameter("const");
        fit_val += fits[j];
    }
    fit_val /= (Float_t)nJack;

    // calculate average and jk error
    for(int j = 0; j < nJack; j++)
    {
        fit_err +=  TMath::Power(fits[j] - fit_val, 2);
    }
    fit_err = TMath::Sqrt(fit_err * Float_t(nJack-1)/nJack);

    // normalization
    fit_val /= norm;
    fit_err /= norm;

    ofstream out;
    out.open("fit.txt");
    out << fit_val << "\n";
    out << fit_err;
    out.close();

    // performs final average over bin contents
    Float_t average, error;
    for (int t = 0; t < 8; t++)
    {
        average = AverageOverFiles(jth_ratio_ama, t+1, nJack);
        error   = ErrorOverFiles(jth_ratio_ama, t+1, nJack, average);
        average /= norm;
        error /= norm;
        g_scalar->SetBinContent(t+1, average);
        g_scalar->SetBinError(t+1, error);
    }

    // ========================== Drawing ==========================
    TCanvas * MyC = new TCanvas("MyC", "Canvas", 1000, 600);
    MyC->cd();
    g_scalar->Draw("E1");

    TLine * line1 = new TLine(3., fit_val, 5., fit_val);
    line1->SetLineColor(12);
    line1->SetLineWidth(3);
    line1->SetLineStyle(2);
    line1->Draw();

    TLegend * leg = new TLegend(0.15, 0.91, 0.62, 0.98);
    leg->AddEntry(g_scalar, "Re(g_{s})", "PE");
    leg->AddEntry((TObject*)0, Form("Jackknife Fit: %.3f #pm %.3f", fit_val, fit_err), "");
    leg->Draw();

    return 0;
}
