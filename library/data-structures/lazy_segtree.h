#pragma once

template<typename T, typename lazy_tag = T>
struct lazy_segtree {
	int n;
	T unit;
	lazy_tag lazy_unit;
	vector<T> seg;
	vector<lazy_tag> lazy;
	function<T(T, T)> f;
	function<void(T &, lazy_tag, int)> add;
	function<void(lazy_tag &, lazy_tag)> lazy_add;

	lazy_segtree(int n, T unit, function<T(T, T)> f, function<void(T &, lazy_tag, int)> add,
	             function<void(lazy_tag &, lazy_tag)> lazy_add = [](lazy_tag &a, lazy_tag b) { a += b; }) :
			n(n), seg(4 * n, unit),
			lazy(4 * n, unit),
			f(f),
			add(add), lazy_add(lazy_add), unit(unit),
			lazy_unit(unit) {}
	lazy_segtree(int n, T unit, function<T(T, T)> f, function<void(T &, lazy_tag, int)> add, lazy_tag lazy_unit,
	             function<void(lazy_tag &, lazy_tag)> lazy_add = [](lazy_tag &a, lazy_tag b) { a += b; }) :
			n(n), seg(4 * n, unit),
			lazy(4 * n, lazy_unit),
			f(f),
			add(add), lazy_add(lazy_add), unit(unit),
			lazy_unit(lazy_unit) {}
	void propagate(int x, int l, int r) {
		int mid = l + (r - l) / 2;
		add(seg[2 * x + 1], lazy[x], mid - l + 1);
		add(seg[2 * x + 2], lazy[x], r - mid);
		lazy_add(lazy[2 * x + 1], lazy[x]);
		lazy_add(lazy[2 * x + 2], lazy[x]);
		lazy[x] = lazy_unit;
	}
	template<typename F>
	void change(int x, int xl, int xr, int l, int r, lazy_tag val, F apply) {
		if (xr < l || xl > r) {
			return;
		}
		if (l <= xl && xr <= r) {
			lazy[x] = apply(lazy[x], val);
			add(seg[x], val, xr - xl + 1);
			return;
		}
		propagate(x, xl, xr);
		int mid = xl + (xr - xl) / 2;
		change(2 * x + 1, xl, mid, l, r, val, apply);
		change(2 * x + 2, mid + 1, xr, l, r, val, apply);
		seg[x] = f(seg[2 * x + 1], seg[2 * x + 2]);
	}
	template<typename F>
	void change(int l, int r, lazy_tag val, F apply) {
		change(0, 0, n - 1, l, r, val, apply);
	}
	void update(int l, int r, lazy_tag val) {
		change(l, r, val, [](lazy_tag a, lazy_tag b) { return a + b; });
	}
	void set(int l, int r, lazy_tag val) {
		change(l, r, val, [](lazy_tag a, lazy_tag b) { return b; });
	}
	void apply(int l, int r, lazy_tag val) {
		change(l, r, val, [&](lazy_tag a, lazy_tag b) {
			lazy_add(a, b);
			return a;
		});
	}
	T query(int l, int r, int x, int xl, int xr) {
		if (xr < l || xl > r) {
			return unit;
		}
		if (l <= xl && xr <= r) {
			return seg[x];
		}
		propagate(x, xl, xr);
		int mid = (xl + xr) / 2;
		return f(query(l, r, 2 * x + 1, xl, mid), query(l, r, 2 * x + 2, mid + 1, xr));
	}

	T query(int l, int r) {
		return query(l, r, 0, 0, n - 1);
	}
};