#pragma once

struct poly {
	vector<complex<long double>> coef;

	poly() {}

	static int next_pow_2(int n) {
		int p = 1;
		while (p < n) p <<= 1;
		return p;
	}

	template<typename T>
	explicit poly(const vector<T> &real, int m = -1) {
		if (m == -1) m = real.size();
		int n = next_pow_2(m);
		coef.resize(n);
		for (int i = 0; i < real.size(); ++i) coef[i] = real[i];
		for (int i = real.size(); i < n; ++i) coef[i] = complex<long double>(0);
	}

	explicit poly(int m) {
		int n = next_pow_2(m);
		coef.resize(n, complex<long double>(0));
	}

	size_t size() const {
		return coef.size();
	}

	complex<long double> &operator[](int idx) {
		return coef[idx];
	}


	const complex<long double> &operator[](int idx) const {
		return coef[idx];
	}

	static size_t reverseBits(size_t num, int log2n) {
		size_t reversed = 0;
		for (int i = 0; i < log2n; ++i)
			if (num & (1 << i)) reversed |= (1 << (log2n - 1 - i));
		return reversed;
	}

	friend void fft(poly &a, bool invert = false) {
		int n = a.size();
		int log2n = log2(n);
		for (int i = 0; i < n; ++i) {
			int j = reverseBits(i, log2n);
			if (i < j) swap(a[i], a[j]);
		}
		for (int len = 2; len <= n; len *= 2) {
			long double angle = 2 * acos(-1) / len * (invert ? -1 : 1);
			complex<long double> wlen(cos(angle), sin(angle));
			for (int i = 0; i < n; i += len) {
				complex<long double> w(1);
				for (int j = 0; j < len / 2; ++j) {
					complex<long double> u = a[i + j];
					complex<long double> v = a[i + j + len / 2] * w;
					a[i + j] = u + v;
					a[i + j + len / 2] = u - v;
					w *= wlen;
				}
			}
		}
		if (invert) {
			for (auto &x: a.coef) x /= n;
		}
	}

	friend poly operator*(poly a, poly b) {
		int n = next_pow_2(a.size() + b.size() - 1);
		a.coef.resize(n);
		b.coef.resize(n);
		fft(a, false);
		fft(b, false);
		for (int i = 0; i < n; ++i) a[i] *= b[i];
		fft(a, true);
		a.coef.resize(a.size() + b.size() - 1);
		for (auto &x: a.coef) x = complex<long double>(round(x.real()), 0);
		return a;
	}
};