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

Int_t plot_C3()
{
    using std::cout;
    using std::endl;

    const Int_t nFiles = 164;

    TFile * f = new TFile("../file_processing/jackknife.root");
    TFile * outFile = new TFile("plot_C3.root", "RECREATE");
    outFile->cd();

    TH1F * C3_up_hist       = new TH1F("C3_up_hist", "C3_up_hist", 64, -0.5, 63.5);
    TH1F * C3_down_hist     = new TH1F("C3_down_hist", "C3_down_hist", 64, -0.5, 63.5);
    TH1F * C3_up_down_hist  = new TH1F("C3_up_down_hist", "C3_up_down_hist", 64, -0.5, 63.5);

    TH1F * jth_C3_u_hist_real[nFiles];    
    TH1F * jth_C3_d_hist_real[nFiles];    

    for (int j = 0; j < nFiles; j++)
    {
        jth_C3_u_hist_real[j]   = (TH1F*)f->Get(Form("C3_u_real_hist_j_%d;1", j));
        jth_C3_d_hist_real[j]   = (TH1F*)f->Get(Form("C3_d_real_hist_j_%d;1", j));
    }

    Float_t average_up_value, average_down_value;
    Float_t up_err, down_err, up_down_err;
    for (int t = 0; t < nTimes; t++)
    {
        if (t+1 != 10)
        {
            average_up_value    = AverageOverFiles(jth_C3_u_hist_real, t+1, nFiles);
            average_down_value  = AverageOverFiles(jth_C3_d_hist_real, t+1, nFiles);

            up_err          = ErrorOverFiles(jth_C3_u_hist_real, t+1, nFiles, average_up_value);
            down_err        = ErrorOverFiles(jth_C3_d_hist_real, t+1, nFiles, average_down_value);

            C3_up_hist      ->SetBinContent(t+1, average_up_value);
            C3_down_hist    ->SetBinContent(t+1, average_down_value);
            C3_up_down_hist ->SetBinContent(t+1, average_up_value - average_down_value);

            C3_up_hist      ->SetBinError(t+1, up_err);
            C3_down_hist    ->SetBinError(t+1, down_err);

            cout << up_err << endl;
        }
    }

    // ========================== Drawing ==========================
    TCanvas * MyC   = new TCanvas("MyC", "Canvas", 1400, 700);
    TPad * pad1     = new TPad("pad1", "Pad", 0.025, 0.025, 0.975, 0.975);
    pad1->Divide(3, 1);
    gPad->SetTickx();
    gPad->SetTicky();
    pad1->Draw();

    TLegend * legend = new TLegend(0.5, 0.8, 0.9, 0.9);
    TLegend * legend2 = new TLegend(0.5, 0.8, 0.9, 0.9);
    TLegend * legend3 = new TLegend(0.5, 0.8, 0.9, 0.9);
    legend->AddEntry(C3_up_hist,"<C^{(3)}> Up-Quark","PE");
    legend2->AddEntry(C3_down_hist,"<C^{(3)}> Down-Quark","PE");
    legend3->AddEntry(C3_up_down_hist,"<C^{(3)}> Both Quarks","PE");

    MyC->cd();

    pad1->cd(1);
    C3_up_hist->SetMarkerStyle(26);
    C3_up_hist->Draw("CPE1");
    legend->Draw();

    pad1->cd(2);
    C3_down_hist->SetMarkerStyle(26);
    C3_down_hist->Draw("CPE1");
    legend2->Draw();

    pad1->cd(3);
    C3_up_down_hist->SetMarkerStyle(26);
    C3_up_down_hist->Draw("CP");
    legend3->Draw();

    //MyC->Update();

    return 0;
}
