#pragma once

template<typename T = int, typename U = long double>
struct poly {
	vector<T> coef;
	poly() {}
	static inline int next_pow_2(int n) { return 1 << (32 - __builtin_clz(n - 1)); }
	template<typename V>
	explicit poly(const vector<V> &real) {
		coef.resize(real.size());
		for (int i = 0; i < real.size(); ++i) { coef[i] = real[i]; }
	}
	explicit poly(int n) { coef.resize(n, 0); }
	inline size_t size() const { return coef.size(); }
	inline T &operator[](int idx) { return coef[idx]; }
	inline const T &operator[](int idx) const { return coef[idx]; }

	static void fft(vector<complex<U>> &a) {
		int n = a.size();
		static int last_n;
		static vector<int> rev;
		static vector<complex<U>> rt(2, 1);
		if (n != last_n) {
			rev.resize(n);
			int L = 31 - __builtin_clz(n);
			for (int i = 0; i < n; ++i) { rev[i] = (rev[i / 2] | (i & 1) << L) / 2; }
			last_n = n;
			int prev = rt.size();
			rt.resize(n);
			for (int k = prev; k < n; k *= 2) {
				auto x = polar(1.0L, acos(-1.0L) / k);
				for (int i = k; i < 2 * k; ++i) {
					rt[i] = i & 1 ? rt[i / 2] * x : rt[i / 2];
				}
			}
		}

		for (int i = 1; i < n; i++) { if (i < rev[i]) swap(a[i], a[rev[i]]); }
		complex<U> *A = a.data();
		for (int len = 1; len < n; len *= 2) {
			for (int i = 0; i < n; i += 2 * len) {
				for (int j = 0; j < len; ++j) {
					complex<U> v = A[i + j + len] * rt[len + j];
					A[i + j + len] = A[i + j] - v;
					A[i + j] += v;
				}
			}
		}
	}
	friend poly operator*(const poly &a, const poly &b) {
		int n = next_pow_2(a.size() + b.size() - 1);
		vector<complex<U>> in(n), out(n);
		copy(a.coef.begin(), a.coef.end(), in.begin());
		for (int i = 0; i < b.size(); ++i) { in[i].imag(b[i]); }
		fft(in);
		for (int i = 0; i < n; ++i) { in[i] *= in[i]; }
		for (int i = 0; i < n; ++i) { out[i] = in[-i & (n - 1)] - conj(in[i]); }
		fft(out);
		poly res(a.size() + b.size() - 1);
		for (int i = 0; i < res.size(); ++i) { res.coef[i] = round(out[i].imag() / (4 * n)); }
		return res;
	}
};