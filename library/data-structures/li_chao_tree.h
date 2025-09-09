template<typename T = int, T INF = numeric_limits<T>::max()>
struct lct {
	struct line {
		T m, b;
		T operator()(T x) {
			return x * m + b;
		}
		line(T m, T b) : m(m), b(b) {};
	};

	struct Node {
		Node *l = nullptr, *r = nullptr;
		line v;
		Node(line v) : v(v) {}
	};

	Node *root = nullptr;

	void insert(Node *&x, line cur, T l, T r) {
		if (!x) {
			x = new Node(cur);
			return;
		}
		T mid = l + (r - l) / 2;
		bool comp_l = cur(l) < (x->v)(l);
		bool comp_mid = cur(mid) < (x->v)(mid);
		if (comp_mid) {
			swap(x->v, cur);
		}
		if (l == r) {
			return;
		} else if (comp_l != comp_mid) {
			insert(x->l, cur, l, mid);
		} else {
			insert(x->r, cur, mid + 1, r);
		}
	}
	void insert(T m, T b, T l = -INF, T r = INF) {
		insert(root, line(m, b), l, r);
	}
	T query(Node *x, T cur, T l, T r) {
		if (!x) {
			return INF;
		}
		if (l == r) {
			return (x->v)(cur);
		}
		T mid = l + (r - l) / 2;
		if (cur <= mid) {
			return min((x->v)(cur), query(x->l, cur, l, mid));
		} else {
			return min((x->v)(cur), query(x->r, cur, mid + 1, r));
		}
	}
	T query(T x, T l = -INF, T r = INF) {
		return query(root, x, l, r);
	}
};