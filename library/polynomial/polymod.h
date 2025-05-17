#pragma once

template<int mod = 998244353, int root = 3>
struct poly {
	std::vector<int> coef;

	poly() {}

	static int next_pow_2(int n) {
		int p = 1;
		while (p < n) p <<= 1;
		return p;
	}

	explicit poly(int m) {
		int n = next_pow_2(m);
		coef.assign(n, 0);
	}

	template<typename T>
	explicit poly(const std::vector<T> &real, int m = -1) {
		if (m == -1) m = (int) real.size();
		int n = next_pow_2(m);
		coef.resize(n);
		for (int i = 0, sz = (int) real.size(); i < sz; ++i)
			coef[i] = real[i];
		for (int i = (int) real.size(); i < n; ++i)
			coef[i] = 0;
	}

	size_t size() const { return coef.size(); }

	int &operator[](int idx) { return coef[idx]; }
	const int &operator[](int idx) const { return coef[idx]; }

	static size_t reverseBits(size_t num, int log2n) {
		size_t rev = 0;
		for (int i = 0; i < log2n; ++i)
			if (num & (size_t(1) << i))
				rev |= (size_t(1) << (log2n - 1 - i));
		return rev;
	}

	friend void ntt(poly &a, bool invert = false) {
		int n = (int) a.size(), log2n = __builtin_ctz(n);
		std::vector<int> rev(n);
		for (int i = 0; i < n; ++i)
			rev[i] = (int) reverseBits(i, log2n);
		for (int i = 0; i < n; ++i)
			if (i < rev[i]) std::swap(a[i], a[rev[i]]);

		for (int len = 2; len <= n; len <<= 1) {
			int wlen = pow_mod(root, (mod - 1) / len);
			if (invert) wlen = pow_mod(wlen, mod - 2);
			for (int i = 0; i < n; i += len) {
				int w = 1;
				for (int j = 0; j < len / 2; ++j) {
					int u = a[i + j];
					int v = int(1LL * a[i + j + len / 2] * w % mod);
					a[i + j] = u + v < mod ? u + v : u + v - mod;
					a[i + j + len / 2] = u - v >= 0 ? u - v : u - v + mod;
					w = int(1LL * w * wlen % mod);
				}
			}
		}

		if (invert) {
			int inv_n = pow_mod((int) n, mod - 2);
			for (auto &x: a.coef)
				x = int(1LL * x * inv_n % mod);
		}
	}

	friend poly operator*(poly a, poly b) {
		int n = next_pow_2((int) a.size() + (int) b.size() - 1);
		a.coef.resize(n);
		b.coef.resize(n);
		ntt(a, false);
		ntt(b, false);
		for (int i = 0; i < n; ++i)
			a[i] = int(1LL * a[i] * b[i] % mod);
		ntt(a, true);
		a.coef.resize((int) a.size() + (int) b.size() - 1);
		return a;
	}

	static int pow_mod(int base, int exp) {
		int result = 1;
		while (exp > 0) {
			if (exp & 1) result = int(1LL * result * base % mod);
			base = int(1LL * base * base % mod);
			exp >>= 1;
		}
		return result;
	}


	static void fwt_or(poly& a, bool invert) {
		int n = (int)a.size();
		assert((n & (n - 1)) == 0);
		for (int len = 1; len < n; len <<= 1) {
			for (int i = 0; i < n; i += len << 1) {
				for (int j = 0; j < len; ++j) {
					if (!invert)      a[i + j + len] += a[i + j];
					else              a[i + j + len] -= a[i + j];
					if (a[i + j + len] >= mod) a[i + j + len] -= mod;
					if (a[i + j + len] < 0) a[i + j + len] += mod;
				}
			}
		}
	}

	static void fwt_and(poly& a, bool invert) {
		int n = (int)a.size();
		assert((n & (n - 1)) == 0);
		for (int len = 1; len < n; len <<= 1) {
			for (int i = 0; i < n; i += len << 1) {
				for (int j = 0; j < len; ++j) {
					if (!invert)      a[i + j] += a[i + j + len];
					else              a[i + j] -= a[i + j + len];
					if (a[i + j] >= mod) a[i + j] -= mod;
					if (a[i + j] < 0) a[i + j] += mod;
				}
			}
		}
	}

	static void fwt_xor(poly& a, bool invert) {
		int n = (int)a.size();
		assert((n & (n - 1)) == 0);
		for (int len = 1; len < n; len <<= 1) {
			for (int i = 0; i < n; i += len << 1) {
				for (int j = 0; j < len; ++j) {
					int u = a[i + j], v = a[i + j + len];
					a[i + j] = u + v;
					a[i + j + len] = u - v;
					if (a[i + j] >= mod) a[i + j] -= mod;
					if (a[i + j] < 0) a[i + j] += mod;
					if (a[i + j + len] >= mod) a[i + j + len] -= mod;
					if (a[i + j + len] < 0) a[i + j + len] += mod;
				}
			}
		}
		if (invert) {
			int inv_n = pow_mod(n, mod - 2);
			for (auto& x : a.coef)
				x = int(1LL * x * inv_n % mod);
		}
	}


	template<typename F>
	static poly fwt_conv(const poly &x, const poly &y, F f, F invf) {
		poly a = x;
		poly b = y;
		int need = std::max(a.size(), b.size());
		int n = next_pow_2(need);
		a.coef.resize(n);
		b.coef.resize(n);
		f(a, false);
		f(b, false);
		for (int i = 0; i < n; ++i)
			a.coef[i] = int(1LL * a.coef[i] * b.coef[i] % mod);
		invf(a, true);
		return a;
	}

	friend poly convolution_or(const poly &a, const poly &b) {
		return fwt_conv(a, b, fwt_or, fwt_or);
	}

	friend poly convolution_and(const poly &a, const poly &b) {
		return fwt_conv(a, b, fwt_and, fwt_and);
	}

	friend poly convolution_xor(const poly &a, const poly &b) {
		return fwt_conv(a, b, fwt_xor, fwt_xor);
	}
};
