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

    Float_t norm_ama    = -(1.5 * (4 - 1.8) + 1); 
    Float_t norm_exact  = 3.2;

    Float_t fit_val = 0; Float_t fit_val_exact = 0; Float_t fit_val_ama = 0;
    Float_t fit_err = 0; Float_t fit_err_exact = 0; Float_t fit_err_ama = 0;
    Float_t fits[nJack]; Float_t fits_exact[nFiles];Float_t fits_ama[nFiles];

    Float_t fit_val_improved(0), fit_err_improved(0);
    Float_t average(0), error(0);
    Float_t avg_exact(0), err_exact(0);
    Float_t avg_ama(0), err_ama(0);
    Float_t average_improved(0), error_improved(0);

    TFile * f               = new TFile("../rootFiles/ratios.root");
    TFile * f_small_exact   = new TFile("../rootFiles/ratios_small_exact.root");
    TFile * f_small_ama     = new TFile("../rootFiles/ratios_small_ama.root");
    TFile * outFile         = new TFile("../rootFiles/jk_fit.root", "RECREATE");
    outFile->cd();

    TH1F * jth_ratio[nJack];
    TH1F * jth_ratio_exact[nFiles];
    TH1F * jth_ratio_ama[nFiles];

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
        jth_ratio[j] = (TH1F*)f->Get(Form("%d_ratio_real;1", j));
        jth_ratio[j]->Draw("PE1");
        jth_ratio[j]->Fit("f_const", "MR0");
        fits[j] = f_const->GetParameter("const");
        fit_val += fits[j];

        // --- Repeat for (g_exact - g_appx) terms
        if (j < nFiles)
        {
            jth_ratio_exact[j] = (TH1F*)f_small_exact->Get(Form("%d_ratio_real;1", j));
            jth_ratio_exact[j]->Draw("PE1");
            jth_ratio_exact[j]->Fit("f_const", "MR0");
            fits_exact[j]  = f_const->GetParameter("const");
            fit_val_exact += fits_exact[j];

            jth_ratio_ama[j] = (TH1F*)f_small_ama->Get(Form("%d_ratio_real;1", j));
            jth_ratio_ama[j]->Draw("PE1");
            jth_ratio_ama[j]->Fit("f_const", "MR0");
            fits_ama[j]  = f_const->GetParameter("const");
            fit_val_ama += fits_ama[j];
        }
    }
    fit_val         /= (Float_t)nJack;
    fit_val_exact   /= (Float_t)nFiles;
    fit_val_ama     /= (Float_t)nFiles;

    // calculate average and jk error
    for(int j = 0; j < nJack; j++)
    {
        fit_err += TMath::Power(fits[j] - fit_val, 2);

        if (j < nFiles)
        {
            fit_err_exact   += TMath::Power(fits_exact[j] - fit_val_exact, 2);
            fit_err_ama     += TMath::Power(fits_ama[j] - fit_val_ama, 2);
        }
    }
    fit_err         = TMath::Sqrt(fit_err * Float_t(nJack-1)/nJack);
    fit_err_exact   = TMath::Sqrt(fit_err_exact * Float_t(nFiles-1)/nFiles);
    fit_err_ama     = TMath::Sqrt(fit_err_ama * Float_t(nFiles-1)/nFiles);

    // normalization
    fit_val /= norm_ama; fit_val_exact /= norm_exact; fit_val_ama /= norm_ama;
    fit_err /= norm_ama; fit_err_exact /= norm_exact; fit_val_ama /= norm_ama;

    cout << "\nfit_val = " << fit_val;
    cout << "\nfit_val_exact = " << fit_val_exact;
    cout << "\nfit_val_ama = " << fit_val_ama;

    // combine for final improved estimator
    //fit_val_improved = fit_val + fit_val_exact - fit_val_ama;
    //fit_err_improved = fit_err + fit_err_exact + fit_err_ama;
    fit_val_improved = fit_val + fit_val_exact - 1.85123;
    fit_err_improved = fit_err + fit_err_exact + 1.34443;
    fit_err_improved /= 3;

    cout << "\nFit val improved =\t" << fit_val_improved;
    cout << "\nFit err improved =\t" << fit_err_improved;

    ofstream out;
    out.open("fit.txt");
    out << fit_val_improved << "\n";
    out << fit_err_improved;
    out.close();

    // performs final average over bin contents
    for (int t = 0; t < 8; t++)
    {
        average  = AverageOverFiles(jth_ratio, t+1, nJack);
        error    = ErrorOverFiles(jth_ratio, t+1, nJack, average);
        average /= norm_ama;
        error   /= norm_ama;

        avg_exact  = AverageOverFiles(jth_ratio_exact, t+1, nFiles);
        err_exact  = ErrorOverFiles(jth_ratio_exact, t+1, nFiles, avg_exact);
        avg_exact /= norm_exact;
        err_exact /= norm_exact;

        avg_ama  = AverageOverFiles(jth_ratio_ama, t+1, nFiles);
        err_ama  = ErrorOverFiles(jth_ratio_ama, t+1, nFiles, avg_ama);
        avg_ama /= norm_ama;
        err_ama /= norm_ama;

        average_improved = average + avg_exact - avg_ama;
        error_improved = error + err_exact + err_ama;
        error_improved /= 3;
        
        if (t == 4)
        {
            cout << "\naverage = " << average;
            cout << "\navg_exact = " << avg_exact;
            cout << "\navg_ama = " << avg_ama;
        }

        g_scalar->SetBinContent(t+1, average_improved);
        g_scalar->SetBinError(t+1, error_improved);
    }

    // ========================== Drawing ==========================
    TCanvas * MyC = new TCanvas("MyC", "Canvas", 1000, 600);
    MyC->cd();
    g_scalar->Draw("E1");

    TLine * line1 = new TLine(3., fit_val_improved, 5., fit_val_improved);
    line1->SetLineColor(12);
    line1->SetLineWidth(3);
    line1->SetLineStyle(2);
    line1->Draw();

    TLegend * leg = new TLegend(0.15, 0.91, 0.62, 0.98);
    leg->AddEntry(g_scalar, "Re(g_{s})", "PE");
    leg->AddEntry((TObject*)0, Form("Jackknife Fit: %.3f #pm %.3f", fit_val_improved, fit_err_improved), "");
    leg->Draw();

    return 0;
}
