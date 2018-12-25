#pragma once

#include "BigMath.h"

BigInt gcd(const BigInt&, const BigInt&);
BigInt extended_gcd(const BigInt& a, const BigInt& b, BigInt& x, BigInt& y); //find x and y, so that a * x + b * y = gcd(a, b)
BigInt inverse_modulo(const BigInt& a, const BigInt& mod);
BigInt CRTH(const std::vector<BigInt>& a, const std::vector<BigInt>& m); //using Chinese remainder theorem for solving x = a[i] (mod m[i])
std::vector<BigInt> factorization_PollardRho(const BigInt&);
std::vector<BigInt> factorization(const BigInt&);
BigInt get_divider_PollardRho(const BigInt& n); // returns n if not trivial divider wasn't found
BigInt get_divider(const BigInt& n); // returns n if not trivial divider wasn't found
bool Miller_Rabin_test(const BigInt& n, int iterations_count = 3);
BigInt discrete_logarithm(const BigInt& a, const BigInt& b, const BigInt& m); // a^x = b (mod m) baby-step-giant-step-algorithm
BigInt Euler_function(const BigInt&);
BigInt Mobius_function(const BigInt&);
int Legendre_symbol(const BigInt& n, const BigInt& p);
int Jacobi_symbol(const BigInt& n, const BigInt& p);
BigInt discrete_sqrt(const BigInt& b, const BigInt& m); // x^2 = b (mod m)