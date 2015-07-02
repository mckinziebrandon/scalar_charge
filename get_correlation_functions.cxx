/*==================================================================
Filename:       get_scalar_charge.cxx
Author:         Brandon McKinzie
Date Created:   Jun 10, 2015
Description:    outputs ntuples containing correlation function column data
                
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

void get_correlation_functions()
{
    using std::cout;
    using std::endl;
    
    // variables
    TString baseName, branchName, directory, fileName; 
    Int_t fileNumber;

    // file associations
    fstream inFile;
    TFile * outFile = new TFile("correlation_functions.root", "RECREATE");
    outFile->cd();

    // tree stuff
    Data data;
    TNtuple * C3_tuple = new TNtuple("C3_tuple", "C3_tuple", "t:R1:I1:R2:I2");
    TNtuple * C2_tuple = new TNtuple("C2_tuple", "C2_tuple", "t:R1:I1:R2:I2");

    for (int file = 0; file < nFiles; file++) 
    {
        // go to appropriate source directory file
        baseName     = "./tsrc0/nuc3pt.dat.";
        fileNumber   = 608 + file * 8;             // gauge configuration id
        fileName    = baseName;
        fileName   += fileNumber;


        // ----------------------------- open this data file and fill trees -------------------------
        inFile.open(fileName.Data());
        if (inFile.is_open())
        {
            cout << "\nSuccessfully Opened: " << fileName.Data() << " to fill Tree branches . . . \n";
           
            // fill 2-pt correlation data into tree
            GotoLine(inFile, lineNumber_C2);
            for (int t = 0; t < nTimes; t++)
            {
                inFile >> data.t;
                inFile >> data.R1;
                inFile >> data.I1;
                inFile >> data.R2;
                inFile >> data.I2;

                C2_tuple->Fill(data.t, data.R1, data.I1, data.R2, data.I2);
            }

            // fill 3-pt correlation data into tree
            GotoLine(inFile, lineNumber_C3);
            for (int t = 0; t < nTimes; t++)
            {
                inFile >> data.t;
                inFile >> data.R1;
                inFile >> data.I1;
                inFile >> data.R2;
                inFile >> data.I2;

                C3_tuple->Fill(data.t, data.R1, data.I1, data.R2, data.I2);
            }
        } 
        else
        {
            cout << "\nError: Could not open " << fileName.Data() << endl;
        }
        // --------------------------------------------------------------------------------------------

        inFile.close();
    } // file loop  

    outFile->Write();
}
