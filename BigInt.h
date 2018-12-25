#pragma once

#include <iostream>
#include <vector>
#include <string>

class BigInt
{
	std::vector<int> digits;
	int number_of_digits;
	int sign;

public:
	static const int base = 10;

	BigInt();
	BigInt(long long);
	int get_sign() const;
	std::vector<int> get_digits() const;
	BigInt(std::vector<int>, int);
	BigInt(std::string);
};

std::ostream& operator<<(std::ostream&, const BigInt&);