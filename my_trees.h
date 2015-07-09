#include <iostream>

#ifndef MY_TREES_H
#define MY_TREES_H

class CorrelationTrees
{
private:
    TTree* C2[nSources];
    TTree* C3[nSources];
    TNtuple* C2_avg;
    TNtuple* C3_avg;
public:
    CorrelationTrees();
    TTree* GetC2Tree(Int_t src); 
    TTree* GetC3Tree(Int_t src); 
    void SetC2Tree(Int_t src, TTree* t); 
    void SetC3Tree(Int_t src, TTree* t); 
    void SetBranchAddresses(Data* d2, Data* d3);
    Data SourceAveragedData(Data* d, Int_t corrNum, Int_t i);

    friend class TTree;
};

CorrelationTrees::CorrelationTrees()
{
    std::cout << "\nEmpty correlation trees constructed.\n";
}

void CorrelationTrees::SetBranchAddresses(Data* d2, Data* d3)
{
    for (Int_t src = 0; src < nSources; src++)
    {
        C2[src]->SetBranchAddress("R1", &d2[src].real_1);
        C2[src]->SetBranchAddress("I1", &d2[src].imag_1);
        C2[src]->SetBranchAddress("R2", &d2[src].real_2);
        C2[src]->SetBranchAddress("I2", &d2[src].imag_2);

        C3[src]->SetBranchAddress("R1", &d3[src].real_1);
        C3[src]->SetBranchAddress("I1", &d3[src].imag_1);
        C3[src]->SetBranchAddress("R2", &d3[src].real_2);
        C3[src]->SetBranchAddress("I2", &d3[src].imag_2);
    }
}

TTree* CorrelationTrees::GetC2Tree(Int_t src)
{
    return C2[src];
}

TTree* CorrelationTrees::GetC3Tree(Int_t src)
{
    return C3[src];
}

void CorrelationTrees::SetC2Tree(Int_t src, TTree* t)
{
    C2[src] = t;    
}

void CorrelationTrees::SetC3Tree(Int_t src, TTree* t)
{
    C3[src] = t;    
}

Data CorrelationTrees::SourceAveragedData(Data* d, Int_t corrNum, Int_t i)
{
    Float_t avg[4];
    avg[0] = avg[1] = avg[2] = avg[3] = 0;

    for (Int_t src = 0; src < 1; src++)
    {
        if      (corrNum == 2) C2[src]->GetEntry(i);
        else if (corrNum == 3) C3[src]->GetEntry(i);
        else    std::cout << "\nYou messed something up.\n";

        avg[0] += d[src].real_1;
        avg[1] += d[src].imag_1;
        avg[2] += d[src].real_2;
        avg[3] += d[src].imag_2;
        
    }

    for (Int_t k = 0; k < 4; k++)
    {
        if (i / nTimes == 0 || i / nTimes == 2 || i / nTimes == 4)
            avg[k] = avg[k] / (nSources-1);
        else
            avg[k] = avg[k] / nSources;
    }

    Data result(avg[0], avg[1], avg[2], avg[3]);
    return result;
}

#endif
