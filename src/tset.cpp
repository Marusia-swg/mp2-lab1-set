#include "tset.h"
#include <stdexcept>

TSet::TSet(int mp) : BitField(mp)
{
    MaxPower = mp;
}

TSet::TSet(const TSet &s) : BitField(s.BitField)
{
    MaxPower = s.MaxPower;
}

TSet::TSet(const TBitField &bf) : BitField(bf)
{
    MaxPower = bf.GetLength();
}

TSet::operator TBitField()
{
    return BitField;
}

int TSet::GetMaxPower(void) const
{
    return MaxPower;
}

int TSet::IsMember(const int Elem) const
{
    if (Elem < 0 || Elem >= MaxPower)
        throw std::out_of_range("IsMember: out of range");
    return BitField.GetBit(Elem);
}

void TSet::InsElem(const int Elem)
{
    if (Elem < 0 || Elem >= MaxPower)
        throw std::out_of_range("InsElem: out of range");
    BitField.SetBit(Elem);
}

void TSet::DelElem(const int Elem)
{
    if (Elem < 0 || Elem >= MaxPower)
        throw std::out_of_range("DelElem: out of range");
    BitField.ClrBit(Elem);
}

int TSet::operator==(const TSet &s) const
{
    return BitField == s.BitField;
}

int TSet::operator!=(const TSet &s) const
{
    return BitField != s.BitField;
}

TSet& TSet::operator=(const TSet &s)
{
    if (this == &s) return *this;
    MaxPower = s.MaxPower;
    BitField = s.BitField;
    return *this;
}

TSet TSet::operator+(const int Elem)
{
    if (Elem < 0 || Elem >= MaxPower)
        throw std::out_of_range("operator+: out of range");
    TSet res(*this);
    res.InsElem(Elem);
    return res;
}

TSet TSet::operator-(const int Elem)
{
    if (Elem < 0 || Elem >= MaxPower)
        throw std::out_of_range("operator-: out of range");
    TSet res(*this);
    res.DelElem(Elem);
    return res;
}

TSet TSet::operator+(const TSet &s)
{
    TSet res(*this);
    res.BitField = BitField | s.BitField;
    res.MaxPower = (MaxPower > s.MaxPower) ? MaxPower : s.MaxPower;
    return res;
}

TSet TSet::operator*(const TSet &s)
{
    TSet res(*this);
    res.BitField = BitField & s.BitField;
    res.MaxPower = (MaxPower > s.MaxPower) ? MaxPower : s.MaxPower;
    return res;
}

TSet TSet::operator~(void)
{
    TSet res(*this);
    res.BitField = ~BitField;
    return res;
}

istream& operator>>(istream &istr, TSet &s)
{
    for (int i = 0; i < s.GetMaxPower(); i++)
    {
        int bit;
        istr>> bit;
        if (bit) s.InsElem(i);
        else s.DelElem(i);
    }
    return istr;
}

ostream& operator<<(ostream &ostr, const TSet &s)
{
    for (int i = 0; i < s.MaxPower; i++)
        ostr << s.BitField.GetBit(i);
    return ostr;
}