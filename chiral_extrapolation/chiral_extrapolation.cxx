/*==================================================================
Filename:       
Author:         Brandon McKinzie
Date Created:   July 22, 2015
Description:    
==================================================================*/
#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include "TGraphErrors.h"
#include "my_includes.h"

Int_t chiral_extrapolation()
{
    using std::cout;
    using std::endl;
    
    Float_t fit_val(0), fit_err(0);
    TString path1 = getenv("data");
    TString path2 = getenv("data2");
    path1 += "fit_branch/fit.txt";
    path2 += "fit_branch/fit.txt";

    fstream f1(path1.Data());
    fstream f2(path2.Data());
    
    Float_t p1[2];
    Float_t p2[2];

    if (f1.is_open() && f2.is_open())
    {
        f1 >> p1[0] >> p1[1];
        f2 >> p2[0] >> p2[1];

        cout << p1[0] << "\t" << p1[1] << endl;
        cout << p2[0] << "\t" << p2[1];
    }

    TGraphErrors * g = new TGraphErrors();
    g->SetTitle(";Input Quark Mass; g_{s}");

    g->Set(2);
    g->SetPoint(0, 4.2e-3, p1[0]);
    g->SetPoint(1, 1.0e-3, p2[0]);
    g->SetPointError(0, 0, p1[1]);
    g->SetPointError(1, 0, p2[1]);

    TLine * line = new TLine(1.0e-3, p2[0], 4.2e-3, p1[0]);

    // ========================== Drawing ==========================

    TCanvas * MyC = new TCanvas("MyC", "Canvas", 1000, 600);
    MyC->cd();

    g->Draw("AP");
    g->GetXaxis()->CenterTitle();
    g->GetYaxis()->CenterTitle();
    line->Draw();

    TLegend * leg = new TLegend(0.15, 0.91, 0.62, 0.98);
    leg->AddEntry(g, "Isovector Scalar Charge", "PE");
    leg->Draw();

    return 0;
}
