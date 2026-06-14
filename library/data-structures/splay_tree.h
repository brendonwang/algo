#include <bits/stdc++.h>

using namespace std;

template<typename T, typename L, typename F, typename A, typename LA>
struct splay_tree {
	struct Node {
		T v;
		T subtree_val;
		L lazy;
		bool rev = false;
		int sz = 1;
		Node *p = nullptr;
		Node *c[2] = {nullptr, nullptr};

		Node(const T &val, const L &lazy) : v(val), subtree_val(val), lazy(lazy) {}
	};

	inline T v(Node *x) {
		return (x == nullptr ? unit : x->v);
	}

	inline L lazy(Node *x) {
		return (x == nullptr ? lazy_unit : x->lazy);
	}

	inline bool rev(Node *x) {
		return (x == nullptr ? false : x->rev);
	}

	inline T subtree_val(Node *x) {
		return (x == nullptr ? unit : x->subtree_val);
	}

	inline int sz(Node *x) {
		return (x == nullptr ? 0 : x->sz);
	}

	inline void pull(Node *x) {
		if (!x) return;
		x->subtree_val = f(f(subtree_val(x->c[0]), x->v), subtree_val(x->c[1]));
		x->sz = 1 + sz(x->c[0]) + sz(x->c[1]);
	}

	inline void apply(Node *x, const L &lz, bool rev) {
		if (!x) return;
		if (lz != lazy_unit) {
			add(x->v, lz, 1);
			add(x->subtree_val, lz, x->sz);
			lazy_add(x->lazy, lz);
		}
		if (rev) {
			x->rev ^= rev;
			swap(x->c[0], x->c[1]);
		}
	}

	inline void propagate(Node *x) {
		if (!x || (x->lazy == lazy_unit && !x->rev)) return;
		apply(x->c[0], x->lazy, x->rev);
		apply(x->c[1], x->lazy, x->rev);
		x->lazy = lazy_unit;
		x->rev = false;
	}

	inline splay_tree new_root(Node *x) {
		return splay_tree(unit, f, add, lazy_unit, lazy_add, x);
	}

	T unit;
	F f;
	A add;
	L lazy_unit;
	LA lazy_add;

	Node *root;

	template<class X = T, class Y = L, std::enable_if_t<std::is_same<X, Y>::value, int> = 0>
	splay_tree(const T &unit, const F &f, const A &add, const LA &lazy_add, Node *r = nullptr) : f(f),
	                                                                                             unit(unit),
	                                                                                             add(add),
	                                                                                             lazy_unit(unit),
	                                                                                             lazy_add(lazy_add),
	                                                                                             root(r) {}

	splay_tree(const T &unit, const F &f, const A &add, const L &lazy_unit, const LA &lazy_add, Node *r = nullptr) : f(
			f),
	                                                                                                                 unit(unit),
	                                                                                                                 add(add),
	                                                                                                                 lazy_unit(
			                                                                                                                 lazy_unit),
	                                                                                                                 lazy_add(
			                                                                                                                 lazy_add),
	                                                                                                                 root(r) {}

	int dir(Node *x) {
		if (x == nullptr || x->p == nullptr) return -1;
		if (x->p->c[0] == x) return 0;
		else return 1;
	}

	void link(Node *p, Node *x, int d) {
		if (p) p->c[d] = x;
		else root = x;
		if (x) x->p = p;
	}

	void rotate(Node *x) {
		Node *p = x->p, *g = p->p;
		int d = dir(x), gd = dir(p);
		propagate(p);
		propagate(x);
		link(p, x->c[d ^ 1], d);
		link(g, x, gd);
		link(x, p, d ^ 1);
		pull(p);
		pull(x);
	}

	void splay(Node *x) {
		while (~dir(x)) {
			Node *y = x->p;
			int dx = dir(x), dy = dir(y);
			if (~dy) rotate(dx == dy ? y : x);
			rotate(x);
		}
	}

	Node *find(int idx) {
		Node *x = root, *last = nullptr;
		while (x) {
			propagate(x);

			last = x;
			if (sz(x->c[0]) == idx) {
				splay(x);
				return x;
			}
			if (idx < sz(x->c[0])) x = x->c[0];
			else {
				idx -= sz(x->c[0]) + 1;
				x = x->c[1];
			}
		}
		splay(last);
		return nullptr;
	}

	pair<splay_tree, splay_tree> split(int idx) {
		Node *x = find(idx);

		if (!x) {
			if (idx < 0) {
				Node *r = root;
				root = nullptr;
				return {new_root(nullptr), new_root(r)};
			} else {
				Node *l = root;
				root = nullptr;
				return {new_root(l), new_root(nullptr)};
			}
		}

		propagate(root);
		Node *r = root->c[1];
		if (r) r->p = nullptr;
		root->c[1] = nullptr;
		Node *l = root;
		root = nullptr;
		pull(l), pull(r);
		return {new_root(l), new_root(r)};
	}

	static splay_tree merge(splay_tree l, splay_tree r) {
		if (!l.root) return r;
		if (!r.root) return l;
		Node *x = l.root;
		while (true) {
			l.propagate(x);
			if (!x->c[1]) break;
			x = x->c[1];
		}
		l.splay(x);
		l.propagate(l.root);
		l.link(l.root, r.root, 1);
		r.root = nullptr;
		l.pull(l.root);
		return l;
	}

	void insert(int idx, const T &val) {
		auto [l, r] = split(idx - 1);
		root = new Node(val, lazy_unit);
		link(root, l.root, 0);
		link(root, r.root, 1);
		pull(root);
	}

	void erase(int idx) {
		Node *x = find(idx);
		if (!x) return;

		propagate(x);

		Node *l = root->c[0];
		Node *r = root->c[1];

		if (l) l->p = nullptr;
		if (r) r->p = nullptr;

		root = merge(new_root(l), new_root(r)).root;
		pull(root);
	}

	array<splay_tree, 3> split(int l, int r) {
		auto [left, midright] = split(l - 1);
		auto [mid, right] = midright.split(r - l);
		return {left, mid, right};
	}

	T query(int l, int r) {
		auto [left, mid, right] = split(l, r);
		T ans = subtree_val(mid.root);
		root = merge(left, merge(mid, right)).root;
		return ans;
	}

	template<typename APPLY>
	void change(int idx, const T &val, APPLY apply) {
		Node *x = find(idx);
		if (x) {
			propagate(x);
			apply(x->v, val);
			pull(x);
		}
	}

	void apply(int l, int r, const L &lz) {
		auto [left, mid, right] = split(l, r);
		if (mid.root) {
			apply(mid.root, lz, false);
		}
		root = merge(left, merge(mid, right)).root;
	}

	void reverse(int l, int r) {
		auto [left, mid, right] = split(l, r);
		if (mid.root) {
			apply(mid.root, lazy_unit, true);
		}
		root = merge(left, merge(mid, right)).root;
	}
};