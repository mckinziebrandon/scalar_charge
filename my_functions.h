#include <iostream>
#include <fstream>
#include <string>
#include <limits>
#include <cstdlib>
#include "TBranch.h"
#include "TNtuple.h"
#include "TTree.h"
#include "TFile.h"
#include "TH1.h"
#include "TH2.h"
#include "TH2F.h"
#include "TROOT.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TDirectory.h"
#include "TCanvas.h"
#include "TF1.h"
#include "TMath.h"
#include "TLegend.h"
#include "TLatex.h"

#ifndef MY_FUNCTIONS_H
#define MY_FUNCTIONS_H

// -------- frequently used constants --------
const Int_t nFiles              = 164;
const Int_t nTimes              = 64;
const Int_t lineNumber_C3       = 10057;
const Int_t lineNumber_C2       = 5007;
const Int_t snk_src_difference  = 9;
const Int_t nSources            = 7;
// -------------------------------------------

// ---------------------------------------------------
std::fstream& GotoLine(std::fstream&, int);
Double_t StandardError(const Int_t, double, double[]);
Double_t StandardMean(const Int_t, const TH1F*);
Double_t Cosh(Double_t*, Double_t*);
// ---------------------------------------------------

std::fstream& GotoLine(std::fstream& file, int num)
{
    file.seekg(std::ios::beg);
    for(int i=0; i < num - 1; ++i)
    {
            file.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
    }
    return file;
}

Double_t StandardError(const Int_t nPoints, double mean, double point[])
{
    Double_t err = 0;
    for (Int_t i = 0; i < nPoints; i++)
    {
        err += TMath::Power(mean - point[i], 2);
    }

    return TMath::Sqrt(err / (64 - 1)) / 8;
}

Double_t StandardMean(const Int_t nBins, const TH1F* hist)
{
    Double_t mean = 0;
    for (Int_t bin = 1; bin <= nBins; bin++)
    {
        mean += hist->GetBinContent(bin);
    }
    
    return (mean / (Double_t)nBins);
}

Double_t Cosh(Double_t *x, Double_t *par)
{
    Float_t xx = x[0];
    Double_t f = par[0]*TMath::CosH(par[1]*(xx - 28)) + par[2];
    return f;
}



#endif
