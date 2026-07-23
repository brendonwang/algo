template<typename T, typename F>
struct segtree {
	int n;
	vector<T> seg;
	T unit;
	F f;
	segtree(int n, T unit, const F &f) : n(n), unit(unit), f(f), seg(4 * n, unit) {}
	segtree(int n, const vector<T> &a, T unit, const F &f) : n(n), unit(unit), f(f), seg(4 * n, unit) { build(a); }
	segtree(const vector<T> &a, T unit, const F &f) : n(a.size()), unit(unit), f(f), seg(4 * n, unit) { build(a); }
	void build(const vector<T> &a, int x, int xl, int xr) {
		if (xl == xr) seg[xl] = a[xl];
		int m = xl + (xr - xl) / 2;
		build(a, 2*x+1, xl, m);
		build(a, 2*x+2, m+1, xr);
		seg[x] = f(seg[2*x+1], seg[2*x+2]);
	}
	void build(const vector<T> &a) {
		build(a, 0, 0, n-1);
	}
	void update(int i, T val, int x, int xl, int xr) {
		if (xl == xr) seg[xl] += val;
		int m = xl + (xr - xl) / 2;
		if (i <= m)	update(i, val, 2*x+1, xl, m);
		else update(i, val, 2*x+2, m+1, xr);
		seg[x] = f(seg[2*x+1], seg[2*x+2]);
	}
	void update(int i, T val) {
		update(i, val, 0, 0, n-1);
	}
	void set(int i, T val, int x, int xl, int xr) {
		if (xl == xr) seg[xl] = val;
		int m = xl + (xr - xl) / 2;
		if (i <= m)	set(i, val, 2*x+1, xl, m);
		else set(i, val, 2*x+2, m+1, xr);
		seg[x] = f(seg[2*x+1], seg[2*x+2]);
	}
	void set(int i, T val) {
		set(i, val, 0, 0, n-1);
	}
	T query(int l, int r, int x, int xl, int xr) {
		if (xr < l || r > xl) return unit;
		if (l <= xl && xr <= r) return seg[x];
		int m = xl + (xr - xl) / 2;
		return f(query(l, r, 2*x+1, xl, m), query(l, r, 2*x+2, m+1, xr));
	}
};