#include <bits/stdc++.h>

using namespace std;

template<typename T>
struct splay_tree {
	struct Node {
		T v;
		Node *p = nullptr;
		Node *c[2] = {nullptr, nullptr};

		Node() = default;

		Node(T val) : v(val) {}
	};

	Node *root;

	splay_tree(Node *r = nullptr) : root(r) {}

	int dir(Node *x) {
		if (x->p == nullptr) {
			return -1;
		}
		if (x->p->c[0] == x) {
			return 0;
		} else {
			return 1;
		}
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
	}

	void splay(Node *x) {
		while (~dir(x)) {
			Node *y = x->p;
			int dx = dir(x), dy = dir(y);
			if (~dy) rotate(dx == dy ? y : x);
			rotate(x);
		}
	}

	Node *find(const T &val) {
		Node *x = root, *last = nullptr;
		while (x) {
			last = x;
			if (x->v == val) break;
			if (val < x->v) x = x->c[0];
			else x = x->c[1];
		}
		if (last) splay(last);
		return root && root->v == val ? root : nullptr;
	}

	Node *leq(const T &val) {
		Node *x = root, *res = nullptr;
		while (x) {
			if (x->v <= val) {
				res = x;
				x = x->c[1];
			} else {
				x = x->c[0];
			}
		}
		return res;
	}

	pair<splay_tree, splay_tree> split(const T &val) {
		Node *x = leq(val);

		if (!x) {
			Node *r = root;
			root = nullptr;
			return {splay_tree(nullptr), splay_tree(r)};
		}

		splay(x);
		Node *r = root->c[1];
		if (r) r->p = nullptr;
		root->c[1] = nullptr;
		Node *l = root;
		root = nullptr;
		return {splay_tree(l), splay_tree(r)};
	}

	static splay_tree merge(splay_tree l, splay_tree r) {
		if (!l.root) return r;
		if (!r.root) return l;
		Node *x = l.root;
		while (x->c[1]) {
			x = x->c[1];
		}
		l.splay(x);
		l.link(l.root, r.root, 1);
		r.root = nullptr;
		return l;
	}

	void insert(const T &val) {
		auto [l, r] = split(val);
		root = new Node(val);
		link(root, l.root, 0);
		link(root, r.root, 1);
	}

	void erase(const T &val) {
		Node *x = find(val);
		if (!x) return;

		Node *l = root->c[0];
		Node *r = root->c[1];

		if (l) l->p = nullptr;
		if (r) r->p = nullptr;

		root = merge(splay_tree(l), splay_tree(r)).root;
	}
};