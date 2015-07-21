#ifndef MY_FUNCTIONS_H
#define MY_FUNCTIONS_H

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
#include "TGaxis.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TH1.h"
#include "TH2.h"
#include "TH2F.h"
#include "TLatex.h"
#include "TLine.h"
#include "TLegend.h"
#include "TMath.h"
#include "TNtuple.h"
#include "TPad.h"
#include "TTree.h"
#include "TROOT.h"
#include "TStyle.h"
#include "TSystem.h"


// -------- frequently used constants --------
const Int_t nFiles              = 170;
const Int_t nTimes              = 64;
const Int_t lineNumber_C3       = 10057;
const Int_t lineNumber_C2       = 5007;
const Int_t snk_src_difference  = 9;
const Int_t nSources            = 4;
// -------------------------------------------

// ---------------------------------------------------
std::fstream& GotoLine(std::fstream&, int);
Double_t Cosh(Double_t*, Double_t*);
Double_t Const(Double_t *x, Double_t *par);
Float_t AverageOverFiles(TH1F** histArray, Int_t inputBin, const Int_t n);
Float_t ErrorOverFiles(TH1F** histArray, Int_t inputBin, const Int_t n, const Float_t avg);
// ---------------------------------------------------

class Complex
{
private:
    Float_t real;
    Float_t imag;
    Float_t magnitude;
    void set_mag();
public:
    Complex();
    Complex(Float_t re, Float_t im); // not implemented
    Float_t GetReal() const;   
    Float_t GetImag() const;
    Float_t GetMagnitude() const;
    void SetReal(Float_t re);   
    void SetImag(Float_t im);
    Complex operator+(const Complex & c) const;
    Complex operator-(const Complex & c) const;
    Complex operator/(const Complex & c) const;
};

// ==================== CLASS METHODS/DECLARATIONS ====================

Complex::Complex()
{
    real = imag = magnitude =  0;
}

Complex::Complex(Float_t re, Float_t im)
{
    real = re;
    imag = im;
    magnitude = TMath::Sqrt(re*re + im*im);
}

Float_t Complex::GetReal() const
{
    return real;
}

Float_t Complex::GetImag() const
{
    return imag;
}

Float_t Complex::GetMagnitude() const
{
    return magnitude;
}

void Complex::SetReal(Float_t re)
{
    real = re; 
    this->set_mag();
}

void Complex::SetImag(Float_t im)
{
    imag = im; 
    this->set_mag();
}

void Complex::set_mag()
{
    magnitude = TMath::Sqrt(real*real + imag*imag);
}

Complex Complex::operator+(const Complex & c) const
{
    Complex sum;
    sum.real = real + c.real;
    sum.imag = imag + c.imag;
    sum.set_mag();
    return sum;
}

Complex Complex::operator-(const Complex & c) const
{
    Complex diff;
    diff.real = real - c.real;
    diff.imag = imag - c.imag;
    diff.set_mag();
    return diff;
}

Complex Complex::operator/(const Complex & c) const
{
    Complex result;
    result.real = (real*c.real + imag*c.imag) / (c.real*c.real + c.imag*c.imag);
    result.imag = (imag*c.real - real*c.imag) / (c.real*c.real + c.imag*c.imag);
    result.set_mag();
    return result;
}

// ==================== INDEPENDENT FUNCTIONS =========================

// puts file reader at beginning of desired line
std::fstream& GotoLine(std::fstream& file, int num)
{
    file.seekg(std::ios::beg);
    for(int i=0; i < num - 1; ++i)
    {
            file.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
    }
    return file;
}

// pion two point correlation cosh fitter function
Double_t Cosh(Double_t *x, Double_t *par)
{
    Float_t xx = x[0];
    Double_t f = par[0]*TMath::CosH(par[1]*(xx - 28)) + par[2];
    return f;
}

Double_t Const(Double_t *x, Double_t *par)
{
    Float_t xx = x[0];
    Double_t f = par[0];
    return f;
}

// takes a hist array and averages over n files at specific inputBin
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
