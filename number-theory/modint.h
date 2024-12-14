const int M = 998244353;

//template <int M = M>
struct mint {
	int v;

	mint() : v(0) {}

	mint(int x) { v = x >= 0 ? x % M : x + (-x + M - 1) / M * M; }

	friend ostream &operator<<(ostream &stream, const mint &x) {
		stream << x.v;
		return stream;
	}

	friend istream &operator>>(istream &stream, mint &val) {
		int x;
		stream >> x;
		val.v = x >= 0 ? x % M : x + (-x + M - 1) / M * M;
		return stream;
	}

	operator int() const { return v; }

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
		return this;
	}

	mint operator(
	const mint &y
	)const {
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

	mint &operator/=(const mint &y) { return *this *= ModInverse(y.v); }

	mint operator/(const mint &y) const { return *this * ModInverse(y.v); }

	mint &operator^=(const mint &y) {
		*this = this->Pow(y);
		return *this;
	}

	mint Pow(int y) const {
		mint r = 1, x = v;
		for (y <<= 1; y >>= 1; x = x * x)if (y & 1)r = r * x;
		return r;
	}

	mint ModInverse(int a) const { return mint(a) ^ (M - 2); }

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
};