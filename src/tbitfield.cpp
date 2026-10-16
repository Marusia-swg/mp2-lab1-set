#include "tbitfield.h"
#include <stdexcept>

#define BITS_PER_ELEM (8 * sizeof(TELEM))

TBitField::TBitField(int len)
{
    if (len <= 0)
        throw std::invalid_argument("TBitField: length must be positive");
    BitLen = len;
    MemLen = (len + BITS_PER_ELEM - 1) / BITS_PER_ELEM;
    pMem = new TELEM[MemLen];
    for (int i = 0; i < MemLen; i++)
        pMem[i] = 0;
}

TBitField::TBitField(const TBitField &bf)
{
    BitLen = bf.GetLength();
    MemLen = bf.MemLen;
    pMem = new TELEM[MemLen];
    for (int i = 0; i < MemLen; i++)
        pMem[i] = bf.pMem[i];
}

TBitField::~TBitField()
{
    delete[] pMem;
    pMem = nullptr;
}

int TBitField::GetMemIndex(const int n) const
{
    return n / BITS_PER_ELEM;
}

TELEM TBitField::GetMemMask(const int n) const
{
    return (TELEM)1 << (n % BITS_PER_ELEM);
}

int TBitField::GetLength(void) const
{
    return BitLen;
}

void TBitField::SetBit(const int n)
{
    if (n < 0 || n >= BitLen)
        throw std::out_of_range("SetBit: index out of range");
    pMem[GetMemIndex(n)] |= GetMemMask(n);
}

void TBitField::ClrBit(const int n)
{
    if (n < 0 || n >= BitLen)
        throw std::out_of_range("ClrBit: index out of range");
    pMem[GetMemIndex(n)] &= ~GetMemMask(n);
}

int TBitField::GetBit(const int n) const
{
    if (n < 0 || n >= BitLen)
        throw std::out_of_range("GetBit: index out of range");
    return (pMem[GetMemIndex(n)] & GetMemMask(n)) ? 1 : 0;
}

TBitField& TBitField::operator=(const TBitField &bf)
{
    if (this == &bf)
        return *this;
    if (MemLen != bf.MemLen)
    {
        delete[] pMem;
        MemLen = bf.MemLen;
        pMem = new TELEM[MemLen];
    }
    BitLen = bf.GetLength();
    for (int i = 0; i < MemLen; i++)
        pMem[i] = bf.pMem[i];
    return *this;
}

int TBitField::operator==(const TBitField &bf) const
{
    if (BitLen != bf.GetLength()) return 0;
    for (int i = 0; i < MemLen; i++)
        if (pMem[i] != bf.pMem[i]) return 0;
    return 1;
}

int TBitField::operator!=(const TBitField &bf) const
{
    return !(*this == bf);
}

TBitField TBitField::operator|(const TBitField &bf)
{
    int maxLen = (BitLen > bf.GetLength()) ? BitLen : bf.GetLength();
    TBitField res(maxLen);
    int minMem = (MemLen < bf.MemLen) ? MemLen : bf.MemLen;
    for (int i = 0; i < minMem; i++)
        res.pMem[i] = pMem[i] | bf.pMem[i];
    for (int i = minMem; i < MemLen; i++)
        res.pMem[i] = pMem[i];
    for (int i = minMem; i < bf.MemLen; i++)
        res.pMem[i] = bf.pMem[i];
    return res;
}

TBitField TBitField::operator&(const TBitField &bf)
{
    int maxLen = (BitLen > bf.GetLength()) ? BitLen : bf.GetLength();
    TBitField res(maxLen);
    int minMem = (MemLen < bf.MemLen) ? MemLen : bf.MemLen;
    for (int i = 0; i < minMem; i++)
        res.pMem[i] = pMem[i] & bf.pMem[i];
    return res;
}

TBitField TBitField::operator~(void)
{
    TBitField res(*this);
    for (int i = 0; i < MemLen; i++)
        res.pMem[i] = ~res.pMem[i];
    int last = BitLen % BITS_PER_ELEM;
    if (last != 0)
    {
        TELEM mask = ((TELEM)1 << last) - 1;
        res.pMem[MemLen - 1] &= mask;
    }
    return res;
}

istream& operator>>(istream &istr, TBitField &bf)
{
    for (int i = 0; i < bf.GetLength(); i++)
    {
        int bit;
        istr>> bit;
        if (bit) bf.SetBit(i);
        else bf.ClrBit(i);
    }
    return istr;
}

ostream& operator<<(ostream &ostr, const TBitField &bf)
{
    for (int i = 0; i < bf.GetLength(); i++)
        ostr << bf.GetBit(i);
    return ostr;
}