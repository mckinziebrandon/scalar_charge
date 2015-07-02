/*==================================================================
Filename:       get_scalar_charge.cc
Author:         Brandon McKinzie
Date Created:   Jun 10, 2015
Description:    Obtain pion mass via curve fitting the corresponding
                two-point correlation function.
                *** note: could loop over gauge configurations,
                resetting branch addresses every time (if even necessary?)
                and drawing a relevant source-averaged histogram and saving 
                it using davis ROOT voodoo. Then you'd have source-avg hists
                for each gauge configuration
==================================================================*/
#include <iostream>
#include <fstream>
#include <string>
#include <limits>
#include <cstdlib>

#include "my_functions.h"

class Data
{
public:
    Int_t t;
    Float_t R1;
    Float_t I1;
    Float_t R2;
    Float_t I2;
};

void get_scalar_charge()
{
    using std::cout;
    using std::endl;
    
    // variables
    TString baseName, branchName, directory, fileName; 
    Int_t fileNumber;

    // file associations
    fstream inFile;
    TFile * outFile = new TFile("get_scalar_charge.root", "RECREATE");
    outFile->cd();

    // tree stuff
    Data c2, c3;
    TTree * C2_Tree = new TTree("C2_Tree", "C2_Tree");
    TTree * C3_Tree = new TTree("C3_Tree", "C3_Tree");
    C3_Tree->Branch("c3", &c3);
    C2_Tree->Branch("c2", &c2);
    
    for (int source = 0; source < nSources; source++) 
    {
        // go to appropriate source directory file
        fileNumber  = source * 8;       // source location id
        directory   = "./tsrc";
        directory  += fileNumber;
        directory  += "/";
        baseName     = "nuc3pt.dat.";
        fileNumber  = 1000;             // gauge configuration id
        fileName    = directory + baseName;
        fileName   += fileNumber;

        cout << "\nOpened: " << fileName.Data() << " to fill Tree branches . . . \n";

        inFile.open(fileName.Data());
        if (inFile.is_open())
        {
           
            // fill 2-pt correlation data into tree
            GotoLine(inFile, lineNumber_C2);
            for (int t = 0; t < nTimes; t++)
            {
                inFile >> c2.t;
                inFile >> c2.R1;
                inFile >> c2.I1;
                inFile >> c2.R2;
                inFile >> c2.I2;

                C2_Tree->Fill();
            }

            // fill 3-pt correlation data into tree
            GotoLine(inFile, lineNumber_C3);
            for (int t = 0; t < nTimes; t++)
            {
                inFile >> c3.t;
                inFile >> c3.R1;
                inFile >> c3.I1;
                inFile >> c3.R2;
                inFile >> c3.I2;

                C3_Tree->Fill();
            }
        } 
        inFile.close();
    }  
    outFile->Write();
}
