#pragma once

template<typename T, typename I = int>
struct segtree {
	struct Node {
		Node *l = nullptr, *r = nullptr;
		T v;
		Node(const T &v) : l(nullptr), r(nullptr), v(v) {}
	};

	Node *left(Node *x) {
		if (!x->l) {
			x->l = new Node(unit);
		}
		return x->l;
	}
	Node *right(Node *x) {
		if (!x->r) {
			x->r = new Node(unit);
		}
		return x->r;
	}

	T unit;
	I n;
	function<T(T, T)> f;
	Node *root;
	segtree(I n, T unit, function<T(T, T)> f) : n(n), unit(unit), f(f), root(new Node(unit)) {}
	void update(Node *x, I xl, I xr, I i, T val) {
		if (xl == xr) {
			x->v += val;
			return;
		}
		I mid = xl + (xr - xl) / 2;
		if (i <= mid) {
			update(left(x), xl, mid, i, val);
		} else {
			update(right(x), mid + 1, xr, i, val);
		}
	}
	void update(I i, T val) {
		update(root, 0, n - 1, i, val);
	}
	void set(Node *x, I xl, I xr, I i, T val) {
		if (xl == xr) {
			x->v = val;
			return;
		}
		I mid = xl + (xr - xl) / 2;
		if (i <= mid) {
			set(left(x), xl, mid, i, val);
		} else {
			set(right(x), mid + 1, xr, i, val);
		}
		x->v = f(left(x)->v, right(x)->v);
	}
	void set(I i, T val) {
		set(root, 0, n - 1, i, val);
	}
	T query(Node *x, I xl, I xr, I l, I r) {
		if (r < xl || xr < l) {
			return unit;
		}
		if (l <= xl && xr <= r) {
			return x->v;
		}
		I mid = xl + (xr - xl) / 2;
		return f(query(left(x), xl, mid, l, r), query(right(x), mid + 1, xr, l, r));
	}
	T query(I l, I r) {
		return query(root, 0, n - 1, l, r);
	}
};
