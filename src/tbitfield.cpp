// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле
#include <vector>
#include "tbitfield.h"

TBitField::TBitField(int len) :BitLen(len), MemLen((len +7) >>3)
{
	if (len < 0)
		throw("Len uncorrect");
	else
		pMem = new TELEM[MemLen]{ 0 };
}

TBitField::TBitField(const TBitField& bf) :BitLen(bf.BitLen), MemLen(bf.MemLen)   // конструктор копирования
{
	pMem = new TELEM[MemLen];
	for (int i = 0; i < MemLen; i++)
		pMem[i] = bf.pMem[i];
}

TBitField::~TBitField()
{
	delete[]  pMem;
	pMem = nullptr;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
	if (n < 0 || n > BitLen)
		throw("index uncorrect");
	else
		return n >> 3;
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
	if (n < 0 || n > BitLen)
		throw(" index uncorrect");
	else
		return 1 << (n&7);
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
	return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
		pMem[GetMemIndex(n)] |= GetMemMask(n);
}

void TBitField::ClrBit(const int n) // очистить бит
{
		pMem[GetMemIndex(n)] &= ~GetMemMask(n);
}

int TBitField::GetBit(const int n) const // получить значение бита
{
		return pMem[GetMemIndex(n)] & GetMemMask(n);
}

// битовые операции

TBitField& TBitField::operator=(const TBitField& bf) // присваивание
{
	if (this != &bf)
	{
		delete[] pMem;
		BitLen = bf.BitLen;
		MemLen = bf.MemLen;
		pMem = new TELEM[MemLen];
		for (int i = 0; i < MemLen; i++)
			pMem[i] = bf.pMem[i];
	}
	return *this;
}

int TBitField::operator==(const TBitField& bf) const // сравнение
{
	if (BitLen != bf.BitLen) return 0;
	else
		for (int i = 0; i < MemLen; i++)
			if (pMem[i] != bf.pMem[i])
				return 0;
	return 1;
}
int TBitField::operator!=(const TBitField& bf) const // сравнение
{
	if (BitLen != bf.BitLen) return 1;
	else
		for (int i = 0; i < MemLen; i++)
			if (pMem[i] == bf.pMem[i])
				return 0;
	return 1;
}

TBitField TBitField::operator|(const TBitField& bf) // операция "или"
{
	int len;
	(bf.BitLen <= BitLen) ? (len = BitLen) : (len = bf.BitLen);
	TBitField temp_var(len);
	for (int i = 0; i < MemLen; i++)
		temp_var.pMem[i] = pMem[i];
	for (int i = 0; i < bf.MemLen;i++)
		temp_var.pMem[i] |= bf.pMem[i];
	return temp_var;
}

TBitField TBitField::operator&(const TBitField& bf) // операция "и"
{
	int len;
	(bf.BitLen <= BitLen) ? (len = BitLen) : (len = bf.BitLen);
	TBitField temp_var(len);
	for (int i = 0; i < MemLen; i++)
		temp_var.pMem[i] = pMem[i];
	for (int i = 0; i < bf.MemLen; i++)
		temp_var.pMem[i] &= bf.pMem[i];
	return temp_var;
}

TBitField TBitField::operator~(void) // отрицание
{
	TBitField temp_var(BitLen);
	for (int i = 0; i < temp_var.MemLen - 1; i++)
		temp_var.pMem[i] = ~pMem[i];
	for (int i = (temp_var.MemLen - 1) * sizeof(TELEM) * 8; i < temp_var.BitLen; i++)
	{
		if (GetBit(i))
			temp_var.ClrBit(i);
		else
			temp_var.SetBit(i);
	}
	return temp_var;
}

// ввод/вывод

istream& operator>>(istream& istr, TBitField& bf) // ввод
{
	string temp_var;
	istr >> temp_var;
	if (temp_var.size() > bf.GetLength())
		throw "uncorrect length";
	for (int i = 0; i < bf.BitLen; i++)
	{
		if (temp_var[i] == '0')
			bf.ClrBit(i);
		else if (temp_var[i] == '1')
			bf.SetBit(i);
		else
			throw "uncorrect input";
	}
	return istr;
}


ostream& operator<<(ostream& ostr, const TBitField& bf) // вывод
{
	for (int i = 0; i < bf.BitLen; i++)
	{
		if (bf.GetBit(i) )
			ostr << i << " ";
	}
	return ostr;
}