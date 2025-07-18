template<typename T, int MAXN = 1e6>
struct treap {
	struct Node {
		Node *l = nullptr, *r = nullptr;
		T v;
		int priority;
		Node() = default;
		Node(T val) : v(val) {}
	};

	mt19937 rng{(unsigned int) (std::chrono::steady_clock::now().time_since_epoch().count())};


	static inline Node buff[MAXN];
	static inline int cnt = 0;

	Node *new_node(int val) {
		assert(cnt < MAXN);
		buff[cnt].v = val;
		buff[cnt].priority = rng();
		return &buff[cnt++];
	}

	Node *root;

	treap(Node *r = nullptr) : root(r) {}

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
			if (size(x->l) <= idx) {
				auto [l, r] = split(idx - size(x->l) - 1, x->r);
				x->r = l;
				return {x, r};
			} else {
				auto [l, r] = split(idx, x->l);
				x->l = r;
				return {l, x};
			}
		} else {
			return {nullptr, nullptr};
		}
	}
	Node *merge(Node *l, Node *r) {
		if (!l) return r;
		if (!r) return l;
		if (l->priority < r->priority) {
			swap(l, r);
		}
		l->r = merge(l->r, r);
		return l;
	}

	void insert(T val) {
		if (!root) {
			root = new_node(val);
			return;
		}
		auto [left, right] = split(val);
		Node *new_val = new_node(val);
		root = merge(merge(left, new_val), right);
	}
	void erase(T val) {
		if (find(val)) {
			root = merge(root->l, root->r);
		}
	}

};