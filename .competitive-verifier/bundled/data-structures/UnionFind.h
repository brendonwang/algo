#line 2 "data-structures/UnionFind.h"

struct DSU {
	vector<int> e;

	explicit DSU(int n) : e(n, -1) {}

	int find(int x) {
		return e[x] < 0 ? x : e[x] = find(e[x]);
	}

	int size(int x) {
		return -e[find(x)];
	}

	void unite(int a, int b) {
		a = find(a), b = find(b);
		if (a == b) {
			return;
		}
		if (e[a] > e[b]) {
			swap(a, b);
		}
		e[a] += e[b];
		e[b] = a;
	}

	bool connected(int a, int b) {
		return find(a) == find(b);
	}
};
