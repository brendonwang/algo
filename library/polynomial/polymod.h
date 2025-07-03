#pragma once

template<int mod = 998244353, int root = 3, int NMAX = 25>
struct poly {
	vector<int> coef;
	poly() {}
	static inline int next_pow_2(int n) { return 1 << (32 - __builtin_clz(n - 1)); }
	template<typename V>
	explicit poly(const vector<V> &real) {
		coef.resize(real.size());
		for (int i = 0; i < real.size(); ++i) { coef[i] = real[i]; }
	}
	explicit poly(int n) { coef.resize(n, 0); }
	inline size_t size() const { return coef.size(); }
	inline int &operator[](int idx) { return coef[idx]; }
	inline const int &operator[](int idx) const { return coef[idx]; }
	static inline int pow_mod(ll base, ll exp) {
		int result = 1;
		while (exp > 0) {
			if (exp & 1) result = int(1LL * result * base % mod);
			base = int(1LL * base * base % mod);
			exp >>= 1;
		}
		return result;
	}
	static void ntt(vector<int> &a) {
		int n = a.size();
		static int last_n;
		static vector<int> rev;
		static vector<ll> rt(2, 1);
		if (n != last_n) {
			rev.resize(n);
			int L = 31 - __builtin_clz(n);
			for (int i = 0; i < n; ++i) { rev[i] = (rev[i / 2] | (i & 1) << L) / 2; }
			last_n = n;
			if (rt.size() < n) {
				int prev = rt.size();
				rt.resize(n);
				for (int k = prev, s = __builtin_ctz(prev) + 1; k < n; k *= 2, s++) {
					auto x = pow_mod(root, mod >> s);
					for (int i = k; i < 2 * k; ++i) {
						rt[i] = i & 1 ? rt[i / 2] * x % mod : rt[i / 2];
					}
				}
			}
		}

		for (int i = 1; i < n; i++) { if (i < rev[i]) swap(a[i], a[rev[i]]); }
		int *A = a.data();
		for (int len = 1; len < n; len *= 2) {
			for (int i = 0; i < n; i += 2 * len) {
				for (int j = 0; j < len; ++j) {
					int u = A[i + j], v = A[i + j + len] * rt[len + j] % mod;
					A[i + j] = u + v - (u + v >= mod ? mod : 0);
					A[i + j + len] = u - v + (u < v ? mod : 0);
				}
			}
		}
	}
	friend poly operator*(const poly &a, const poly &b) {
		int s = (int) a.size() + (int) b.size() - 1, n = next_pow_2(s);
		int inv = pow_mod(n, mod - 2);
		vector<int> L(a.coef), R(b.coef), out(n);
		L.resize(n), R.resize(n);
		ntt(L), ntt(R);
		for (int i = 0; i < n; ++i) {
			out[-i & (n - 1)] = (ll) L[i] * R[i] % mod * inv % mod;
		}
		ntt(out);
		poly res(s);
		copy_n(out.begin(), s, res.coef.begin());
		return res;
	}
	poly operator*=(const poly &b) {
		int s = (int) size() + (int) b.size() - 1, n = next_pow_2(s);
		int inv = pow_mod(n, mod - 2);
		vector<int> L(coef), R(b.coef), out(n);
		L.resize(n), R.resize(n);
		ntt(L), ntt(R);
		for (int i = 0; i < n; ++i) {
			out[-i & (n - 1)] = (ll) L[i] * R[i] % mod * inv % mod;
		}
		ntt(out);
		coef.resize(s);
		for (int i = 0; i < s; ++i) {
			coef[i] = out[i];
		}
		return *this;
	}

	poly &operator*=(ll k) {
		k %= mod;
		if (k < 0) k += mod;
		for (int &x: coef) x = 1ll * x * k % mod;
		return *this;
	}
	friend poly operator*(poly p, ll k) { return p *= k; }
	friend poly operator*(ll k, poly p) { return p *= k; }

	static void fwt_or(poly &a, bool invert) {
		int n = (int) a.size();
		assert((n & (n - 1)) == 0);
		for (int len = 1; len < n; len <<= 1) {
			for (int i = 0; i < n; i += len << 1) {
				for (int j = 0; j < len; ++j) {
					if (!invert) a[i + j + len] += a[i + j];
					else a[i + j + len] -= a[i + j];
					if (a[i + j + len] >= mod) a[i + j + len] -= mod;
					if (a[i + j + len] < 0) a[i + j + len] += mod;
				}
			}
		}
	}

	static void fwt_and(poly &a, bool invert) {
		int n = (int) a.size();
		assert((n & (n - 1)) == 0);
		for (int len = 1; len < n; len <<= 1) {
			for (int i = 0; i < n; i += len << 1) {
				for (int j = 0; j < len; ++j) {
					if (!invert) a[i + j] += a[i + j + len];
					else a[i + j] -= a[i + j + len];
					if (a[i + j] >= mod) a[i + j] -= mod;
					if (a[i + j] < 0) a[i + j] += mod;
				}
			}
		}
	}

	static void fwt_xor(poly &a, bool invert) {
		int n = (int) a.size();
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
			for (auto &x: a.coef)
				x = 1ll * x * inv_n % mod;
		}
	}


	template<typename F>
	static poly fwt_conv(const poly &x, const poly &y, F f, F invf) {
		poly a = x;
		poly b = y;
		int need = max(a.size(), b.size());
		int n = next_pow_2(need);
		a.coef.resize(n);
		b.coef.resize(n);
		f(a, false);
		f(b, false);
		for (int i = 0; i < n; ++i)
			a.coef[i] = int(1ll * a.coef[i] * b.coef[i] % mod);
		invf(a, true);
		a.coef.resize(need);
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

	poly operator+=(const poly &rhs) {
		int min_sz = min(size(), rhs.size());
		for (int i = 0; i < min_sz; ++i) {
			coef[i] += rhs.coef[i];
			if (coef[i] >= mod) coef[i] -= mod;
		}
		if (size() < rhs.size()) {
			coef.resize(rhs.size());
			for (int i = min_sz; i < rhs.size(); ++i) {
				coef[i] = rhs.coef[i];
			}
		}
		return *this;
	}
	poly operator-=(const poly &rhs) {
		int min_sz = min(size(), rhs.size());
		for (int i = 0; i < min_sz; ++i) {
			coef[i] -= rhs.coef[i];
			if (coef[i] < 0) {
				coef[i] += mod;
			}
		}
		if (size() < rhs.size()) {
			coef.resize(rhs.size());
			for (int i = size(); i < rhs.size(); ++i) {
				coef[i] = -rhs.coef[i] + mod;
			}
		}
		return *this;
	}
	friend poly operator+(poly lhs, const poly &rhs) {
		return lhs += rhs;
	}
	friend poly operator-(poly lhs, const poly &rhs) {
		return lhs -= rhs;
	}
	poly operator+=(ll k) {
		coef[0] = ((coef[0] + k) % mod + mod) % mod;
		return *this;
	}
	poly operator-=(ll k) {
		coef[0] = ((coef[0] - k) % mod + mod) % mod;
		return *this;
	}
	friend poly operator+(poly lhs, ll k) {
		return lhs += k;
	}
	friend poly operator-(poly lhs, ll k) {
		return lhs -= k;
	}
	friend poly operator-(ll k, poly rhs) {
		return rhs * -1 + k;
	}
	friend poly operator+(ll k, poly rhs) {
		return rhs + k;
	}
	poly inv() const {
		poly cur_a(vector<ll>{coef[0]});

		poly b(vector<ll>{pow_mod(coef[0], mod - 2)});
		for (int k = 1; k < size(); k *= 2) {
			for (int i = k; i < min(2 * k, size()); ++i) {
				cur_a.coef.push_back(coef[i]);
			}
			b *= 2 - b * cur_a;
			b.coef.resize(2 * k);
		}
		return b;
	}

	poly operator/(poly rhs) const {
		return (*this) * rhs.inv();
	}
	poly operator%(poly rhs) {
		return (*this) - ((*this) / rhs) * rhs;
	}
	poly log() const {
		poly res = this->deriv() * this->inv();
		res.coef.resize(size() - 1);
		return res.integral();
	}
	void log_inplace() {
		*this = this->deriv() * this->inv();
		coef.resize(size() - 1);
		integral_inplace();
	}
	poly exp() const {
		if (size() == 1) {
			return poly(vector<ll>{1});
		}
		poly cur_a(vector<ll>{coef[0], coef[1]});
		poly b(vector<ll>{1});
		for (int k = 1; k < size(); k *= 2) {
			for (int i = 2 * k; i < min(4 * k, size()); ++i) {
				cur_a.coef.push_back(coef[i]);
			}
			b *= (cur_a - b.log() + 1);
			b.coef.resize(4 * k);
		}
		return b;
	}

	static inline bool euler_rule(int x) {
		return x == 0 || pow_mod(x, (mod - 1) / 2) == 1;
	}

	static inline int mod_sqrt(int n) {
		ll a, i2;
		mt19937 engine(chrono::steady_clock::now().time_since_epoch().count());
		while (true) {
			a = uniform_int_distribution<ll>(0, mod)(engine);
			i2 = ((a * a - n) % mod + mod) % mod;
			if (!euler_rule(i2)) {
				break;
			}
		}
		int pow = (mod + 1) / 2;
		pll ans = {1, 0};
		pll base = {a, 1};
		while (pow > 0) {
			if (pow & 1)
				ans = {(ans.first * base.first + (i2 * ((ans.second * base.second) % mod)) % mod) % mod,
				       (ans.first * base.second + ans.second * base.first) % mod};
			base = {(base.first * base.first + ((i2 * ((base.second * base.second) % mod)) % mod)) % mod,
			        (2 * base.first * base.second) % mod};
			pow >>= 1;
		}
		return ans.first;
	}

	poly sqrt() const {
		int i = 0;
		for (; i < size(); ++i) {
			if (coef[i] != 0) {
				break;
			}
		}
		if (i == size()) {
			return poly(vector<ll>(size(), 0));
		}
		if (i & 1) {
			throw domain_error("poly::sqrt: leading coefficient is not a quadratic residue");
		}
		auto cur_coef = vector<ll>(coef.begin() + i, coef.end());
		if (!euler_rule(cur_coef[0])) {
			throw domain_error("poly::sqrt: leading coefficient is not a quadratic residue");
		}
		poly b(vector<ll>{mod_sqrt(cur_coef[0])});
		if (cur_coef.size() > 1) {
			poly cur_a(vector<ll>{cur_coef[0], cur_coef[1]});
			int inv2 = (mod + 1) / 2;
			for (int k = 1; k < size() - i / 2; k *= 2) {
				for (int i = 2 * k; i < min(4 * k, cur_coef.size()); ++i) {
					cur_a.coef.push_back(cur_coef[i]);
				}
				b += cur_a * b.inv();
				b *= inv2;
				b.coef.resize(4 * k);
			}
		}
		poly ans(size());
		for (int j = i / 2; j < size(); ++j) {
			ans[j] = b[j - i / 2];
		}
		return ans;
	}

	void sqrt_inplace() {
		int i = 0;
		for (; i < size(); ++i) {
			if (coef[i] != 0) {
				break;
			}
		}
		if (i == size()) {
			coef.assign(size(), 0);
		}
		if (i & 1) {
			throw domain_error("poly::sqrt: leading coefficient is not a quadratic residue");
		}
		auto cur_coef = vector<ll>(coef.begin() + i, coef.end());
		if (!euler_rule(cur_coef[0])) {
			throw domain_error("poly::sqrt: leading coefficient is not a quadratic residue");
		}
		poly b(vector<ll>{mod_sqrt(cur_coef[0])});
		if (cur_coef.size() > 1) {
			poly cur_a(vector<ll>{cur_coef[0], cur_coef[1]});
			int inv2 = (mod + 1) / 2;
			for (int k = 1; k < size() - i / 2; k *= 2) {
				for (int i = 2 * k; i < min(4 * k, cur_coef.size()); ++i) {
					cur_a.coef.push_back(cur_coef[i]);
				}
				b += cur_a * b.inv();
				b *= inv2;
				b.coef.resize(4 * k);
			}
		}
		for (int j = 0; j < i / 2; ++j) {
			coef[j] = 0;
		}
		for (int j = i / 2; j < size(); ++j) {
			coef[j] = b[j - i / 2];
		}
	}

	poly operator>>(int k) const {
		return poly(vector<ll>(coef.begin() + k, coef.end()));
	}
	poly operator<<(int k) const {
		vector<ll> new_coef(k, 0);
		for (auto &c: coef) {
			new_coef.push_back(c);
		}
		return poly(new_coef);
	}
	poly operator>>=(int k) {
		return *this >> k;
	}
	poly operator<<=(int k) {
		return *this << k;
	}

	poly pow(ll k) const {
		if (k == 0) {
			poly ans(vector<ll>(size(), 0));
			ans[0] = 1;
			return ans;
		}
		int i = 0;
		for (; i < size(); ++i) {
			if (coef[i] != 0) {
				break;
			}
		}
		if (i == size()) {
			return poly(vector<ll>(size(), 0));
		}

		return (pow_mod(coef[i], k) * ((k % mod) * ((*this >> i).log())).exp()) << min(min(k, size()) * i, size());
	}

	void pow_inplace(ll k) {
		if (k == 0) {
			coef[0] = 1;
		}
		int i = 0;
		for (; i < size(); ++i) {
			if (coef[i] != 0) {
				break;
			}
		}
		if (i == size()) {
			coef.assign(size(), 0);
			return;
		}
		ll leading = pow_mod(coef[i], k);
		*this >>= i;
		this->log_inplace();
		*this *= k % mod;
		*this = leading * this->exp() << min(min(k, size()) * i, size());
	}

	poly deriv() const {
		if (size() == 1) {
			return poly(vector<int>{0});
		}
		poly res(size() - 1);
		for (size_t i = 0; i < size() - 1; ++i) {
			res.coef[i] = (1ll * coef[i + 1] * (i + 1)) % mod;
		}
		return res;
	}

	poly integral() const {
		int n = size();
		poly res(n + 1);
		res.coef[0] = 0;
		for (int i = 0; i < n; ++i) {
			res.coef[i + 1] = (1ll * coef[i] * pow_mod(i + 1, mod - 2)) % mod;
		}
		return res;
	}
	void deriv_inplace() {
		if (size() == 1) {
			coef[0] = 0;
			return;
		}
		for (int i = 0; i < size() - 1; ++i) {
			coef[i] = (1ll * coef[i + 1] * (i + 1)) % mod;
		}
		coef.resize(size() - 1);
	}
	void integral_inplace() {
		coef.resize(size() + 1);
		for (int i = size() - 1; i >= 1; --i) {
			coef[i] = (1ll * coef[i - 1] * pow_mod(i, mod - 2)) % mod;
		}
		coef[0] = 0;
	}
	bool operator==(const poly &rhs) const {
		return coef == rhs.coef;
	}
	bool operator!=(const poly &rhs) const {
		return !(rhs == *this);
	}
};