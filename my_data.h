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
    Data(float, float, float, float);
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
    Int_t GetFileNumber();
    void AddColPair(const Complex & c, Int_t pair);
    void Print() const;
    Data operator+(const Data & d) const;
    Data operator/(const Data & d) const;
    void operator=(const Data & d);

    friend class CorrelationTrees;
};

Data::Data()
{
    t = real_1 = imag_1 = real_2 = imag_2 = 0;
    fileNumber = 608;
    dirNumber = 0;
}

Data::Data(Float_t r1, Float_t i1, Float_t r2, Float_t i2)
{
    real_1 = r1;
    imag_1 = i1;
    real_2 = r2;
    imag_2 = i2;
    t = 0;
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

Int_t Data::GetFileNumber()
{
    return fileNumber;
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


Data Data::operator+(const Data & d) const
{
    Data sum;
    sum.t = t + d.t;
    sum.real_1 = real_1 + d.real_1;
    sum.imag_1 = imag_1 + d.imag_1;
    sum.real_2 = real_2 + d.real_2;
    sum.imag_2 = imag_2 + d.imag_2;
    return sum;
}

Data Data::operator/(const Data & d) const
{
    Data result;
    result.t = t / d.t;
    result.real_1 = real_1 / d.real_1;
    result.imag_1 = imag_1 / d.imag_1;
    result.real_2 = real_2 / d.real_2;
    result.imag_2 = imag_2 / d.imag_2;
    return result;
}

void Data::operator=(const Data & d)
{
    t = d.t;
    real_1 = d.real_1;
    imag_1 = d.imag_1;
    real_2 = d.real_2;
    imag_2 = d.imag_2;
}

#endif
