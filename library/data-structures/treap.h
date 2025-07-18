template<typename T, int MAXN = 1e6, typename lazy_tag = T>
struct treap {
	struct Node {
		Node *l = nullptr, *r = nullptr;
		T val, subtree;
		unsigned int priority;
		lazy_tag lazy;
		int sz = 1;
		bool rev = false;
		Node() = default;
		template<typename Treap>
		inline void propagate(Treap *tr) {
			if (lazy != tr->lazy_unit) {
				tr->add(val, lazy, 1);
				tr->add(subtree, lazy, sz);
				if (l) tr->lazy_add(l->lazy, lazy);
				if (r) tr->lazy_add(r->lazy, lazy);
				lazy = tr->lazy_unit;
			}
			if (rev) {
				swap(l, r);
				if (l) l->rev = !l->rev;
				if (r) r->rev = !r->rev;
				rev = false;
			}
		}

		template<typename Treap>
		inline void update(Treap *tr) {
			sz = 1;
			subtree = val;
			if (l) {
				l->propagate(tr);
				sz += l->sz;
				subtree = tr->f(subtree, l->subtree);
			}
			if (r) {
				r->propagate(tr);
				sz += r->sz;
				subtree = tr->f(subtree, r->subtree);
			}
		}
	};

	mt19937 rng{(unsigned int) (std::chrono::steady_clock::now().time_since_epoch().count())};

	static inline Node buff[MAXN];
	static inline int cnt = 0;

	inline Node *new_node(T val) {
		assert(cnt < MAXN);
		buff[cnt].val = val;
		buff[cnt].subtree = val;
		buff[cnt].lazy = lazy_unit;
		buff[cnt].priority = rng();

		return &buff[cnt++];
	}

	using combine_fn = T(*)(T, T);
	using apply_fn = void (*)(T &, lazy_tag, int);
	using lazy_apply_fn = void (*)(lazy_tag &, lazy_tag);
	combine_fn f;
	apply_fn add;
	static inline void default_lazy_add(lazy_tag &a, lazy_tag b) { a += b; }
	lazy_apply_fn lazy_add;

	T unit;
	lazy_tag lazy_unit;
	Node *root;

	treap(T unit_, combine_fn f_, apply_fn add_, lazy_tag lazy_unit_, lazy_apply_fn lazy_add_ = default_lazy_add,
	      Node *r = nullptr) : f(f_), add(add_), lazy_add(lazy_add_), unit(unit_), lazy_unit(lazy_unit_), root(r) {}
	treap(T unit_, combine_fn f_, apply_fn add_, lazy_apply_fn lazy_add_ = default_lazy_add, Node *r = nullptr)
			: f(f_), add(add_), lazy_add(lazy_add_), unit(unit_), lazy_unit(unit_), root(r) {}
	treap(T unit_, combine_fn f_, Node *r = nullptr) : f(f_), unit(unit_), root(r) {}

	inline int size(Node *x) {
		if (x) {
			return x->sz;
		} else {
			return 0;
		}
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
			if (sz == idx) {
				return x;
			} else {
				return nullptr;
			}
		} else {
			return nullptr;
		}
	}
	pair<Node *, Node *> split(int idx, Node *x) {
		if (x) {
			x->propagate(this);
			if (size(x->l) <= idx) {
				auto [l, r] = split(idx - size(x->l) - 1, x->r);
				x->r = l;
				x->update(this);
				return {x, r};
			} else {
				auto [l, r] = split(idx, x->l);
				x->l = r;
				x->update(this);
				return {l, x};
			}
		} else {
			return {nullptr, nullptr};
		}
	}
	Node *merge(Node *l, Node *r) {
		if (!l) return r;
		if (!r) return l;
		l->propagate(this);
		r->propagate(this);
		if (l->priority < r->priority) {
			r->l = merge(l, r->l);
			r->update(this);
			return r;
		} else {
			l->r = merge(l->r, r);
			l->update(this);
			return l;
		}
	}

	void insert(int idx, T val) {
		if (!root) {
			root = new_node(val);
			return;
		}
		auto [left, right] = split(idx, root);
		Node *new_val = new_node(val);
		root = merge(merge(left, new_val), right);
	}
	Node *erase(int idx, Node *x) {
		if (!x) return nullptr;
		x->propagate(this);
		if (size(x->l) == idx) {
			return merge(x->l, x->r);
		} else if (size(x->l) < idx) {
			x->r = erase(idx - size(x->l) - 1, x->r);
		} else {
			x->l = erase(idx, x->l);
		}
		x->update(this);
		return x;
	}
	void erase(int idx) {
		root = erase(idx, root);
	}
	template<typename F>
	void change(int idx, T val, F apply) {
		auto [left, right] = split(idx - 1, root);
		auto [mid, rest] = split(0, right);
		if (mid) {
			mid->val = apply(mid->val, val);
			mid->update(this);
		}
		root = merge(merge(left, mid), rest);
	}
	template<typename F>
	void change(int l, int r, lazy_tag val, F apply) {
		auto [left, right] = split(l - 1, root);
		auto [mid, rest] = split(r - l, right);
		if (mid) {
			apply(mid->lazy, val);
			mid->propagate(this);
			mid->update(this);
		}
		root = merge(merge(left, mid), rest);
	}
	void update(int idx, T val) {
		change(idx, val, [](T a, T b) { return a + b; });
	}
	void set(int idx, T val) {
		change(idx, val, [](T a, T b) { return b; });
	}
	void update(int l, int r, lazy_tag val) {
		change(l, r, val, [](lazy_tag &a, lazy_tag b) { a += b; });
	}
	void set(int l, int r, lazy_tag val) {
		change(l, r, val, [](lazy_tag &a, lazy_tag b) { a = b; });
	}

	void apply(int l, int r, lazy_tag val) {
		change(l, r, val, lazy_add);
	}

	T query(int l, int r) {
		auto [left, right] = split(l - 1, root);
		auto [mid, rest] = split(r - l, right);
		T ans = mid ? mid->subtree : unit;
		root = merge(merge(left, mid), rest);
		return ans;
	}
	void reverse(int l, int r) {
		auto [left, right] = split(l - 1, root);
		auto [mid, rest] = split(r - l, right);
		if (mid) {
			mid->rev = true;
			mid->propagate(this);
			mid->update(this);
		}
		root = merge(merge(left, mid), rest);
	}
};

