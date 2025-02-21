#pragma once


// range min segment tree
//SegmentTree<int> seg(arr, oo, [](int a, int b) {return min(a, b); }, [](int& a, int x, int len) {a += x; });
// range max segment tree
//SegmentTree<int> seg(arr, -oo, [](int a, int b) {return max(a, b); }, [](int& a, int x, int len) {a += x; });
// range sum segment tree
//SegmentTree<int> seg(arr, 0, [](int a, int b) {return a + b; }, [](int& a, int x, int len) {a += x * len; });\

template<class T>
struct lazy_segtree {
	vector<T> seg;
	vector<T> lazy;
	int n;
	T unit;
	function<void(T &, T, int)> add;
	function<T(T, T)> f;
	function<void(T &, T)> combine;

	lazy_segtree(int n, T unit, function<T(T, T)> _f, function<void(T &, T, int)> add,
	             function<void(T &, T)> combine = [](T &a, T b) { a += b; }) : n(n),
	                                                                           seg(4 * n, unit),
	                                                                           lazy(4 * n, unit),
	                                                                           f(_f),
	                                                                           add(add),
	                                                                           combine(combine) {}

	void build(vector<T> &a) {
		for (int i = 0; i < n; ++i) {
			update(i, i + 1, a[i]);
		}
	}

	void propagate(int x, int xl, int xr) {
		int m = (xl + xr) / 2;
		add(seg[2 * x], lazy[x], m - xl + 1);
		add(seg[2 * x + 1], lazy[x], xr - m);
		combine(lazy[2 * x], lazy[x]);
		combine(lazy[2 * x + 1], lazy[x]);
		lazy[x] = unit;
	}

	void update(int l, int r, T v, int x, int xl, int xr) {
		if (xr < l || xl > r) {
			return;
		}
		if (l <= xl && xr <= r) {
			combine(lazy[x], v);
			add(seg[x], v, xr - xl + 1);
			return;
		}
		propagate(x, xl, xr);
		int m = (xl + xr) / 2;
		update(l, r, v, 2 * x, xl, m);
		update(l, r, v, 2 * x + 1, m + 1, xr);
		seg[x] = f(seg[2 * x], seg[2 * x + 1]);
	}

	void update(int l, int r, T v) {
		update(l, r, v, 1, 0, n - 1);
	}

	T query(int l, int r, int x, int xl, int xr) {
		if (xr < l || xl > r) {
			return unit;
		}
		if (l <= xl && xr <= r) {
			return seg[x];
		}
		propagate(x, xl, xr);
		int m = (xl + xr) / 2;
		return f(query(l, r, 2 * x, xl, m), query(l, r, 2 * x + 1, m + 1, xr));
	}

	T query(int l, int r) {
		return query(l, r, 1, 0, n - 1);
	}
};