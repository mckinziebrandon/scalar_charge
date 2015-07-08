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
    TString baseName, branchName, dirName, fileName; 
    TString tupleName[2];
    Int_t fileNumber;

    // file associations
    fstream inFile;
    TFile * outFile = new TFile("correlation_functions.root", "RECREATE");
    outFile->cd();

    // objects for storing raw data
    Data data;
    TNtuple * cFuncs[2];

    // ----- begin source-location loop -----
    for (int dir = 0; dir < nSources; dir++)   
    {
        dirName  = "../tsrc";
        dirName += dir*8;
        dirName += "/";

        tupleName[0] = "C2"; 
        tupleName[0] += dirName;
        tupleName[1] = "C3";
        tupleName[1] += dirName;

        cFuncs[0] = new TNtuple(tupleName[0].Data(), "cFuncs[0]", "t:R1:I1:R2:I2");
        cFuncs[1] = new TNtuple(tupleName[1].Data(), "cFuncs[1]", "t:R1:I1:R2:I2");

        for (int file = 0; file < nFiles; file++) 
        {
            // go to appropriate source directory file
            baseName    = dirName;
            baseName    += "nuc3pt.dat.";
            fileNumber   = 608 + file * 8;             // gauge configuration id
            fileName    = baseName;
            fileName   += fileNumber;


            // ----------------------------- open this data file and fill trees -------------------------
            inFile.open(fileName.Data());
            if (inFile.is_open())
            {
                //cout << "\nSuccessfully Opened: " << fileName.Data() << " to fill Tree branches . . . \n";
               
                // fill 2-pt correlation 
                GotoLine(inFile, lineNumber_C2);
                for (int t = 0; t < nTimes; t++)
                {
                        inFile >> data.t;
                        inFile >> data.R1;
                        inFile >> data.I1;
                        inFile >> data.R2;
                        inFile >> data.I2;

                        cFuncs[0]->Fill(data.t, data.R1, data.I1, data.R2, data.I2);

                    if (fileNumber < 630)
                    {
                        if (t == 0)
                        {
                            cout << "\n\nTWO POINT CORRELATION: " << fileName.Data() << "\n\n"; 
                        }
                        cout << data.t  << "\t";
                        cout << data.R1 << "\t";
                        cout << data.I1 << "\t";
                        cout << data.R2 << "\t";
                        cout << data.I2 << endl;
                    }
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

                    if (t != 9 + 8*dir)
                    {
                        cFuncs[1]->Fill(data.t, data.R1, data.I1, data.R2, data.I2);
                    }
                    else
                    {
                        cFuncs[1]->Fill(0, 0, 0, 0, 0);
                    }
                    
                    if (fileNumber < 630)
                    {

                        if (t == 0)
                        {
                            cout << "\n\nTHREE POINT CORRELATION: " << fileName.Data() << "\n\n"; 
                        }
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

        cFuncs[0]->Write();
        cFuncs[1]->Write();

        delete cFuncs[0];
        delete cFuncs[1];

    }  // ----- end source-location loop -----
    
    outFile->Close();
}
