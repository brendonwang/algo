#pragma once

template<class T>
struct segtree {
	vector<T> seg;
	int n;
	T unit;
	function<T(T, T)> f;

	segtree(int n, T unit, function<T(T, T)> f) : n(n), unit(unit), f(f) {
		seg.assign(2 * n, unit);
	}

	segtree(int n, vector<T> &a, T unit, function<T(T, T)> f) : n(n), unit(unit), f(f), seg(2 * n, unit) {
		build(a);
	}

	segtree(vector<T> &a, T unit, function<T(T, T)> f) : n(a.size()), unit(unit), f(f), seg(2 * n, unit) {
		build(a);
	}

	void build(vector<T> &a) {
		for (int i = 0; i < n; ++i) {
			seg[i + n] = a[i];
		}
		for (int i = n - 1; i > 0; --i) {
			seg[i] = f(seg[i << 1], seg[i << 1 | 1]);
		}
	}

	void update(int i, T val) {
		for (seg[i += n] += val; i >>= 1;) {
			seg[i] = f(seg[i << 1], seg[i << 1 | 1]);
		}
	}

	T query(int l, int r) {
		T ra = unit, rb = unit;
		for (l += n, r += n; l < r; l >>= 1, r >>= 1) {
			if (l & 1) {
				ra = f(ra, seg[l++]);
			}
			if (r & 1) {
				rb = f(seg[--r], rb);
			}
		}
		return f(ra, rb);
	}
};