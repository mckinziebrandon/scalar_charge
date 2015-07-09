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
#include "../my_trees.h"

void get_jackknife_means()
{
    const Int_t nFiles = 164;

    using std::cout;
    using std::endl;

    TFile * f = new TFile("correlation_functions.root");
    TFile * outFile = new TFile("jackknife.root", "RECREATE");
    outFile->cd();

    Float_t C2_time_file[nTimes][nFiles][2];
    Float_t C3_u_time_file[nTimes][nFiles][2];
    Float_t C3_d_time_file[nTimes][nFiles][2];

    Float_t jth_C2[nTimes][nFiles][2];
    Float_t jth_C3_u[nTimes][nFiles][2];
    Float_t jth_C3_d[nTimes][nFiles][2];

    JackHistGroup jackHists;
    CorrelationTrees * CTrees = new CorrelationTrees();
    Data C2_data[nSources]; 
    Data C3_data[nSources];
    Data avg_C2_data;
    Data avg_C3_data;

    TString tupleName;
    // initialize source-specific trees that hold input data
    for (Int_t src = 0; src < nSources; src++)
    {
        tupleName = "C2_src"; 
        tupleName += 8*src;
        CTrees->SetC2Tree(src, (TTree*)f->Get(tupleName.Data()));

        tupleName = "C3_src"; 
        tupleName += 8*src;
        CTrees->SetC3Tree(src, (TTree*)f->Get(tupleName.Data()));
    }
        
    CTrees->SetBranchAddresses(C2_data, C3_data);

    Int_t timeslice, count(0);
    Int_t nEntries = (Int_t)CTrees->GetC3Tree(0)->GetEntries();
    cout << nEntries << endl;
    
    // perform source average
    for (int i = 0; i < nEntries; i++)
    {
        timeslice = i%nTimes;

        avg_C2_data = CTrees->SourceAveragedData(C2_data, 2, i);
        avg_C3_data = CTrees->SourceAveragedData(C3_data, 3, i);
        avg_C2_data.Print();
    }
/*
        if (timeslice == 0 && i !=0)
        {
            fcount++;
        }

        C2_time_file[timeslice][count][0]   = C2_data.R1;
        C2_time_file[timeslice][count][1]   = C2_data.I1;
        C3_u_time_file[timeslice][count][0] = C3_data.R1;
        C3_u_time_file[timeslice][count][1] = C3_data.I1;
        C3_d_time_file[timeslice][count][0] = C3_data.R2;
        C3_d_time_file[timeslice][count][1] = C3_data.I2;

        }
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
*/
}
