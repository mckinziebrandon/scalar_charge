#include <iostream>
#include <fstream>
#include <string>
#include <limits>
#include <cstdlib>
#include "TBranch.h"
#include "TCanvas.h"
#include "TDirectory.h"
#include "TF1.h"
#include "TFile.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TH1.h"
#include "TH2.h"
#include "TH2F.h"
#include "TLatex.h"
#include "TLegend.h"
#include "TMath.h"
#include "TNtuple.h"
#include "TPad.h"
#include "TTree.h"
#include "TROOT.h"
#include "TStyle.h"


#ifndef MY_FUNCTIONS_H
#define MY_FUNCTIONS_H

class Data
{
public:
    Int_t t;
    Float_t R1;
    Float_t I1;
    Float_t R2;
    Float_t I2;
};

// -------- frequently used constants --------
const Int_t nFiles              = 165;
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
Float_t AverageOverFiles(TH1F** histArray, Int_t inputBin, const Int_t n);
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

Float_t AverageOverFiles(TH1F** histArray, Int_t inputBin, const Int_t n)
{
    Float_t result(0);
    for (Int_t i = 0; i < n; i++)
    {
        result += histArray[i]->GetBinContent(inputBin);
    }

    return result / (Float_t)n;
}

Float_t ErrorOverFiles(TH1F** histArray, Int_t inputBin, const Int_t n, const Float_t avg)
{
    Float_t result(0), bin_content(0);

    for (Int_t i = 0; i < n; i++)
    {
        bin_content  = histArray[i]->GetBinContent(inputBin);
        result      += TMath::Power(bin_content - avg, 2);
    }

    return TMath::Sqrt(result * Float_t(n-1)/n);
}


#endif
