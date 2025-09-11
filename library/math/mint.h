#pragma once

template<auto &M = MOD>
struct mint {
	ll v;
	mint() : v(0) {}
	mint(ll x) { v = x >= 0 ? x % M : x + (-x + M - 1) / M * M; }
	friend ostream &operator<<(ostream &stream, const mint &x) {
		stream << x.v;
		return stream;
	}
	friend istream &operator>>(istream &stream, mint &val) {
		ll x;
		stream >> x;
		val.v = x >= 0 ? x % M : x + (-x + M - 1) / M * M;
		return stream;
	}
	explicit operator ll() const { return v; }
	mint &operator++() {
		if (++v >= M)v -= M;
		return *this;
	}
	mint &operator--() {
		if (--v < 0)v += M;
		return *this;
	}
	mint &operator+=(const mint &y) {
		v = v + y.v - ((v + y.v) >= M ? M : 0);
		return *this;
	}
	mint operator+(const mint &y) const {
		mint x = *this;
		return x += y;
	}
	mint &operator-=(const mint &y) {
		v = v - y.v + (v - y.v < 0 ? M : 0);
		return *this;
	}
	mint operator-(const mint &y) const {
		mint x = *this;
		return x -= y;
	}
	mint &operator*=(const mint &y) {
		v = ((long long) v * y.v) % M;
		return *this;
	}
	mint operator*(const mint &y) const {
		mint x = *this;
		return x *= y;
	}
	mint &operator%=(const mint &y) {
		if (y.v)v %= y.v;
		return *this;
	}
	mint operator%(const mint &y) const {
		mint x = *this;
		return x %= y;
	}
	mint &operator/=(const mint &y) { return *this *= modinv(y); }
	mint operator/(const mint &y) const { return *this * modinv(y); }
	mint &operator^=(const mint &y) {
		*this = this->Pow(y);
		return *this;
	}
	mint Pow(ll y) const {
		mint r = 1, x = v;
		for (y <<= 1; y >>= 1; x = x * x)if (y & 1)r = r * x;
		return r;
	}
	mint modinv(ll a) const { return mint(a) ^ (M - 2); }
	mint modinv(mint a) const { return a ^ (M - 2); }
	friend mint modinv(mint a) { return a ^ (M - 2); }
	friend mint operator+(const mint &a, long long b) { return a + mint(b); }
	friend mint operator+(long long a, const mint &b) { return mint(a) + b; }
	friend mint operator+(const mint &a, int32_t b) { return a + mint(b); }
	friend mint operator+(int32_t a, const mint &b) { return mint(a) + b; }
	friend mint operator-(const mint &a, long long b) { return a - mint(b); }
	friend mint operator-(long long a, const mint &b) { return mint(a) - b; }
	friend mint operator-(const mint &a, int32_t b) { return a - mint(b); }
	friend mint operator-(int32_t a, const mint &b) { return mint(a) - b; }
	friend mint operator*(const mint &a, long long b) { return a * mint(b); }
	friend mint operator*(long long a, const mint &b) { return mint(a) * b; }
	friend mint operator*(const mint &a, int32_t b) { return a * mint(b); }
	friend mint operator*(int32_t a, const mint &b) { return mint(a) * b; }
	friend mint operator/(const mint &a, int32_t b) { return a / mint(b); }
	friend mint operator/(int32_t a, const mint &b) { return mint(a) / b; }
	friend mint operator/(const mint &a, long long b) { return a / mint(b); }
	friend mint operator/(long long a, const mint &b) { return mint(a) / b; }
	friend mint operator^(const mint &a, long long b) { return a.Pow(b); }
	friend mint operator^(long long a, const mint &b) { return mint(a).Pow(b); }
	friend mint operator^(const mint &a, int32_t b) { return a.Pow(b); }
	friend mint operator^(int32_t a, const mint &b) { return mint(a).Pow(b); }
	friend mint operator^(const mint &a, const mint &b) { return a.Pow(b); }
	bool operator==(const mint &y) const { return v == y.v; }
	bool operator==(int32_t y) const { return v == y; }
	bool operator==(long long y) const { return v == y; }
	bool operator!=(const mint &y) const { return v != y.v; }
	bool operator!=(int32_t y) const { return v != y; }
	bool operator!=(long long y) const { return v != y; }
	bool operator>(const mint &y) const { return v > y.v; }
	bool operator>(int32_t y) const { return v > y; }
	bool operator>(long long y) const { return v > y; }
	bool operator>=(const mint &y) const { return v >= y.v; }
	bool operator>=(int32_t y) const { return v >= y; }
	bool operator>=(long long y) const { return v >= y; }
	bool operator<(const mint &y) const { return v < y.v; }
	bool operator<(int32_t y) const { return v < y; }
	bool operator<(long long y) const { return v < y; }
	bool operator<=(const mint &y) const { return v <= y.v; }
	bool operator<=(int32_t y) const { return v <= y; }
	bool operator<=(long long y) const { return v <= y; }

	bool legendre() {
		return v == 0 || Pow((M - 1) / 2) == 1;
	}

	optional<mint> mod_sqrt() {
		if (v == 0) return 0;
		if (!legendre()) return nullopt;
		if (M == 2) return v;
		mint a, i2;
		static mt19937 engine(chrono::steady_clock::now().time_since_epoch().count());
		while (true) {
			a = uniform_int_distribution<ll>(0, M - 1)(engine);
			i2 = a * a - v;
			if (!i2.legendre()) {
				break;
			}
		}

		if (i2.legendre()) {
			throw;
		}
		ll pow = (M + 1) / 2;
		pair<mint, mint> ans = {1, 0};
		pair<mint, mint> base = {a, 1};
		while (pow > 0) {
			if (pow & 1)
				ans = {(ans.first * base.first + i2 * (ans.second * base.second)),
				       (ans.first * base.second + ans.second * base.first)};
			base = {(base.first * base.first + i2 * (base.second * base.second)),
			        2 * base.first * base.second};
			pow >>= 1;
		}
		return ans.first;
	}

};