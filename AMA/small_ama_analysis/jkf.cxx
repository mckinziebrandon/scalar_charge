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

Int_t jkf()
{
    const Int_t nFiles = 39;
    using std::cout;
    using std::endl;
    
    Float_t fit_val(0), fit_err(0);
    Float_t fit_errr(0);
    Float_t fits[nFiles];

    TFile * f       = new TFile("../rFiles/ratios.root");
    TFile * outFile = new TFile("../rFiles/jk_fit.root", "RECREATE");
    outFile->cd();

    TH1F * jth_ratio_hist_real[nFiles];
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
    // and fit/calculate errors
    for (int j = 0; j < nFiles; j++)
    {
        jth_ratio_hist_real[j] = (TH1F*)f->Get(Form("%d_ratio_real;1", j));
        jth_ratio_hist_real[j]->Draw("PE1");
        jth_ratio_hist_real[j]->Fit("f_const", "MR0");

        fit_val += f_const->GetParameter("const");
        fits[j] = f_const->GetParameter("const");
        fit_err += f_const->GetChisquare();
    }

    fit_val = fit_val / (Float_t)nFiles;
    fit_err = fit_err / (Float_t)nFiles;

    for(int j = 0; j < nFiles; j++)
    {
        fit_errr +=  TMath::Power(fits[j] - fit_val, 2);
    }

    fit_errr = TMath::Sqrt(fit_errr * Float_t(nFiles-1)/nFiles);

    fit_val /= 3.2;
    fit_err /= 3.2;
    fit_errr /= 3.2;

    ofstream out;
    out.open("fit.txt");
    out << fit_val << "\n";
    out << fit_errr;
    out.close();

    // performs final average over bin contents
    Float_t average, error;
    for (int t = 0; t < 8; t++)
    {
        average = AverageOverFiles(jth_ratio_hist_real, t+1, nFiles);
        error   = ErrorOverFiles(jth_ratio_hist_real, t+1, nFiles, average);
        average /= 3.2;
        error /= 3.2;
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
    leg->AddEntry((TObject*)0, Form("Jackknife Fit: %.3f #pm %.3f", fit_val, fit_errr), "");
    leg->AddEntry((TObject*)0, Form("#Chi^{2}: %.3f", fit_err), "");
    leg->Draw();

    return 0;
}
