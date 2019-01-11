#include <algorithm>
#include <utility>
#include <map>
#include "BigMath.h"

bool operator>(const BigInt &a, const BigInt &b) {
    if (a.get_sign() * b.get_sign() == -1)
        return a.get_sign() == 1;

    if (a.get_sign() * b.get_sign() == 1) {
        int reverse = 0;
        if (a.get_sign() == -1) // a < 0 && b < 0
            reverse = 1;

        auto a_digits = a.get_digits(), b_digits = b.get_digits();
        if (a_digits.size() != b_digits.size())
            return (a_digits.size() > b_digits.size()) ^ reverse;

        for (int i = a_digits.size() - 1; i >= 0; --i)
            if (a_digits[i] != b_digits[i])
                return (a_digits[i] > b_digits[i]) ^ reverse;

        return 0;
    }

    return a.get_sign() == 1 || b.get_sign() == -1;
}

bool operator<(const BigInt &a, const BigInt &b) {
    return b > a;
}

bool operator==(const BigInt &a, const BigInt &b) {
    return (a.get_sign() == b.get_sign() && a.get_digits() == b.get_digits());
}

bool operator!=(const BigInt &a, const BigInt &b) {
    return !(a == b);
}

bool operator>=(const BigInt &a, const BigInt &b) {
    return a > b || a == b;
}

bool operator<=(const BigInt &a, const BigInt &b) {
    return a < b || a == b;
}

BigInt operator-(const BigInt &a) {
    return BigInt(a.get_digits(), -1 * a.get_sign());
}

BigInt operator+(const BigInt &a) {
    return a;
}

BigInt abs(const BigInt &a) {
    return BigInt(a.get_digits(), 1);
}

BigInt add_two_positive_numbers(const BigInt &a, const BigInt &b) {
    if (a < 0 || b < 0)
        throw "Numbers are not positive";

    std::vector<int> a_digits(a.get_digits()), b_digits(b.get_digits());
    int sum_number_of_digits = std::max(a_digits.size(), b_digits.size()) + 1;
    std::vector<int> sum_digits(sum_number_of_digits);

    for (int i = 0; i < sum_number_of_digits - 1; ++i) {
        if (i < a_digits.size())
            sum_digits[i] += a_digits[i];
        if (i < b_digits.size())
            sum_digits[i] += b_digits[i];

        sum_digits[i + 1] = sum_digits[i] / BigInt::base;
        sum_digits[i] %= BigInt::base;
    }

    if (sum_number_of_digits == a_digits.size())
        sum_digits.back() += a_digits.back();
    if (sum_number_of_digits == b_digits.size())
        sum_digits.back() += b_digits.back();

    if (sum_digits.back() >= BigInt::base) {
        sum_digits.back() %= BigInt::base;
        sum_digits.push_back(1);
    }

    return BigInt(sum_digits, 1);
}

BigInt subtract_two_positive_numbers(const BigInt &a, const BigInt &b) {
    if (a < 0 || b < 0)
        throw "Numbers are not positive";

    std::vector<int> a_digits(a.get_digits()), b_digits(b.get_digits());
    std::vector<int> sum_digits;
    int sign = 1;
    if (a < b) {
        std::swap(a_digits, b_digits);
        sign = -1;
    }

    for (int i = 0; i < b_digits.size(); ++i) {
        if (a_digits[i] < b_digits[i]) {
            a_digits[i] += BigInt::base;
            a_digits[i + 1] -= 1;
        }

        sum_digits.push_back(a_digits[i] - b_digits[i]);
    }

    return BigInt(sum_digits, sign);
}

BigInt operator+(const BigInt &a, const BigInt &b) {
    if (a > 0) {
        if (b > 0)
            return add_two_positive_numbers(a, b);
        else
            return subtract_two_positive_numbers(a, abs(b));
    } else {
        if (b > 0)
            return subtract_two_positive_numbers(b, abs(a));
        else
            return -add_two_positive_numbers(abs(a), abs(b));
    }
}

BigInt operator*(const BigInt &a, const BigInt &b) {
    int prod_sign = a.get_sign() * b.get_sign();
    std::vector<int> a_digits(a.get_digits()), b_digits(b.get_digits());
    int prod_number_of_digits = a_digits.size() + b_digits.size();
    std::vector<int> prod_digits(prod_number_of_digits);

    for (int i = 0; i < a_digits.size(); ++i)
        for (int j = 0; j < b_digits.size(); ++j)
            prod_digits[i + j] += a_digits[i] * b_digits[j];

    for (int i = 0; i < prod_number_of_digits - 1; ++i) {
        prod_digits[i + 1] += prod_digits[i] / BigInt::base;
        prod_digits[i] %= BigInt::base;
    }

    return BigInt(prod_digits, prod_sign);
}

BigInt operator-(const BigInt &a, const BigInt &b) {
    return a + -1 * b;
}

BigInt operator/(const BigInt &a, const BigInt &b) {
    if (b == 0) throw "DividedByZero";

    std::vector<int> a_digits = a.get_digits();
    BigInt dividend = 0, divider = abs(b), result = 0;
    int p = a_digits.size() - 1;
    while (p >= 0) {
        if (dividend < divider) {
            dividend = dividend * BigInt::base;
            dividend = dividend + a_digits[p];
            p--;
        }

        while (dividend < divider && p >= 0) {
            result = result * BigInt::base;
            dividend = dividend * BigInt::base;
            dividend = dividend + a_digits[p];
            p--;
        }

        if (dividend < divider && p < 0)
            result = result * 10;

        if (dividend >= divider) {
            BigInt n_divider = divider;
            int k = 1;
            while (n_divider + divider <= dividend)
                n_divider = n_divider + divider, k++;

            dividend = dividend - n_divider;

            result = result * 10;
            result = result + k;
        }
    }

    return result * a.get_sign() * b.get_sign();
}

BigInt operator%(const BigInt &a, const BigInt &b) {
    if (b == 0) throw "DividedByZero";

    std::vector<int> a_digits = a.get_digits();
    BigInt dividend = 0, divider = abs(b);
    int p = a_digits.size() - 1;
    while (p >= 0) {
        while (dividend < divider && p >= 0) {
            dividend = dividend * BigInt::base;
            dividend = dividend + a_digits[p];
            p--;
        }

        if (dividend >= divider) {
            BigInt n_divider = divider;
            int k = 1;
            while (n_divider + divider <= dividend)
                n_divider = n_divider + divider, k++;

            dividend = dividend - n_divider;
        }
    }

    dividend = dividend * a.get_sign();
    if (dividend < 0) dividend = dividend + divider;

    return dividend;
}

std::pair<BigInt, BigInt> div(const BigInt &a, const BigInt &b) {
    if (b == 0) throw "DividedByZero";

    std::vector<int> a_digits = a.get_digits();
    BigInt dividend = 0, divider = abs(b), result = 0;
    int p = a_digits.size() - 1;
    while (p >= 0) {
        if (dividend < divider) {
            dividend = dividend * 10;
            dividend = dividend + a_digits[p];
            p--;
        }

        while (dividend < divider && p >= 0) {
            result = result * 10;
            dividend = dividend * 10;
            dividend = dividend + a_digits[p];
            p--;
        }

        if (dividend < divider && p < 0)
            result = result * 10;

        if (dividend >= divider) {
            BigInt n_divider = divider;
            int k = 1;
            while (n_divider + divider < dividend)
                n_divider = n_divider + divider, k++;

            dividend = dividend - n_divider;

            result = result * 10;
            result = result + k;
        }
    }

    return std::make_pair(result * a.get_sign() * b.get_sign(), dividend);
}

BigInt sqrt(const BigInt &a) {
    if (a < 0) throw "Root of negative number";
    BigInt l = 0, r = a + 1;
    while (r - l > 1) {
        BigInt m = (r + l) / 2;
        if (m * m > a)
            r = m;
        else
            l = m;
    }

    return l;
}

BigInt add_modulo(const BigInt &a, const BigInt &b, const BigInt &mod) {
    return (a + b) % mod;
}

BigInt subtract_modulo(const BigInt &a, const BigInt &b, const BigInt &mod) {
    return (a - b) % mod;
}

BigInt multiply_modulo(const BigInt &a, const BigInt &b, const BigInt &mod) {
    return (a * b) % mod;
}

BigInt divide_modulo(const BigInt &a, const BigInt &b, const BigInt &mod) {
    return (a / b) % mod;
}

BigInt big_pow(const BigInt &a, const BigInt &p) {
    if (p == 0) return 1;

    std::pair<BigInt, BigInt> d = std::make_pair(p / 2, p % 2); //div(p, 2);
    BigInt b = big_pow(a, d.first);
    b = b * b;
    if (d.second == 1)
        b = b * a;
    return b;
}

BigInt big_pow_modulo(const BigInt &a, const BigInt &p, const BigInt &m) {
    if (p == 0) return 1;

    std::pair<BigInt, BigInt> d = std::make_pair(p / 2, p % 2); //div(p, 2);
    BigInt b = big_pow_modulo(a, d.first, m);
    b = b * b;
    if (d.second == 1)
        b = b * a;
    return b % m;
}
