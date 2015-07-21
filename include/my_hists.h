#include <iostream>

#ifndef MY_HISTS_H
#define MY_HISTS_H

class JackHistGroup
{
private:
    TH1F * C2[nFiles][2];
    TH1F * C3_up[nFiles][2];
    TH1F * C3_down[nFiles][2];
public:
    JackHistGroup();
    JackHistGroup(TFile* f);
    ~JackHistGroup();
    TH1F* GetC2(Int_t j, TString str);
    TH1F* GetC3_up(Int_t j, TString str);
    TH1F* GetC3_down(Int_t j, TString str);
};

JackHistGroup::JackHistGroup()
{
    for (int j = 0; j < nFiles; j++)
    {
        C2[j][0]        = new TH1F(Form("C2_real_j_%d", j) , "hist", 64, -0.5, 63.5); 
        C2[j][1]        = new TH1F(Form("C2_imag_j_%d", j) , "hist", 64, -0.5, 63.5); 
        C3_up[j][0]     = new TH1F(Form("C3_u_real_j_%d", j) , "hist", 64, -0.5, 63.5); 
        C3_up[j][1]     = new TH1F(Form("C3_u_imag_j_%d", j) , "hist", 64, -0.5, 63.5); 
        C3_down[j][0]   = new TH1F(Form("C3_d_real_j_%d", j) , "hist", 64, -0.5, 63.5); 
        C3_down[j][1]   = new TH1F(Form("C3_d_imag_j_%d", j) , "hist", 64, -0.5, 63.5); 
    }
}

JackHistGroup::JackHistGroup(TFile* f)
{
    for (int j = 0; j < nFiles; j++)
    {
        C2[j][0]        = (TH1F*)f->Get(Form("C2_real_j_%d;1", j));
        C2[j][1]        = (TH1F*)f->Get(Form("C2_imag_j_%d;1", j));
        C3_up[j][0]     = (TH1F*)f->Get(Form("C3_u_real_j_%d;1", j));
        C3_up[j][1]     = (TH1F*)f->Get(Form("C3_u_imag_j_%d;1", j));
        C3_down[j][0]   = (TH1F*)f->Get(Form("C3_d_real_j_%d;1", j));
        C3_down[j][1]   = (TH1F*)f->Get(Form("C3_d_imag_j_%d;1", j));
    }

}
JackHistGroup::~JackHistGroup()
{
    for (int j = 0; j < nFiles; j++)
    {
        delete C2[j][0];       
        delete C2[j][1];      
        delete C3_up[j][0];
        delete C3_up[j][1]; 
        delete C3_down[j][0];
        delete C3_down[j][1];
    }
}

TH1F* JackHistGroup::GetC2(Int_t j, TString str)
{
    if (str == "real")
        return C2[j][0];
    else if (str == "imag")
        return C2[j][1];
    else
        std::cout << "\nYou did something wrong\n";
        return (TH1F*)0;
}

TH1F* JackHistGroup::GetC3_up(Int_t j, TString str)
{
    if (str == "real")
        return C3_up[j][0];
    else if (str == "imag")
        return C3_up[j][1];
    else
        std::cout << "\nYou did something wrong\n";
        return (TH1F*)0;
}

TH1F* JackHistGroup::GetC3_down(Int_t j, TString str)
{
    if (str == "real")
        return C3_down[j][0];
    else if (str == "imag")
        return C3_down[j][1];
    else
        std::cout << "\nYou did something wrong\n";
        return (TH1F*)0;
}

#endif
