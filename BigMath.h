#pragma once

#include "BigInt.h"

bool operator> (const BigInt&, const BigInt&);
bool operator< (const BigInt&, const BigInt&);
bool operator== (const BigInt&, const BigInt&);
bool operator!= (const BigInt&, const BigInt&);
bool operator>= (const BigInt&, const BigInt&);
bool operator<= (const BigInt&, const BigInt&);
BigInt operator+ (const BigInt&, const BigInt&);
BigInt operator* (const BigInt&, const BigInt&);
BigInt operator- (const BigInt&, const BigInt&);
BigInt operator/ (const BigInt&, const BigInt&);
BigInt operator% (const BigInt&, const BigInt&);

BigInt operator- (const BigInt&);
BigInt operator+ (const BigInt&);

BigInt abs(const BigInt&);
std::pair<BigInt, BigInt> div(const BigInt&, const BigInt&); //returns result of division and reminder
BigInt sqrt(const BigInt&);
BigInt add_modulo(const BigInt& a, const BigInt& b, const BigInt& mod);
BigInt subtract_modulo(const BigInt& a, const BigInt& b, const BigInt& mod);
BigInt multiply_modulo(const BigInt& a, const BigInt& b, const BigInt& mod);
BigInt divide_modulo(const BigInt& a, const BigInt& b, const BigInt& mod);
BigInt big_pow(const BigInt& a, const BigInt& p);
BigInt big_pow_modulo(const BigInt&, const BigInt&, const BigInt&);