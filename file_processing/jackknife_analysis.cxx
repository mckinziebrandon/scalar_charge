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

void jackknife_analysis()
{
    const Int_t nFiles = 164;

    using std::cout;
    using std::endl;

    TFile * f = new TFile("correlation_functions.root");
    TFile * outFile = new TFile("jackknife.root", "RECREATE");
    outFile->cd();

    Data C2_data, C3_data;
    TTree * C2_tree = (TTree*)f->Get("C2_tuple;1");
    TTree * C3_tree = (TTree*)f->Get("C3_tuple;1");

    C2_tree->SetBranchAddress("R1", &C2_data.R1);
    C2_tree->SetBranchAddress("I1", &C2_data.I1);
//  C2_tree->SetBranchAddress("R2", &C2_data.R2);
//  C2_tree->SetBranchAddress("I2", &C2_data.I2);

    C3_tree->SetBranchAddress("R1", &C3_data.R1);
    C3_tree->SetBranchAddress("I1", &C3_data.I1);
    C3_tree->SetBranchAddress("R2", &C3_data.R2);
    C3_tree->SetBranchAddress("I2", &C3_data.I2);

    TH1F * jth_C2_hist_real[nFiles];    // jth jacknife histogram
    TH1F * jth_C2_hist_imag[nFiles];    // jth jacknife histogram
    TH1F * jth_C3_u_hist_real[nFiles];    // jth jacknife histogram
    TH1F * jth_C3_u_hist_imag[nFiles];    // jth jacknife histogram
    TH1F * jth_C3_d_hist_real[nFiles];    // jth jacknife histogram
    TH1F * jth_C3_d_hist_imag[nFiles];    // jth jacknife histogram

    // name histograms
    for (int j = 0; j < nFiles; j++)
    {
        jth_C2_hist_real[j] = new TH1F(Form("C2_real_hist_j_%d", j) , "hist", 64, -0.5, 63.5); 
        jth_C2_hist_imag[j] = new TH1F(Form("C2_imag_hist_j_%d", j) , "hist", 64, -0.5, 63.5); 
        jth_C3_u_hist_real[j] = new TH1F(Form("C3_u_real_hist_j_%d", j) , "hist", 64, -0.5, 63.5); 
        jth_C3_u_hist_imag[j] = new TH1F(Form("C3_u_imag_hist_j_%d", j) , "hist", 64, -0.5, 63.5); 
        jth_C3_d_hist_real[j] = new TH1F(Form("C3_d_real_hist_j_%d", j) , "hist", 64, -0.5, 63.5); 
        jth_C3_d_hist_imag[j] = new TH1F(Form("C3_d_imag_hist_j_%d", j) , "hist", 64, -0.5, 63.5); 
    }

    Float_t C2_RFunc_time_file[nTimes][nFiles];
    Float_t C2_IFunc_time_file[nTimes][nFiles];
    Float_t C3_u_RFunc_time_file[nTimes][nFiles];
    Float_t C3_u_IFunc_time_file[nTimes][nFiles];
    Float_t C3_d_RFunc_time_file[nTimes][nFiles];
    Float_t C3_d_IFunc_time_file[nTimes][nFiles];

    Float_t C2_jth_RFunc[nTimes][nFiles];
    Float_t C2_jth_IFunc[nTimes][nFiles];
    Float_t C3_u_jth_RFunc[nTimes][nFiles];
    Float_t C3_u_jth_IFunc[nTimes][nFiles];
    Float_t C3_d_jth_RFunc[nTimes][nFiles];
    Float_t C3_d_jth_IFunc[nTimes][nFiles];

    Int_t timeslice, count(0);
    Int_t nEntries = (Int_t)C3_tree->GetEntries();

    // organize data into array
    for (int i = 0; i < nEntries; i++)
    {
        timeslice = i%nTimes;
        C2_tree->GetEntry(i);
        C3_tree->GetEntry(i);

        if (timeslice == 0 && i !=0)
        {
            count++;
        }

        C2_RFunc_time_file[timeslice][count] = C2_data.R1;
        C2_IFunc_time_file[timeslice][count] = C2_data.I1;
        C3_u_RFunc_time_file[timeslice][count] = C3_data.R1;
        C3_u_IFunc_time_file[timeslice][count] = C3_data.I1;
        C3_d_RFunc_time_file[timeslice][count] = C3_data.R2;
        C3_d_IFunc_time_file[timeslice][count] = C3_data.I2;
    }

    for (int time = 0; time < nTimes; time++)
    {
        for (int j = 0; j < nFiles; j++)
        {
            C2_jth_RFunc[time][j] = 0;
            C2_jth_IFunc[time][j] = 0;
            C3_u_jth_RFunc[time][j] = 0;
            C3_u_jth_IFunc[time][j] = 0;
            C3_d_jth_RFunc[time][j] = 0;
            C3_d_jth_IFunc[time][j] = 0;

            for (int file = 0; file < nFiles; file++)
            {
                if (j != file)
                {
                    C2_jth_RFunc[time][j] += C2_RFunc_time_file[time][file];
                    C2_jth_IFunc[time][j] += C2_IFunc_time_file[time][file];
                    C3_u_jth_RFunc[time][j] += C3_u_RFunc_time_file[time][file];
                    C3_u_jth_IFunc[time][j] += C3_u_IFunc_time_file[time][file];
                    C3_d_jth_RFunc[time][j] += C3_d_RFunc_time_file[time][file];
                    C3_d_jth_IFunc[time][j] += C3_d_IFunc_time_file[time][file];
                }
            }

            C2_jth_RFunc[time][j] = C2_jth_RFunc[time][j] / Float_t(nFiles -1);
            C2_jth_IFunc[time][j] = C2_jth_IFunc[time][j] / Float_t(nFiles -1);
            C3_u_jth_RFunc[time][j] = C3_u_jth_RFunc[time][j] / Float_t(nFiles -1);
            C3_u_jth_IFunc[time][j] = C3_u_jth_IFunc[time][j] / Float_t(nFiles -1);
            C3_d_jth_RFunc[time][j] = C3_d_jth_RFunc[time][j] / Float_t(nFiles -1);
            C3_d_jth_IFunc[time][j] = C3_d_jth_IFunc[time][j] / Float_t(nFiles -1);

            jth_C2_hist_real[j]->SetBinContent(time+1, C2_jth_RFunc[time][j]);
            jth_C2_hist_imag[j]->SetBinContent(time+1, C2_jth_IFunc[time][j]);
            jth_C3_u_hist_real[j]->SetBinContent(time+1, C3_u_jth_RFunc[time][j]);
            jth_C3_u_hist_imag[j]->SetBinContent(time+1, C3_u_jth_IFunc[time][j]);
            jth_C3_d_hist_real[j]->SetBinContent(time+1, C3_d_jth_RFunc[time][j]);
            jth_C3_d_hist_imag[j]->SetBinContent(time+1, C3_d_jth_IFunc[time][j]);
        }
    }

    outFile->Write();

}
