#pragma once

#include <iostream>
#include <vector>
#include <deque>
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
	BigInt(std::vector<int>, int);
	BigInt(std::deque<int>, int);
	BigInt(std::string);

	int get_sign() const;
	int get_number_of_digits() const;
	std::vector<int> get_digits() const;
};

std::ostream& operator<<(std::ostream&, const BigInt&);