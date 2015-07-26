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

#include "../include/my_functions.h"
#include "../include/my_data.h"

Int_t gcf()
{
    Float_t files_opened = 39;
    using std::cout;
    using std::endl;

    // file associations
    fstream inFile;
    TFile * outFile = new TFile("../rFiles/correlation_functions.root", "RECREATE");
    outFile->cd();

    // objects for storing raw data
    bool here_last = false;
    Data data;
    TString tupleName, thisPos;
    TNtuple * C2; 
    TNtuple * C3;

    // ----- begin source-location loop -----
    Int_t src_arr[nSources] = {0, 16, 32, 48};
    for (int src = 0; src < 4; src++)   
    {
        data.SetSourceLocation(src_arr[src]);                                              
        cout << "\n\nSource: " << data.GetSourceLocation();

        tupleName = "C2_src"; 
        tupleName += data.GetSourceLocation();
        C2 = new TNtuple(tupleName.Data(), tupleName.Data(), "t:R1:I1:R2:I2");     

        tupleName = "C3_src"; 
        tupleName += data.GetSourceLocation();
        C3 = new TNtuple(tupleName.Data(), tupleName.Data(), "t:R1:I1:R2:I2");    
        
        for (int pos = 0; pos < 1; pos++) // begin position loop
        {
            data.SetPosition(pos);
            cout << "\n  Position: " << data.Position();

            Int_t t_init    = data.GetSourceLocation();                              
            Int_t t         = t_init;                                               
            Int_t fNumber;

            here_last = false;

            data.SetFileNumber(748-16);
            for (int file = 0; file < nFiles; file++) // begin file loop
            {
                fNumber = data.GetFileNumber() + 16;

                // skips: 956, 1004, 1036, 1052
                if (fNumber == 956  || fNumber == 1004) fNumber += 16;
                if (fNumber == 1036)                    fNumber += 32;

                data.SetFileNumber(fNumber);
                TString fileName = data.FileName();

                // ------------------ open this data file and fill trees ---------------
                inFile.open(fileName.Data());                                     
                if (inFile.is_open())
                {
                   if (file == nFiles -1) cout << "\n    Opened: " << fileName.Data() << endl;
                   if (here_last) cout << " - " << fNumber - 16;

                    // --------------------- fill 2-pt correlation ---------------------
                    t = t_init;
                    GotoLine(inFile, lineNumber_C2 + t_init);                    
                    while (t < nTimes)                                          
                    {
                        data.GetLine(inFile);                                  
                        C2->Fill(data.Time(), data.R1(), data.I1(), data.R2(), data.I2()); 
                        t++;                                                            
                    }

                    t = 0;
                    GotoLine(inFile, lineNumber_C2);
                    while (t < t_init)
                    {
                        data.GetLine(inFile);
                        C2->Fill(data.Time(), data.R1(), data.I1(), data.R2(), data.I2());
                        t++;
                    }                                                                  

                    // --------------------- fill 3-pt correlation ---------------------
                    t = t_init;
                    GotoLine(inFile, lineNumber_C3 + t_init);
                    while (t < nTimes)
                    {
                        data.GetLine(inFile);
                        C3->Fill(data.Time(), data.R1(), data.I1(), data.R2(), data.I2());
                        t++;
                    }

                    t = 0;
                    GotoLine(inFile, lineNumber_C3);
                    while (t < t_init)
                    {
                        data.GetLine(inFile);
                        C3->Fill(data.Time(), data.R1(), data.I1(), data.R2(), data.I2());
                        t++;
                    }

                    here_last = false;
                } 
                else 
                {
                    cout << "\n    Error: Could not open " << fileName.Data(); 
                    here_last = true;
                    files_opened -= 1;
                }

                inFile.close();
            } // end file loop  
        } // end position loop

        C2->Write();
        C3->Write();

        delete C2;
        delete C3;
    }  // ----- end source-location loop -----
    
    outFile->Close();
    return 0;
}
