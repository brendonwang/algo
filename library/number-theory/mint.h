#pragma once

template<auto &M = MOD>
struct mint {
	long long v;
	mint() : v(0) {}
	mint(long long x) { v = x >= 0 ? x % M : x + (-x + M - 1) / M * M; }
	friend ostream &operator<<(ostream &stream, const mint &x) {
		stream << x.v;
		return stream;
	}
	friend istream &operator>>(istream &stream, mint &val) {
		long long x;
		stream >> x;
		val.v = x >= 0 ? x % M : x + (-x + M - 1) / M * M;
		return stream;
	}
	operator long long() const { return v; }
	mint &operator++() {
		if (++v >= M) { v -= M; }
		return *this;
	}
	mint &operator--() {
		if (--v < 0) { v += M; }
		return *this;
	}
	mint operator++(int) {
		mint temp = *this;
		if (++v >= M) { v -= M; }
		return temp;
	}
	mint operator--(int) {
		mint temp = *this;
		if (--v < 0) { v += M; }
		return temp;
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
		if (y.v) { v %= y.v; }
		return *this;
	}
	mint operator%(const mint &y) const {
		mint x = *this;
		return x %= y;
	}
	mint &operator/=(const mint &y) { return *this *= inv(y.v); }
	mint operator/(const mint &y) const { return *this * inv(y.v); }
	mint &operator^=(const mint &y) {
		*this = this->pow(y);
		return *this;
	}
	mint pow(long long y) const {
		mint r = 1, x = v;
		for (y <<= 1; y >>= 1; x = x * x) { if (y & 1) { r = r * x; }}
		return r;
	}
	mint inv(long long a) const { return mint(a) ^ (M - 2); }
	mint inv() const { return this ^ (M - 2); }
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
	friend mint operator^(const mint &a, long long b) { return a.pow(b); }
	friend mint operator^(long long a, const mint &b) { return mint(a).pow(b); }
	friend mint operator^(const mint &a, int32_t b) { return a.pow(b); }
	friend mint operator^(int32_t a, const mint &b) { return mint(a).pow(b); }
	friend mint operator^(const mint &a, const mint &b) { return a.pow(b); }
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
};