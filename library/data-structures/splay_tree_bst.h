#include <bits/stdc++.h>

using namespace std;

template<typename T, int MAXN = 450 << 10>
struct splay_tree {
	struct Node {
		Node *l = nullptr, *r = nullptr, *p = nullptr;
		T v;
		Node() = default;
		Node(T val) : v(val) {}
	};

	static inline Node buff[MAXN];
	static inline int cnt = 0;

	Node *new_node(int val) {
		assert(cnt < MAXN);
		buff[cnt].v = val;
		return &buff[cnt++];
	}

	Node *root;

	splay_tree(Node *r = nullptr) : root(r) {}

	void rotateLeft(Node *x) {
		Node *y = x->r;
		assert(y);
		x->r = y->l;
		if (y->l) y->l->p = x;
		y->p = x->p;
		if (x->p) {
			if (x->p->l == x) {
				x->p->l = y;
			} else {
				x->p->r = y;
			}
		} else {
			root = y;
		}
		y->l = x;
		x->p = y;
	}

	void rotateRight(Node *x) {
		Node *y = x->l;
		assert(y);
		x->l = y->r;
		if (y->r) y->r->p = x;
		y->p = x->p;
		if (x->p) {
			if (x->p->l == x) {
				x->p->l = y;
			} else {
				x->p->r = y;
			}
		} else {
			root = y;
		}
		y->r = x;
		x->p = y;
	}
	void splay(Node *x) {
		while (x->p) {
			if (x->p->p == nullptr) {
				// zig
				if (x->p->l == x) {
					rotateRight(x->p);
				} else {
					rotateLeft(x->p);
				}
			} else if (x->p->l == x && x->p->p->l == x->p) {
				// zigzig
				rotateRight(x->p->p);
				rotateRight(x->p);
			} else if (x->p->r == x && x->p->p->r == x->p) {
				rotateLeft(x->p->p);
				rotateLeft(x->p);
			} else if (x->p->l == x && x->p->p->r == x->p) {
				// zigzag
				rotateRight(x->p);
				rotateLeft(x->p);
			} else if (x->p->r == x && x->p->p->l == x->p) {
				rotateLeft(x->p);
				rotateRight(x->p);
			}
		}
		root = x;
	}
	Node *find(int idx) {
		Node *x = root;
		while (x) {
			if (idx == size(x->l)) {
				break;
			} else if (size(x->l) < idx) {
				if (x->r) {
					idx -= size(x->l) + 1;
					x = x->r;
				} else {
					break;
				}
			} else {
				if (x->l) {
					x = x->l;
				} else {
					break;
				}
			}
		}
		if (x) {
			int sz = size(x->l);
			splay(x);
			if (sz == idx) {
				return x;
			} else {
				return nullptr;
			}
		} else {
			return nullptr;
		}
	}
	pair<splay_tree, splay_tree> split(int idx) {
		if (find(idx)) {
			Node *r = root->r;
			if (r) {
				r->p = nullptr;
			}
			root->r = nullptr;
			return {splay_tree(root), splay_tree(r)};
		} else {
			if (size(root->l) < idx) {
				Node *r = root->r;
				if (r) r->p = nullptr;
				root->r = nullptr;
				return {splay_tree(root), splay_tree(r)};
			} else {
				Node *l = root->l;
				if (l) l->p = nullptr;
				root->l = nullptr;
				return {splay_tree(l), splay_tree(root)};
			}
		}
	}
	static splay_tree merge(splay_tree left, splay_tree right) {
		if (!left.root) return right;
		if (!right.root) return left;
		Node *x = left.root;
		while (x->r) x = x->r;
		left.splay(x);
		left.root->r = right.root;
		right.root->p = left.root;
		return left;
	}

	void insert(T val) {
		if (!root) {
			root = new_node(val);
			return;
		}
		auto [left, right] = split(val);
		Node *new_val = new_node(val);
		*this = merge(merge(left, splay_tree(new_val)), right);
		splay(new_val);
	}
	void erase(T val) {
		if (find(val)) {
			splay_tree left(root->l), right(root->r);
			if (left.root) left.root->p = nullptr;
			if (right.root) right.root->p = nullptr;
			*this = merge(left, right);
		}
	}

};