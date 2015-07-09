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
    ~JackHistGroup();
//    NameAllHists();
//    GetC2(Int_t j);
 //   GetC3(Int_t j, Int_t quark);
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

#endif
