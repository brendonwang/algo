#pragma once

#include "../data-structures/segtree.h"

template<typename T = int>
struct hld {
	int n;
	vector<vector<int>> &adj;
	vector<int> parent, depth, heavy, head, pos;
	segtree<T> st;
	int t = 0, unit;
	function<T(T, T)> f;
	hld(vector<vector<T>> &G, T unit, function<T(T, T)> f)
			: n(G.size()), adj(G),
			  parent(n, -1), depth(n), heavy(n, -1),
			  head(n), pos(n), unit(unit), f(f), st(n, unit, f) {
		dfs(0);
		decompose(0, 0);
	}
	hld(vector<vector<int>> &G, vector<T> &vals, T unit, function<T(T, T)> f)
			: n(G.size()), adj(G),
			  parent(n, -1), depth(n), heavy(n, -1),
			  head(n), pos(n), unit(unit), f(f), st(n, unit, f) {
		dfs(0);
		decompose(0, 0);
		for (int i = 0; i < n; ++i) {
			st.set(pos[i], vals[i]);
		}
	}
	int dfs(int v) {
		int sz = 1, maxsz = 0;
		for (auto &u: adj[v]) {
			if (u == parent[v]) continue;
			parent[u] = v;
			depth[u] = depth[v] + 1;
			int childsz = dfs(u);
			if (childsz > maxsz) {
				heavy[v] = u;
				maxsz = childsz;
			}
			sz += childsz;
		}
		return sz;
	}
	void decompose(int v, int h) {
		head[v] = h;
		pos[v] = t++;
		if (heavy[v] != -1) {
			decompose(heavy[v], h);
		}
		for (auto &u: adj[v]) {
			if (u == parent[v] || u == heavy[v]) continue;
			decompose(u, u);
		}
	}

	template<typename F>
	void query_path(int u, int v, F op) {
		while (head[u] != head[v]) {
			if (depth[head[u]] < depth[head[v]]) swap(u, v);
			op(pos[head[u]], pos[u]);
			u = parent[head[u]];
		}
		if (depth[u] > depth[v]) swap(u, v);
		op(pos[u], pos[v]);
	}

	void update(int v, T val) {
		st.update(pos[v], val);
	}
	void set(int v, T val) {
		st.set(pos[v], val);
	}

	T query(int a, int b) {
		T res = unit;
		query_path(a, b, [&](auto x, auto y) {
			res = f(res, st.query(x, y + 1));
		});
		return res;
	}
};
