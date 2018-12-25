#include "NumberTheory.h"
#include <map>
#include <algorithm>

BigInt gcd(const BigInt& aa, const BigInt& bb)
{
	BigInt a(aa), b(bb);
	while (a > 0 && b > 0)
	{
		if (a > b)
			a = a % b;
		else
			b = b % a;
	}

	return a > 0 ? a : b;
}

BigInt extended_gcd(const BigInt& a, const BigInt& b, BigInt& x, BigInt& y)
{
	if (a == 0)
	{
		x = 0;
		BigInt tmp = b;
		y = tmp;
		return b;
	}
	BigInt g = extended_gcd(b % a, a, x, y);

	BigInt t = x;
	x = y - b / a * x;
	y = t;

	return g;
}

BigInt inverse_modulo(const BigInt& a, const BigInt& m)
{
	// solve a * x + m * y = 1 with Euclidean algorithm
	BigInt x, y;
	extended_gcd(a, m, x, y);
	if (x < 0)
	{
		x = x % m;
		x = x + m;
	}
	x = x % m;
	return x;
}

BigInt CRTH(const std::vector<BigInt>& r, const std::vector<BigInt>& m)
{
	BigInt MOD = 1;
	for (int i = 0; i < m.size(); ++i)
		MOD = MOD * m[i];

	std::vector<BigInt> M(m.size());

	for (int i = 0; i < m.size(); ++i)
		M[i] = MOD / m[i];

	BigInt x;
	for (int i = 0; i < m.size(); ++i)
		x = x + M[i] * inverse_modulo(M[i], m[i]) * r[i];

	x = x % MOD;

	return x;
}

std::vector<BigInt> factorization_PollardRho(const BigInt& nn)
{
	if (nn < 100)
		return factorization(nn);

	std::vector<BigInt> res;
	BigInt d, n(nn);
	do
	{
		if (Miller_Rabin_test(n))
		{
			d = n;
			res.push_back(d);
		}
		else
		{

			d = get_divider_PollardRho(n);
			if (d != n && !Miller_Rabin_test(d))
			{
				auto d_fact = factorization_PollardRho(d);
				res.insert(res.end(), d_fact.begin(), d_fact.end());
			}
			else
				res.push_back(d);
		}
		n = n / d;
	} while (n != 1);

	return res;
}

std::vector<BigInt> factorization(const BigInt& nn)
{
	std::vector<BigInt> res;

	BigInt n(nn);

	for (int i = 2; i <= sqrt(n); ++i)
	{
		while (n % i == 0)
		{
			res.push_back(i);
			n = n / i;
		}
	}

	if (n != 1) res.push_back(n);
	return res;
}

BigInt get_divider_PollardRho(const BigInt& n)
{
	//if (n < 100) return get_divider(n);

	for (int i = 2; i < 100; ++i)
		if (n % i == 0)
			return i;

	srand(123);
	BigInt x = rand() % n;
	BigInt lambda = 10;
	BigInt bound = sqrt(2 * lambda * sqrt(n)) + 1;  // iterations to obtain (1 - e^lambda) possibility of success
	BigInt max_iter = (bound > 1e4 ? 1e4 : bound);

	for (int i = 1; (1 << i) < max_iter; ++i)
	{
		int cnt = (1 << i);
		BigInt nx = x;
		for (int j = 0; j < cnt; ++j)
		{
			nx = (nx * nx + 1) % n; // f(x) = x^2 + 1
			BigInt divider = gcd(abs(nx - x), n);
			if (divider != 1 && divider != n)
				return divider;
		}
		x = nx;
	}

	return n;
}

BigInt get_divider(const BigInt& n)
{
	for (int i = 2; i <= sqrt(n); ++i)
		if (n % i == 0)
			return i;

	return n;
}

bool Miller_Rabin_test(const BigInt& n, int iterations_count)
{
	if (n == 2) return true;

	BigInt d = n - 1;

	int s = 0;
	while (d % 2 == 0)
		d = d / 2, s++;

	// n - 1 = 2^s * d, d is odd

	for (int i = 0; i < iterations_count; ++i)
	{
		BigInt a = rand() % n;

		if (a == 0) continue;

		BigInt t = big_pow_modulo(a, d, n);
		if (t == 1)
			continue;

		BigInt q = a;
		bool flag = 0;
		for (int r = 0; r < s; ++r)
		{
			if (big_pow_modulo(q, d, n) == n - 1)
				flag = 1;

			q = q * q % n;
		}

		if (flag) continue;

		return false;
	}

	return true;
}

BigInt discrete_logarithm(const BigInt& a, const BigInt& b, const BigInt& m)
{
	if (gcd(a, m) != 1)
		throw "AAndMHaveCommonDividers";

	BigInt n = sqrt(m) + 1;
	std::map<BigInt, BigInt> vals;
	BigInt t = 0, np1 = n + 1;
	for (BigInt i = 1; i != np1; i = i + 1)
	{
		t = t + n;
		vals[big_pow_modulo(a, t, m)] = i;
	}
	for (int i = 0; i <= n; ++i) {
		BigInt cur = (big_pow_modulo(a, i, m) * b) % m;
		if (vals.count(cur)) {
			BigInt ans = vals[cur] * n - i;
			if (ans < m)
				return ans;
		}
	}
	return -1;
}

BigInt Euler_function(const BigInt& n)
{
	auto r = factorization_PollardRho(n);
	sort(r.begin(), r.end());

	std::vector<std::pair<BigInt, int>> nr;

	int cnt = 0;
	for (int i = 0; i < r.size(); ++i)
	{
		cnt++;
		if (i == r.size() - 1 || r[i] != r[i + 1])
		{
			nr.push_back(std::make_pair(r[i], cnt));
			cnt = 0;
		}
	}
	BigInt ans = 1;
	for (auto p : nr)
		ans = ans * big_pow(p.first, p.second - 1) * (p.first - 1);

	return ans;
}

BigInt Mobius_function(const BigInt& n)
{
	auto r = factorization_PollardRho(n);
	sort(r.begin(), r.end());

	std::vector<std::pair<BigInt, int>> nr;

	int cnt = 0;
	for (int i = 0; i < r.size(); ++i)
	{
		cnt++;
		if (i == r.size() - 1 || r[i] != r[i + 1])
		{
			nr.push_back(std::make_pair(r[i], cnt));
			cnt = 0;
		}
	}

	for (auto p : nr)
		if (p.second > 1) return 0;

	return ((nr.size() & 1) == 1 ? -1 : 1);
}

int Legendre_symbol(const BigInt& n, const BigInt& p)
{
	if (p == 2 || !Miller_Rabin_test(p))
		throw "PIsNotPrime";

	if (n % p == 0)
		return 0;

	return (big_pow_modulo(n % p, (p - 1) / 2, p) == 1 ? 1 : -1);
}

int Jacobi_symbol(const BigInt& n, const BigInt& p)
{
	if (p % 2 != 1)
		throw "PIsNotOdd";

	auto r = factorization_PollardRho(p);
	sort(r.begin(), r.end());

	std::vector<std::pair<BigInt, int>> nr;

	int cnt = 0;
	for (int i = 0; i < r.size(); ++i)
	{
		cnt++;
		if (i == r.size() - 1 || r[i] != r[i + 1])
		{
			nr.push_back(std::make_pair(r[i], cnt));
			cnt = 0;
		}
	}

	int ans = 1;

	for (auto t : nr)
	{
		if (t.second & 1)
			ans *= Legendre_symbol(n, t.first);
	}

	return ans;
}

BigInt discrete_sqrt(const BigInt& b, const BigInt& m)
{
	BigInt a = rand() % m;
	while (Legendre_symbol((a * a - b + m) % m, m) != -1)
		a = rand() % m;

	BigInt t = (a * a - b + m) % m;

	BigInt x; // x = (a + sqrt(a * a - n))^((m + 1) / 2)

	BigInt d = (m + 1) / 2;

	std::pair<BigInt, BigInt> p(1, 0);
	std::pair<BigInt, BigInt> k(a, 1);

	while (d != 0)
	{
		if (d % 2 == 1)
			p = std::make_pair((k.first * p.first + t * k.second * p.second) % m, (k.first * p.second + k.second * p.first) % m);

		d = d / 2;

		k = std::make_pair((k.first * k.first + t * k.second * k.second) % m, 2 * k.first * k.second % m);
	}

	if (p.second != 0)
		throw "Error";

	return p.first;
}