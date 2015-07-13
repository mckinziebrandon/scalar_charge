/*==================================================================
Filename:       fit.cxx
Author:         Brandon McKinzie
Date Created:   July 13, 2015
Description:    fit scalar charge plot at plateau
==================================================================*/
#include <iostream>
#include <fstream>
#include <string>
#include <limits>
#include <cstdlib>

#include "../include/my_functions.h"

Int_t straight_fit()
{
    using std::cout;
    using std::endl;
    
    Float_t fit_val, fit_err;

    TCanvas * MyC = new TCanvas("MyC", "Canvas", 1000, 600);
    TPad * pad1     = new TPad("pad1", "Pad", 0.025, 0.025, 0.975, 0.975);
    pad1->Divide(3, 1);
    pad1->Draw();

    TFile * f       = new TFile("../rootFiles/scalar_charge.root");
    TFile * outFile = new TFile("../rootFiles/fit.root", "RECREATE");
    outFile->cd();

    TF1 * f_const   = new TF1("f_const", Const, 0, 6, 1);
    f_const->SetLineStyle(7);
    f_const->SetLineWidth(4);
    f_const->SetLineColorAlpha(kRed+1, 0.5);
    f_const->SetParNames("const");
    f_const->SetParameter("const", 5.);

    TH1F * g_scalar = (TH1F*)f->Get("g_scalar;1");
    g_scalar->SetTitle(";t;g_{s}(t)");
    g_scalar->SetMarkerStyle(22);
    g_scalar->GetXaxis()->CenterTitle();
    g_scalar->GetYaxis()->CenterTitle();
    g_scalar->GetYaxis()->SetTitleOffset(0.5);

    // ---------- Draw & fit for 3 different fit ranges ----------
    MyC->cd();

    pad1->cd(1);
    g_scalar->Draw("E1");
    g_scalar->Fit("f_const", "M0", "", 3, 4);
    fit_val = f_const->GetParameter("const");
    fit_err = f_const->GetParError(0);

    TLine * line1 = new TLine(3., fit_val, 4., fit_val);
    line1->SetLineColor(12);
    line1->SetLineWidth(3);
    line1->SetLineStyle(2);
    line1->Draw();

    TLegend * leg = new TLegend(0.15, 0.91, 0.56, 0.98);
    leg->AddEntry(g_scalar, "Re(g_{s})", "PE");
    leg->AddEntry((TObject*)0, Form("Fit: %.3f #pm %.3f", fit_val, fit_err), "");
    leg->Draw();

    pad1->cd(2);
    g_scalar->Draw("E1");
    g_scalar->Fit("f_const", "M0", "", 3, 5);
    fit_val = f_const->GetParameter("const");
    fit_err = f_const->GetParError(0);

    TLine * line2 = new TLine(3., fit_val, 5., fit_val);
    line2->SetLineColor(12);
    line2->SetLineWidth(3);
    line2->SetLineStyle(2);
    line2->Draw();

    TLegend * leg2 = new TLegend(0.15, 0.91, 0.56, 0.98);
    leg2->AddEntry(g_scalar, "Re(g_{s})", "PE");
    leg2->AddEntry((TObject*)0, Form("Fit: %.3f #pm %.3f", fit_val, fit_err), "");
    leg2->Draw();

    pad1->cd(3);
    g_scalar->Draw("E1");
    g_scalar->Fit("f_const", "M0", "", 4, 5);
    fit_val = f_const->GetParameter("const");
    fit_err = f_const->GetParError(0);

    TLine * line3 = new TLine(4., fit_val, 5., fit_val);
    line3->SetLineColor(12);
    line3->SetLineWidth(3);
    line3->SetLineStyle(2);
    line3->Draw();

    TLegend * leg3 = new TLegend(0.15, 0.91, 0.56, 0.98);
    leg3->AddEntry(g_scalar, "Re(g_{s})", "PE");
    leg3->AddEntry((TObject*)0, Form("Fit: %.3f #pm %.3f", fit_val, fit_err), "");
    leg3->Draw();
    // ----------------------------------------------------------


    gROOT->ForceStyle();
    return 0;
}
