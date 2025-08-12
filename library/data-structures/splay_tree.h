template<typename T, int MAXN = 1000000, typename lazy_tag = T>
struct splay_tree {
	struct Node {
		Node *l = nullptr, *r = nullptr, *p = nullptr;
		int sz = 1;
		T val, subtree;
		lazy_tag lazy;
		bool rev = false;
		Node() = default;
	};

	inline void propagate(Node *x) {
		if (x->lazy != lazy_unit) {
			add(x->val, x->lazy, 1);
			add(x->subtree, x->lazy, x->sz);
			if (x->l) lazy_add(x->l->lazy, x->lazy);
			if (x->r) lazy_add(x->r->lazy, x->lazy);
			x->lazy = lazy_unit;
		}
		if (x->rev) {
			swap(x->l, x->r);
			if (x->l) x->l->rev = !x->l->rev;
			if (x->r) x->r->rev = !x->r->rev;
			x->rev = false;
		}
	}

	inline void update(Node *x) {
		x->sz = 1;
		x->subtree = x->val;
		if (x->l) {
			propagate(x->l);
			x->sz += x->l->sz;
			x->subtree = f(x->subtree, x->l->subtree);
		}
		if (x->r) {
			propagate(x->r);
			x->sz += x->r->sz;
			x->subtree = f(x->subtree, x->r->subtree);
		}
	}

	static inline Node buff[MAXN];
	static inline int cnt = 0;
	Node *new_node(T val) {
		buff[cnt].val = val;
		buff[cnt].subtree = val;
		buff[cnt].lazy = lazy_unit;
		return &buff[cnt++];
	}

	using combine_fn = T(*)(T, T);
	using apply_fn = void (*)(T &, lazy_tag, int);
	using lazy_apply_fn = void (*)(lazy_tag &, lazy_tag);
	combine_fn f;
	apply_fn add;
	lazy_apply_fn lazy_add;
	static inline void default_lazy_add(lazy_tag &a, lazy_tag b) { a += b; }

	T unit;
	lazy_tag lazy_unit;
	Node *root;

	splay_tree(T unit_,
	           combine_fn f_,
	           apply_fn add_,
	           lazy_tag lazy_unit_,
	           lazy_apply_fn lazy_add_ = default_lazy_add,
	           Node *r = nullptr)
			: f(f_), add(add_), lazy_add(lazy_add_),
			  unit(unit_), lazy_unit(lazy_unit_), root(r) {}
	splay_tree(T unit_,
	           combine_fn f_,
	           apply_fn add_,
	           lazy_apply_fn lazy_add_ = default_lazy_add,
	           Node *r = nullptr)
			: f(f_), add(add_), lazy_add(lazy_add_),
			  unit(unit_), lazy_unit(unit_), root(r) {}
	splay_tree(T unit_, combine_fn f_, Node *r = nullptr) : f(f_), unit(unit_), root(r) {}

	inline int size(Node *x) {
		if (x) {
			return x->sz;
		} else {
			return 0;
		}
	}

	void rotateLeft(Node *x) {
		Node *y = x->r;
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
		update(x);
		update(y);
	}

	void rotateRight(Node *x) {
		Node *y = x->l;
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
		update(x);
		update(y);
	}

	void splay(Node *x) {
		while (x->p) {
			propagate(x->p);
			if (x->p->p == nullptr) {
				// zig
				if (x->p->l == x) {
					rotateRight(x->p);
				} else {
					rotateLeft(x->p);
				}
			} else {
				propagate(x->p->p);
				if (x->p->l == x && x->p->p->l == x->p) {
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
		}
		root = x;
	}
	Node *find(int idx) {
		Node *x = root;
		while (x) {
			propagate(x);
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
			update(root);
			return {splay_tree(unit, f, add, lazy_unit, lazy_add, root),
			        splay_tree(unit, f, add, lazy_unit, lazy_add, r)};
		} else {
			if (root) {
				if (size(root->l) < idx) {
					Node *r = root->r;
					if (r) r->p = nullptr;
					root->r = nullptr;
					update(root);
					return {splay_tree(unit, f, add, lazy_unit, lazy_add, root),
					        splay_tree(unit, f, add, lazy_unit, lazy_add, r)};
				} else {
					Node *l = root->l;
					if (l) l->p = nullptr;
					root->l = nullptr;
					update(root);
					return {splay_tree(unit, f, add, lazy_unit, lazy_add, l),
					        splay_tree(unit, f, add, lazy_unit, lazy_add, root)};
				}
			} else {
				return {splay_tree(unit, f, add, lazy_unit, lazy_add, nullptr),
				        splay_tree(unit, f, add, lazy_unit, lazy_add, nullptr)};
			}
		}
	}
	splay_tree merge(splay_tree left, splay_tree right) {
		if (!left.root) return right;
		if (!right.root) return left;
		Node *x = left.root;
		propagate(x);
		while (x->r) {
			x = x->r;
			propagate(x);
		}
		left.splay(x);
		left.root->r = right.root;
		update(left.root);
		right.root->p = left.root;
		return left;
	}

	void insert(int idx, T val) {
		if (!root) {
			root = new_node(val);
			return;
		}
		auto [left, right] = split(idx);
		Node *new_val = new_node(val);
		*this = merge(merge(left, splay_tree(unit, f, add, lazy_unit, lazy_add, new_val)), right);
		splay(new_val);
	}


	void erase(int idx) {
		if (find(idx)) {
			splay_tree left(unit, f, add, lazy_unit, lazy_add, root->l), right(unit, f, add, lazy_unit, lazy_add,
			                                                                   root->r);
			if (left.root) left.root->p = nullptr;
			if (right.root) right.root->p = nullptr;
			*this = merge(left, right);
		}
	}

	template<typename F>
	void change(int idx, T val, F apply) {
		Node *x = find(idx);
		if (x) {
			x->val = apply(x->val, val);
		}
	}
	template<typename F>
	void change(int l, int r, lazy_tag val, F apply) {
		auto [left, right] = split(l - 1);
		auto [mid, rest] = right.split(r - l);
		// apply operation on to mid
		if (mid.root) {
			mid.root->lazy = apply(mid.root->lazy, val);
			propagate(mid.root);
		}
		*this = merge(merge(left, mid), rest);
	}
	void update(int idx, T val) {
		change(idx, val, [](T a, T b) { return a + b; });
	}
	void set(int idx, T val) {
		change(idx, val, [](T a, T b) { return b; });
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

	void reverse(int l, int r) {
		auto [left, right] = split(l - 1);
		auto [mid, rest] = right.split(r - l);
		// apply operation on to mid
		if (mid.root) {
			mid.root->rev = true;
			propagate(mid.root);
		}
		*this = merge(merge(left, mid), rest);
	}

	T query(int l, int r) {
		auto [left, right] = split(l - 1);
		auto [mid, rest] = right.split(r - l);
		T ans = mid.root ? mid.root->subtree : unit;
		*this = merge(merge(left, mid), rest);
		return ans;
	}

};