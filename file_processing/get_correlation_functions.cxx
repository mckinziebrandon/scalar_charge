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
#include "../my_data.h"

void get_correlation_functions()
{
    using std::cout;
    using std::endl;

    // file associations
    fstream inFile;
    TFile * outFile = new TFile("correlation_functions.root", "RECREATE");
    outFile->cd();

    // objects for storing raw data
    Data data;
    TString tupleName[2];
    TNtuple * cFuncs[2];

    // ----- begin source-location loop -----
    for (int dir = 0; dir < 1; dir++)   
    {
        data.SetSourceLocation(8*dir);

        tupleName[0] = "C2_src"; 
        tupleName[0] += dir;
        tupleName[1] = "C3_src";
        tupleName[1] += dir;

        cFuncs[0] = new TNtuple(tupleName[0].Data(), tupleName[0].Data(), "t:R1:I1:R2:I2");
        cFuncs[1] = new TNtuple(tupleName[1].Data(), tupleName[1].Data(), "t:R1:I1:R2:I2");

        for (int file = 0; file < nFiles; file++) 
        {
            data.SetFileNumber(608 + 8*file);
            TString fileName = data.FileName();

            // ------------------ open this data file and fill trees ---------------
            inFile.open(fileName.Data());
            if (inFile.is_open())
            {
               
                // --------------------- fill 2-pt correlation ---------------------
                GotoLine(inFile, lineNumber_C2);
                for (int t = 0; t < nTimes; t++)
                {
                    data.GetLine(inFile);
                    cFuncs[0]->Fill(data.Time(), data.R1(), data.I1(), data.R2(), data.I2());

                    if (file < 2)
                    {
                        if (t == 0) cout << "\n\nTWO POINT CORRELATION: " << fileName.Data() << "\n\n"; 
                        data.Print();
                    }
                }

                // --------------------- fill 3-pt correlation ---------------------
                GotoLine(inFile, lineNumber_C3);
                for (int t = 0; t < nTimes; t++)
                {
                    data.GetLine(inFile);
                    if (t != 9 + 8*dir)
                    {
                        cFuncs[1]->Fill(data.Time(), data.R1(), data.I1(), data.R2(), data.I2());
                    }
                    else
                    {
                        cFuncs[1]->Fill(0, 0, 0, 0, 0);
                    }
                    
                    if (file < 2)
                    {

                        if (t == 0) cout << "\n\nTHREE POINT CORRELATION: " << fileName.Data() << "\n\n"; 
                        data.Print();
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
