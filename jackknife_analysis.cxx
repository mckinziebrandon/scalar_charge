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

#include "my_functions.h"

void jackknife_analysis()
{
    using std::cout;
    using std::endl;

    TFile * f = new TFile("correlation_functions.root");
    TFile * outFile = new TFile("jackknife.root", "RECREATE");
    outFile->cd();

    Data C2_data, C3_data;
    TTree * C2_tree = (TTree*)f->Get("C2_tuple;1");
    TTree * C3_tree = (TTree*)f->Get("C3_tuple;1");

    C2_tree->SetBranchAddress("t",  &C2_data.t);
    C2_tree->SetBranchAddress("R1", &C2_data.R1);
    C2_tree->SetBranchAddress("I1", &C2_data.I1);
    C2_tree->SetBranchAddress("R2", &C2_data.R2);
    C2_tree->SetBranchAddress("I2", &C2_data.I2);

    C3_tree->SetBranchAddress("t",  &C3_data.t);
    C3_tree->SetBranchAddress("R1", &C3_data.R1);
    C3_tree->SetBranchAddress("I1", &C3_data.I1);
    C3_tree->SetBranchAddress("R2", &C3_data.R2);
    C3_tree->SetBranchAddress("I2", &C3_data.I2);

    TH1F * jth_hist_real[nFiles];    // jth jacknife histogram
    TH1F * jth_hist_imag[nFiles];    // jth jacknife histogram
    TH1F * cfunc_hist = new TH1F("cfunc_hist", "hist", 64, -0.5, 63.5);
    for (int j = 0; j < nFiles; j++)
    {
        jth_hist_real[j] = new TH1F(Form("real_hist_j_%d", j) , "hist", 64, -0.5, 63.5); 
        jth_hist_imag[j] = new TH1F(Form("imag_hist_j_%d", j) , "hist", 64, -0.5, 63.5); 
    }

    Int_t timeslice;
    Int_t nEntries = (Int_t)C3_tree->GetEntries();
    for (Int_t i = 0; i < nEntries; i++)
    {
        timeslice = i % nTimes;
        C3_tree->GetEntry(i);
        Float_t real_bin, imag_bin;
        
        for (Int_t j = 0; j < nFiles; j++) // histogram identifier
        {
            for (Int_t file = 0; file < nFiles; file++) // jth file identifier
            {
                if (file != j)
                {
                    real_bin = jth_hist_real[j]->GetBinContent(timeslice+1);
                    imag_bin = jth_hist_imag[j]->GetBinContent(timeslice+1);

                    jth_hist_real[j]->SetBinContent(timeslice+1, real_bin + C3_data.R1/(nFiles-1));
                    jth_hist_imag[j]->SetBinContent(timeslice+1, imag_bin + C3_data.I1/(nFiles-1));
                }

            }
        } // --- end jth file loop ---
    } // --- end overall entry loop ---

    outFile->Write();



}
