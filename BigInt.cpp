#include <math.h>
#include "BigInt.h"

BigInt::BigInt() {
    sign = 0;
    number_of_digits = 1;
    digits = std::vector<int>(number_of_digits, 0);
}

BigInt::BigInt(long long n) {
    number_of_digits = 0;

    if (n != 0) {
        digits = std::vector<int>(0);
        digits.reserve(100);

        long long abs_n = std::abs(n);
        sign = n / abs_n;

        while (abs_n > 0) {
            number_of_digits++;
            digits.push_back(abs_n % BigInt::base);
            abs_n /= BigInt::base;
        }
    } else *this = BigInt();
}

BigInt::BigInt(std::vector<int> n_digits, int n_sign) : digits(n_digits), sign(n_sign) {
    if (digits.size() == 0) digits.push_back(0);

    while (digits.back() == 0 && digits.size() > 1)
        digits.pop_back();
    number_of_digits = digits.size();

    for (int i = 0; i < number_of_digits; ++i)
        if (digits[i] < 0 || digits[i] > base - 1)
            throw "Unsuitable format error";

    if (digits.size() == 1 && digits[0] == 0)
        sign = 0;
}

BigInt::BigInt(std::deque<int> n_digits, int n_sign) : sign(n_sign) {
    for (auto t : n_digits)
        digits.push_back(t);

    if (digits.size() == 0) digits.push_back(0);

    while (digits.back() == 0 && digits.size() > 1)
        digits.pop_back();
    number_of_digits = digits.size();

    for (int i = 0; i < number_of_digits; ++i)
        if (digits[i] < 0 || digits[i] > base - 1)
            throw "Unsuitable format error";

    if (digits.size() == 1 && digits[0] == 0)
        sign = 0;
}

BigInt::BigInt(std::string s) {
    if (s[0] != '-' && !(s[0] >= '0' && s[0] <= '9'))
        throw "Unsuitable format error";

    for (int i = 1; i < s.length(); ++i)
        if (!(s[i] >= '0' && s[i] <= '9'))
            throw "Unsuitable format error";

    if (s[0] == '-') {
        sign = -1;
        s[0] = '0';
    } else sign = 1;

    for (int i = s.length() - 1; i >= 0; --i)
        digits.push_back(s[i] - '0');

    BigInt a(digits, sign);

    digits = a.get_digits();
}

int BigInt::get_sign() const {
    return sign;
}

int BigInt::get_number_of_digits() const {
    return number_of_digits;
}

std::vector<int> BigInt::get_digits() const {
    return digits;
}

std::ostream &operator<<(std::ostream &os, const BigInt &a) {
    if (a.get_sign() == -1)
        os << '-';
    auto digits = a.get_digits();
    for (auto it = digits.rbegin(); it != digits.rend(); ++it)
        os << *it;

    return os;
}