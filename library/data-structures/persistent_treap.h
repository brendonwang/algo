template<typename T, int MAXN = 1e6, typename lazy_tag = T>
struct persistent_treap {
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
				if (l) {
					Node *l_copy = tr->copy_node(l);
					this->l = l_copy;
					tr->lazy_add(l_copy->lazy, lazy);
				}
				if (r) {
					Node *r_copy = tr->copy_node(r);
					this->r = r_copy;
					tr->lazy_add(r->lazy, lazy);
				}
				lazy = tr->lazy_unit;
			}
			if (rev) {
				swap(l, r);
				if (l) {
					Node *l_copy = tr->copy_node(l);
					this->l = l_copy;
					l_copy->rev = !l_copy->rev;
				}
				if (r) {
					Node *r_copy = tr->copy_node(r);
					this->r = r_copy;
					r_copy->rev = !r_copy->rev;
				}
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
		if (cnt >= MAXN) {
			Node *x = new Node();
			x->val = val, x->subtree = val, x->lazy = lazy_unit, x->priority = rng();
			return x;
		}
		assert(cnt < MAXN);
		buff[cnt].val = val;
		buff[cnt].subtree = val;
		buff[cnt].lazy = lazy_unit;
		buff[cnt].priority = rng();
		return &buff[cnt++];
	}
	inline Node *copy_node(Node *x) {
		if (!x) return nullptr;
		if (cnt >= MAXN) {
			Node *new_ptr = new Node();
			*new_ptr = *x;
			return new_ptr;
		}
		assert(cnt < MAXN);
		Node *new_ptr = &buff[cnt++];
		*new_ptr = *x;
		return new_ptr;
	}


	vector<Node *> roots{nullptr};
	using combine_fn = T(*)(T, T);
	using apply_fn = void (*)(T &, lazy_tag, int);
	using lazy_apply_fn = void (*)(lazy_tag &, lazy_tag);
	combine_fn f;
	apply_fn add;
	static inline void default_lazy_add(lazy_tag &a, lazy_tag b) { a += b; }
	lazy_apply_fn lazy_add;
	T unit;
	lazy_tag lazy_unit;

	persistent_treap(T unit_, combine_fn f_, apply_fn add_, lazy_tag lazy_unit_,
	                 lazy_apply_fn lazy_add_ = default_lazy_add) : f(f_), add(add_), lazy_add(lazy_add_), unit(unit_),
	                                                               lazy_unit(lazy_unit_) {}
	persistent_treap(T unit_, combine_fn f_, apply_fn add_, lazy_apply_fn lazy_add_ = default_lazy_add)
			: f(f_), add(add_), lazy_add(lazy_add_), unit(unit_), lazy_unit(unit_) {}
	persistent_treap(T unit_, combine_fn f_, Node *r = nullptr) : f(f_), unit(unit_) {}

	inline int size(Node *x) {
		if (x) {
			return x->sz;
		} else {
			return 0;
		}
	}

	Node *find(int idx, int t = -1) {
		if (t == -1) t = (int) roots.size() - 1;
		Node *x = roots[t];
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
	pair<Node *, Node *> split(int idx, Node *x, bool copy = true) {
		if (x) {
			x->propagate(this);
			Node *cur = copy ? copy_node(x) : x;
			if (size(x->l) <= idx) {
				auto [l, r] = split(idx - size(x->l) - 1, cur->r, copy);
				cur->r = l;
				cur->update(this);
				return {cur, r};
			} else {
				auto [l, r] = split(idx, cur->l, copy);
				cur->l = r;
				cur->update(this);
				return {l, cur};
			}
		} else {
			return {nullptr, nullptr};
		}
	}
	Node *merge(Node *l, Node *r, bool copy = true) {
		if (!l) return r;
		if (!r) return l;
		l->propagate(this);
		r->propagate(this);
		if (l->priority < r->priority) {
			Node *cur = copy ? copy_node(r) : r;
			cur->l = merge(l, cur->l, copy);
			cur->update(this);
			return cur;
		} else {
			Node *cur = copy ? copy_node(l) : l;
			cur->r = merge(cur->r, r, copy);
			cur->update(this);
			return cur;
		}
	}

	void insert(int idx, T val, int t = -1, int cur = -1) {
		if (t == -1) t = (int) roots.size() - 1;
		if (cur == -1) {
			roots.push_back(nullptr);
			cur = (int) roots.size() - 1;
		}
		if (!roots[t]) {
			roots[cur] = new_node(val);
			return;
		}
		auto [left, right] = split(idx, roots[t]);
		Node *new_val = new_node(val);
		roots[cur] = merge(merge(left, new_val), right);
	}
	Node *erase(int idx, Node *x) {
		if (!x) return nullptr;
		x->propagate(this);
		Node *x_copy = copy_node(x);
		if (size(x->l) == idx) {
			return merge(x_copy->l, x_copy->r);
		} else if (size(x->l) < idx) {
			x_copy->r = erase(idx - size(x->l) - 1, x_copy->r);
		} else {
			x_copy->l = erase(idx, x_copy->l);
		}
		x_copy->update(this);
		return x_copy;
	}
	void erase(int idx, int t = -1) {
		if (t == -1) t = (int) roots.size() - 1;
		roots.push_back(erase(idx, roots[t]));
	}
	template<typename F>
	void change(int idx, T val, F apply, int t = -1) {
		if (t == -1) t = (int) roots.size() - 1;
		auto [left, right] = split(idx - 1, roots[t]);
		auto [mid, rest] = split(0, right);
		if (mid) {
			mid->val = apply(mid->val, val);
			mid->update(this);
		}
		roots.push_back(merge(merge(left, mid, false), rest, false));
	}
	template<typename F>
	void change(int l, int r, lazy_tag val, F apply, int t = -1) {
		if (t == -1) t = (int) roots.size() - 1;
		auto [left, right] = split(l - 1, roots[t]);
		auto [mid, rest] = split(r - l, right);
		if (mid) {
			apply(mid->lazy, val);
			mid->propagate(this);
			mid->update(this);
		}
		roots.push_back(merge(merge(left, mid, false), rest, false));
	}
	void update(int idx, T val, int t = -1) {
		change(idx, val, [](T a, T b) { return a + b; }, t);
	}
	void set(int idx, T val, int t = -1) {
		change(idx, val, [](T a, T b) { return b; }, t);
	}
	void update(int l, int r, lazy_tag val, int t = -1) {
		change(l, r, val, [](lazy_tag &a, lazy_tag b) { a += b; }, t);
	}
	void set(int l, int r, lazy_tag val, int t = -1) {
		change(l, r, val, [](lazy_tag &a, lazy_tag b) { a = b; }, t);
	}

	void apply(int l, int r, lazy_tag val, int t = -1) {
		change(l, r, val, lazy_add, t);
	}

	T query(int l, int r, int t = -1) {
		if (t == -1) t = (int) roots.size() - 1;
		auto [left, right] = split(l - 1, roots[t], false);
		auto [mid, rest] = split(r - l, right, false);
		T ans = mid ? mid->subtree : unit;
		roots[t] = merge(merge(left, mid, false), rest, false);
		return ans;
	}
	void reverse(int l, int r, int t = -1) {
		if (t == -1) t = (int) roots.size() - 1;
		auto [left, right] = split(l - 1, roots[t]);
		auto [mid, rest] = split(r - l, right);
		if (mid) {
			mid->rev = true;
			mid->propagate(this);
			mid->update(this);
		}
		roots.push_back(merge(merge(left, mid, false), rest, false));
	}
};