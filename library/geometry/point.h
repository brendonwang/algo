#pragma once

template<class T>
struct point {
	T x, y;
	friend istream &operator>>(istream &input, point<T> &p) {
		input >> p.x >> p.y;
		return input;
	}
	friend ostream &operator<<(ostream &output, const point<T> &p) {
		output << (p.x == 0 ? 0 : p.x) << ' ' << (p.y == 0 ? 0 : p.y);
		return output;
	}
	point<T> operator+(const point<T> &p) const { return {x + p.x, y + p.y}; }
	point<T> operator-(const point<T> &p) const { return {x - p.x, y - p.y}; }
	point<T> operator*(const T &k) const { return {x * k, y * k}; }
	point<T> operator/(const T &k) const { return {x / k, y / k}; }
	friend T dot(const point<T> &p1, const point<T> &p2) { return p1.x * p2.x + p1.y * p2.y; }
	friend T cross(const point<T> &p1, const point<T> &p2) { return p1.x * p2.y - p1.y * p2.x; }
	T cross(const point<T> &p1, const point<T> &p2) const {
		return (x - p1.x) * (p2.y - p1.y) - (y - p1.y) * (p2.x - p1.x);
	}
	friend point<T> midpoint(const point<T> &p1, const point<T> &p2) { return {(p1.x + p2.x) / 2, (p1.y + p2.y) / 2}; }
	T norm() const { return x * x + y * y; }
	T abs() const { return sqrt(norm()); }
	friend point<T> proj(const point<T> &a, const point<T> &b) { return b * (dot(a, b) / dot(b, b)); }
	point<T> rotate90() const { return {-y, x}; }
	point<T> rotate180() const { return {-x, -y}; }
	point<T> rotate270() const { return {y, -x}; }
	point<T> rotate(T angle) const {
		T rad = angle * M_PI / 180.0;
		T cosA = cos(rad);
		T sinA = sin(rad);
		return {x * cosA - y * sinA, x * sinA + y * cosA};
	}
	point<T> reflect(const point<T> &p1, const point<T> &p2) const {
		return *this + (p1 + proj(*this - p1, p2 - p1) - (*this)) * 2;
	}
	point<T> unit() const { return *this / abs(); }
	point<T> normal() const { return {-y, x}; }
	point<T> perp() const { return {y, -x}; }
	static constexpr T eps = 1e-9;
	bool operator==(const point<T> &p) const { return std::abs(x - p.x) < eps && std::abs(y - p.y) < eps; }
	bool operator!=(const point<T> &p) const { return !(*this == p); }
	bool operator<(const point<T> &p) const { return x < p.x - eps || (std::abs(x - p.x) < eps && y < p.y - eps); }
	bool operator<=(const point<T> &p) const { return *this < p || *this == p; }
	bool operator>(const point<T> &p) const { return !(*this <= p); }
	bool operator>=(const point<T> &p) const { return !(*this < p); }
};
