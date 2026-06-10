#include <bits/stdc++.h>

using namespace std;

template<typename T, typename F>
struct splay_tree {
	struct Node {
		T v;
		T subtree_val;
		int sz = 1;
		Node *p = nullptr;
		Node *c[2] = {nullptr, nullptr};

		Node(T val) : v(val), subtree_val(val) {}
	};

	inline T v(Node *x) {
		return (x == nullptr ? unit : x->v);
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

	F f;
	T unit;

	Node *root;

	splay_tree(const T &unit, const F &f, Node *r = nullptr) : f(f), unit(unit), root(r) {}

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
				return {splay_tree(unit, f, nullptr), splay_tree(unit, f, r)};
			} else {
				Node *l = root;
				root = nullptr;
				return {splay_tree(unit, f, l), splay_tree(unit, f, nullptr)};
			}
		}

		Node *r = root->c[1];
		if (r) r->p = nullptr;
		root->c[1] = nullptr;
		Node *l = root;
		root = nullptr;
		pull(l), pull(r);
		return {splay_tree(unit, f, l), splay_tree(unit, f, r)};
	}

	splay_tree merge(splay_tree l, splay_tree r) {
		if (!l.root) return r;
		if (!r.root) return l;
		Node *x = l.root;
		while (x->c[1]) {
			x = x->c[1];
		}
		l.splay(x);
		l.link(l.root, r.root, 1);
		r.root = nullptr;
		pull(l.root);
		return l;
	}

	void insert(int idx, const T &val) {
		auto [l, r] = split(idx - 1);
		root = new Node(val);
		link(root, l.root, 0);
		link(root, r.root, 1);
		pull(root);
	}

	void erase(int idx) {
		Node *x = find(idx);
		if (!x) return;

		Node *l = root->c[0];
		Node *r = root->c[1];

		if (l) l->p = nullptr;
		if (r) r->p = nullptr;

		root = merge(splay_tree(unit, f, l), splay_tree(unit, f, r)).root;
		pull(root);
	}

	// returns the val in [l, r]
	T query(int l, int r) {
		auto [left, midright] = split(l - 1);
		auto [mid, right] = midright.split(r - l);
		T ans = subtree_val(mid.root);
		root = merge(left, merge(mid, right)).root;
		return ans;
	}

	template<typename APPLY>
	void change(int idx, const T &val, APPLY apply) {
		Node *x = find(idx);
		if (x) {
			apply(x->v, val);
			pull(x);
		}
	}

	void update(int idx, T val) {
		change(idx, val, [](T &a, const T &b) { return a = a + b; });
	}

	void set(int idx, T val) {
		change(idx, val, [](T &a, const T &b) { return a = b; });
	}
};