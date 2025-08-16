#pragma once

struct DSU {
	vector<int> e;
	vector<pair<int &, int>> hist;
	explicit DSU(int n) : e(n, -1) {}
	int find(int x) { return e[x] < 0 ? x : find(e[x]); }
	int size(int x) { return -e[find(x)]; }
	inline int time() const { return hist.size(); }
	void rollback(int t) {
		while (time() > t) {
			hist.back().first = hist.back().second;
			hist.pop_back();
		}
	}
	bool unite(int a, int b) {
		a = find(a), b = find(b);
		if (a == b) return true;
		if (e[a] > e[b]) swap(a, b);
		hist.push_back({e[a], e[a]});
		hist.push_back({e[b], e[b]});
		e[a] += e[b];
		e[b] = a;
		return false;
	}
	bool connected(int a, int b) { return find(a) == find(b); }
};