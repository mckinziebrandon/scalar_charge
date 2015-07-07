/*==================================================================
Filename:       get_scalar_charge.cxx
Author:         Brandon McKinzie
Date Created:   Jun 10, 2015
Description:    outputs ntuples containing correlation function column data
==================================================================*/
#include <iostream>
#include <fstream>
#include <string>
#include <limits>
#include <cstdlib>

#include "../my_functions.h"

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

    // objects for storing raw data
    Data data;
    TNtuple * C3_tuple = new TNtuple("C3_tuple", "C3_tuple", "t:R1:I1:R2:I2");
    TNtuple * C2_tuple = new TNtuple("C2_tuple", "C2_tuple", "t:R1:I1:R2:I2");

    for (int file = 0; file < nFiles; file++) 
    {
        // go to appropriate source directory file
        baseName     = "../tsrc0/nuc3pt.dat.";
        fileNumber   = 608 + file * 8;             // gauge configuration id
        fileName    = baseName;
        fileName   += fileNumber;


        // ----------------------------- open this data file and fill trees -------------------------
        inFile.open(fileName.Data());
        if (inFile.is_open())
        {
            cout << "\nSuccessfully Opened: " << fileName.Data() << " to fill Tree branches . . . \n";
           
            // fill 2-pt correlation 
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

            // fill 3-pt correlation 
            GotoLine(inFile, lineNumber_C3);
            for (int t = 0; t < nTimes; t++)
            {
                    inFile >> data.t;
                    inFile >> data.R1;
                    inFile >> data.I1;
                    inFile >> data.R2;
                    inFile >> data.I2;

                if (t != 9)
                {
                    C3_tuple->Fill(data.t, data.R1, data.I1, data.R2, data.I2);
                }
                else
                {
                    C3_tuple->Fill(0, 0, 0, 0, 0);
                }
                
                if (fileNumber == 608)
                {
                    cout << data.t  << "\t";
                    cout << data.R1 << "\t";
                    cout << data.I1 << "\t";
                    cout << data.R2 << "\t";
                    cout << data.I2 << endl;
                }

            }
        } 
        else
        {
            cout << "\nError: Could not open " << fileName.Data() << endl;
        }
        // --------------------------------------------------------------------------------------------

        inFile.close();
    } // end file loop  

    outFile->Write();
    outFile->Close();
}
