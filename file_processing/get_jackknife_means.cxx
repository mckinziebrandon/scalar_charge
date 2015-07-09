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
#include "../my_data.h"
#include "../my_hists.h"

void get_jackknife_means()
{
    const Int_t nFiles = 164;

    using std::cout;
    using std::endl;

    TFile * f = new TFile("correlation_functions.root");
    TFile * outFile = new TFile("jackknife.root", "RECREATE");
    outFile->cd();

    TH1F * jth_C2_hist[nFiles][2];    
    TH1F * jth_C3_u_hist[nFiles][2];  
    TH1F * jth_C3_d_hist[nFiles][2];  

    // name histograms
    for (int j = 0; j < nFiles; j++)
    {
        jth_C2_hist[j][0]     = new TH1F(Form("C2_real_hist_j_%d", j) , "hist", 64, -0.5, 63.5); 
        jth_C2_hist[j][1]     = new TH1F(Form("C2_imag_hist_j_%d", j) , "hist", 64, -0.5, 63.5); 
        jth_C3_u_hist[j][0]   = new TH1F(Form("C3_u_real_hist_j_%d", j) , "hist", 64, -0.5, 63.5); 
        jth_C3_u_hist[j][1]   = new TH1F(Form("C3_u_imag_hist_j_%d", j) , "hist", 64, -0.5, 63.5); 
        jth_C3_d_hist[j][0]   = new TH1F(Form("C3_d_real_hist_j_%d", j) , "hist", 64, -0.5, 63.5); 
        jth_C3_d_hist[j][1]   = new TH1F(Form("C3_d_imag_hist_j_%d", j) , "hist", 64, -0.5, 63.5); 
    }

    Float_t C2_time_file[nTimes][nFiles][2];
    Float_t C3_u_time_file[nTimes][nFiles][2];
    Float_t C3_d_time_file[nTimes][nFiles][2];

    Float_t jth_C2[nTimes][nFiles][2];
    Float_t jth_C3_u[nTimes][nFiles][2];
    Float_t jth_C3_d[nTimes][nFiles][2];

    TString dirName, tupleName[2];
    dirName  = "../tsrc";
    dirName += 0;
    dirName += "/";

    tupleName[0] = "C2_src"; 
    tupleName[0] += 0;
    tupleName[1] = "C3_src";
    tupleName[1] += 0;

    Data C2_data, C3_data;
    TTree * C2_tree = (TTree*)f->Get(tupleName[0].Data());
    TTree * C3_tree = (TTree*)f->Get(tupleName[1].Data());

    C2_tree->SetBranchAddress("R1", &C2_data.R1);
    C2_tree->SetBranchAddress("I1", &C2_data.I1);
//  C2_tree->SetBranchAddress("R2", &C2_data.R2);
//  C2_tree->SetBranchAddress("I2", &C2_data.I2);

    C3_tree->SetBranchAddress("R1", &C3_data.R1);
    C3_tree->SetBranchAddress("I1", &C3_data.I1);
    C3_tree->SetBranchAddress("R2", &C3_data.R2);
    C3_tree->SetBranchAddress("I2", &C3_data.I2);

    Int_t timeslice, count(0);
    Int_t nEntries = (Int_t)C3_tree->GetEntries();
    cout << nEntries << endl;

    
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

            if (i < 2*nTimes)
            {
                C3_data.Print();
            }

            C2_time_file[timeslice][count][0]   = C2_data.R1;
            C2_time_file[timeslice][count][1]   = C2_data.I1;
            C3_u_time_file[timeslice][count][0] = C3_data.R1;
            C3_u_time_file[timeslice][count][1] = C3_data.I1;
            C3_d_time_file[timeslice][count][0] = C3_data.R2;
            C3_d_time_file[timeslice][count][1] = C3_data.I2;
    }

    for (int time = 0; time < nTimes; time++)
    {
            for (int j = 0; j < nFiles; j++)
            {
                jth_C2[time][j][0] = 0;
                jth_C2[time][j][1] = 0;

                jth_C3_u[time][j][0] = 0;
                jth_C3_u[time][j][1] = 0;
                jth_C3_d[time][j][0] = 0;
                jth_C3_d[time][j][1] = 0;

                for (int file = 0; file < nFiles; file++)
                {
                    if (j != file)
                    {
                        jth_C2[time][j][0] += C2_time_file[time][file][0];
                        jth_C2[time][j][1] += C2_time_file[time][file][1];

                        jth_C3_u[time][j][0] += C3_u_time_file[time][file][0];
                        jth_C3_u[time][j][1] += C3_u_time_file[time][file][1];
                        jth_C3_d[time][j][0] += C3_d_time_file[time][file][0];
                        jth_C3_d[time][j][1] += C3_d_time_file[time][file][1];
                    }
                }

                jth_C2[time][j][0] = jth_C2[time][j][0] / Float_t(nFiles -1);
                jth_C2[time][j][1] = jth_C2[time][j][1] / Float_t(nFiles -1);

                jth_C3_u[time][j][0] = jth_C3_u[time][j][0] / Float_t(nFiles -1);
                jth_C3_u[time][j][1] = jth_C3_u[time][j][1] / Float_t(nFiles -1);
                jth_C3_d[time][j][0] = jth_C3_d[time][j][0] / Float_t(nFiles -1);
                jth_C3_d[time][j][1] = jth_C3_d[time][j][1] / Float_t(nFiles -1);

                jth_C2_hist[j][0]->SetBinContent(time+1, jth_C2[time][j][0]);
                jth_C2_hist[j][1]->SetBinContent(time+1, jth_C2[time][j][1]);

                jth_C3_u_hist[j][0]->SetBinContent(time+1, jth_C3_u[time][j][0]);
                jth_C3_u_hist[j][1]->SetBinContent(time+1, jth_C3_u[time][j][1]);
                jth_C3_d_hist[j][0]->SetBinContent(time+1, jth_C3_d[time][j][0]);
                jth_C3_d_hist[j][1]->SetBinContent(time+1, jth_C3_d[time][j][1]);
            }
    }

    outFile->Write();

}
