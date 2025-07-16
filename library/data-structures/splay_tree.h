template<typename T, int MAXN = 1000000, typename lazy_tag = T>
struct splay_tree {
	struct Node {
		Node *l = nullptr, *r = nullptr, *p = nullptr;
		int sz = 1;
		T val, subtree;
		lazy_tag lazy;
		bool rev = false;
		Node() = default;
		template<typename Splay>
		inline void propagate(Splay *st) {
			if (st->add && lazy != st->lazy_unit) {
				st->add(val, lazy, 1);
				st->add(subtree, lazy, sz);
				if (l) st->lazy_add(l->lazy, lazy);
				if (r) st->lazy_add(r->lazy, lazy);
				lazy = st->lazy_unit;
			}
			if (rev) {
				swap(l, r);
				if (l) l->rev = !l->rev;
				if (r) r->rev = !r->rev;
				rev = false;
			}
		}

		template<typename Splay>
		inline void update(Splay *st) {
			sz = 1;
			subtree = val;
			if (l) {
				l->propagate(st);
				sz += l->sz;
				subtree = st->f(subtree, l->subtree);
			}
			if (r) {
				r->propagate(st);
				sz += r->sz;
				subtree = st->f(subtree, r->subtree);
			}
		}
	};

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
		x->update(this);
		y->update(this);
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
		x->update(this);
		y->update(this);
	}

	void splay(Node *x) {
		while (x->p) {
			x->p->propagate(this);
			if (x->p->p == nullptr) {
				// zig
				if (x->p->l == x) {
					rotateRight(x->p);
				} else {
					rotateLeft(x->p);
				}
			} else {
				x->p->p->propagate(this);
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
			x->propagate(this);
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
			root->update(this);
			return {splay_tree(unit, f, add, lazy_unit, lazy_add, root),
			        splay_tree(unit, f, add, lazy_unit, lazy_add, r)};
		} else {
			if (root) {
				if (size(root->l) < idx) {
					Node *r = root->r;
					if (r) r->p = nullptr;
					root->r = nullptr;
					root->update(this);
					return {splay_tree(unit, f, add, lazy_unit, lazy_add, root),
					        splay_tree(unit, f, add, lazy_unit, lazy_add, r)};
				} else {
					Node *l = root->l;
					if (l) l->p = nullptr;
					root->l = nullptr;
					root->update(this);
					return {splay_tree(unit, f, add, lazy_unit, lazy_add, l),
					        splay_tree(unit, f, add, lazy_unit, lazy_add, root)};
				}
			} else {
				return {splay_tree(unit, f, add, lazy_unit, lazy_add, nullptr),
				        splay_tree(unit, f, add, lazy_unit, lazy_add, nullptr)};
			}
		}
	}
	static splay_tree merge(splay_tree left, splay_tree right) {
		if (!left.root) return right;
		if (!right.root) return left;
		Node *x = left.root;
		x->propagate(&left);
		while (x->r) {
			x = x->r;
			x->propagate(&left);
		}
		left.splay(x);
		left.root->r = right.root;
		left.root->update(&left);
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
			mid.root->propagate(&mid);
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
			mid.root->propagate(&mid);
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