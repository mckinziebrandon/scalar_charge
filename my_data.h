#include <iostream>

#ifndef MY_DATA_H
#define MY_DATA_H

class Data
{
private:
    Int_t t;
    Int_t fileNumber;
    Int_t dirNumber;
    Float_t real_1;
    Float_t imag_1;
    Float_t real_2;
    Float_t imag_2;
public:
    Data();
    Int_t Time() const;
    Float_t R1() const;
    Float_t I1() const;
    Float_t R2() const;
    Float_t I2() const;
    void GetLine(std::fstream & f);
    Complex ColPair(Int_t pair) const;
    TString FileName() const;
    void SetSourceLocation(Int_t source);
    void SetFileNumber(Int_t n);
    void AddColPair(const Complex & c, Int_t pair);
    void Print() const;
};

Data::Data()
{
    t = real_1 = imag_1 = real_2 = imag_2 = 0;
    fileNumber = 608;
    dirNumber = 0;
}

Int_t Data::Time() const
{
    return t;
}

Float_t Data::R1() const
{
    return real_1;
}

Float_t Data::I1() const
{
    return imag_1;
}

Float_t Data::R2() const
{
    return real_2;
}

Float_t Data::I2() const
{
    return imag_2;
}

void Data::GetLine(std::fstream & f)
{
    f >> t;
    f >> real_1;
    f >> imag_1;
    f >> real_2;
    f >> imag_2;
}

Complex Data::ColPair(Int_t pair) const
{
    Complex result;
    if (pair == 0)
    {
        result.SetReal(real_1);
        result.SetImag(imag_1);
    }
    else if (pair == 1)
    {
        result.SetReal(real_2);
        result.SetImag(imag_2);
    }
    else
    {
        std::cout << "\nError: pair can only be 0 or 1\n";
    }
    return result;
}

TString Data::FileName() const
{
    TString str;
    str = "../tsrc";
    str += dirNumber;
    str += "/nuc3pt.dat.";
    str += fileNumber;
    return str;
}

void Data::SetSourceLocation(Int_t source)
{
    dirNumber = source;
}

void Data::SetFileNumber(Int_t n)
{
    fileNumber = n;
}

void Data::AddColPair(const Complex & c, Int_t pair)
{
    if (pair == 0)
    {
        real_1 += c.GetReal();    
        imag_1 += c.GetImag();    
    }
    else if (pair == 1)
    {
        real_2 += c.GetReal();    
        imag_2 += c.GetImag();    
    }
    else
    {
        std::cout << "\nError: pair can only be 0 or 1\n";
    }
}

void Data::Print() const
{
    std::cout << t  << "\t";
    std::cout << real_1 << "\t";
    std::cout << imag_1 << "\t";
    std::cout << real_2 << "\t";
    std::cout << imag_2 << std::endl;
}


#endif
