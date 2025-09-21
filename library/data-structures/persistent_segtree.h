#pragma once

template<class T>
struct segtree {
	int n;

	T unit;
	function<T(T, T)> f;

	struct Node {
		T val;
		Node *l, *r;
		Node(T v, segtree<T> *p) : val(v), l(nullptr), r(nullptr) {}
		Node(segtree<T> *p) : val(p->unit), l(nullptr), r(nullptr) {}
		Node(Node *ll, Node *rr, segtree<T> *p) : l(ll), r(rr), val(p->unit) {
			if (l) val = p->f(val, l->val);
			if (r) val = p->f(val, r->val);
		}
	};

	vector<Node *> roots;
	segtree(int n, T unit, function<T(T, T)> f) : n(n), unit(unit), f(f) {
		roots.push_back(build(0, n - 1));
	}
	Node *build(int l, int r) {
		if (l == r) return new Node(this);
		int m = (l + r) / 2;
		return new Node(build(l, m), build(m + 1, r), this);
	}

	Node *update(Node *cur, int i, int v, int l, int r) {
		if (l == r) {
			return new Node(cur->val + v, this);
		}
		int m = (l + r) / 2;
		if (i <= m) {
			return new Node(update(cur->l, i, v, l, m), cur->r, this);
		} else {
			return new Node(cur->l, update(cur->r, i, v, m + 1, r), this);
		}
	}

	void update(int i, int v, int t = -1) {
		if (t == -1) t = roots.size() - 1;
		roots.push_back(update(roots[t], i, v, 0, n - 1));
	}

	Node *set(Node *cur, int i, int v, int l, int r) {
		if (l == r) {
			return new Node(cur->val = v, this);
		}
		int m = (l + r) / 2;
		if (i <= m) {
			return new Node(set(cur->l, i, v, l, m), cur->r, this);
		} else {
			return new Node(cur->l, set(cur->r, i, v, m + 1, r), this);
		}
	}

	void set(int i, int v, int t = -1) {
		if (t == -1) t = roots.size() - 1;
		roots.push_back(set(roots[t], i, v, 0, n - 1));
	}

	T query(Node *cur, int a, int b, int l, int r) {
		if (r <= a || l >= b) {
			return unit;
		}
		if (a <= l && r <= b) {
			return cur->val;
		}
		int m = (l + r) / 2;
		return f(query(cur->l, a, b, l, m), query(cur->r, a, b, m + 1, r));
	}

	int query(int l, int r, int t = -1) {
		if (t == -1) t = roots.size() - 1;
		return query(roots[t], l, r, 0, n - 1);
	}
};