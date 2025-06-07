#pragma once

template<typename T>
struct fenwick {
	vector<T> tree;
	int n;

	fenwick(int n) : n(n), tree(n + 1, 0) {}

	void update(int i, T val) {
		for (++i; i <= n; i += i & -i) {
			tree[i] += val;
		}
	}

	T query(int i) {
		T sum = 0;
		for (++i; i > 0; i -= i & -i) {
			sum += tree[i];
		}
		return sum;
	}

	T query(int l, int r) {
		return query(r) - query(l - 1);
	}
};