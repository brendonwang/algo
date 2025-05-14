
template<int mod = 998244353, int root = 3>
struct poly {
	vector<int> coef;

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
		for (int i = real.size(); i < n; ++i) coef[i] = 0;
	}

	explicit poly(int m) {
		int n = next_pow_2(m);
		coef.resize(n, 0);
	}

	size_t size() const {
		return coef.size();
	}

	int &operator[](int idx) {
		return coef[idx];
	}

	const int &operator[](int idx) const {
		return coef[idx];
	}

	static size_t reverseBits(size_t num, int log2n) {
		size_t reversed = 0;
		for (int i = 0; i < log2n; ++i)
			if (num & (1 << i)) reversed |= (1 << (log2n - 1 - i));
		return reversed;
	}

	friend void ntt(poly &a, bool invert = false) {
		int n = a.size(), log2n = log2(n);
		vector<int> rev(n);
		for (int i = 0; i < n; ++i)
			rev[i] = reverseBits(i, log2n);

		for (int i = 0; i < n; ++i)
			if (i < rev[i]) swap(a[i], a[rev[i]]);

		for (int len = 2; len <= n; len *= 2) {
			int wlen = pow_mod(root, (mod - 1) / len);
			if (invert) wlen = pow_mod(wlen, mod - 2);
			for (int i = 0; i < n; i += len) {
				int w = 1;
				for (int j = 0; j < len / 2; ++j) {
					int u = a[i + j], v = (int) ((1LL * a[i + j + len / 2] * w) % mod);
					a[i + j] = u + v < mod ? u + v : u + v - mod;
					a[i + j + len / 2] = u - v >= 0 ? u - v : u - v + mod;
					w = (int) ((1LL * w * wlen) % mod);
				}
			}
		}

		if (invert) {
			int n_inv = pow_mod(n, mod - 2);
			for (int &x: a.coef) x = (int) ((1LL * x * n_inv) % mod);
		}
	}

	friend poly operator*(poly a, poly b) {
		int n = next_pow_2(a.size() + b.size() - 1);
		a.coef.resize(n);
		b.coef.resize(n);
		ntt(a, false);
		ntt(b, false);
		for (int i = 0; i < n; ++i) a[i] = (int) ((1LL * a[i] * b[i]) % mod);
		ntt(a, true);
		a.coef.resize(a.size() + b.size() - 1);
		return a;
	}

	static int pow_mod(int base, int exp) {
		int result = 1;
		while (exp > 0) {
			if (exp % 2 == 1) result = (int) ((1LL * result * base) % mod);
			base = (int) ((1LL * base * base) % mod);
			exp /= 2;
		}
		return result;
	}
};