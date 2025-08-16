#pragma once

#include "rollback_dsu.h"

template<typename T = int>
struct query_tree {
	DSU dsu;
	int n, q;
	vector<vector<pair<int, pii>>> queries;
	explicit query_tree(int n, int q) : dsu(n), n(n), q(q), queries(4 * q) {};
	void add_query(pair<int, pii> query, int l, int r, int x, int xl, int xr) {
		if (xr < l || xl > r) return;
		if (l <= xl && xr <= r) {
			queries[x].push_back(query);
			return;
		}
		int mid = (xl + xr) / 2;
		add_query(query, l, r, 2 * x + 1, xl, mid);
		add_query(query, l, r, 2 * x + 2, mid + 1, xr);
	}
	void add_query(pair<int, pii> query, int l, int r) {
		add_query(query, l, r, 0, 0, q - 1);
	}

	template<typename ADD, typename QUERY>
	void dfs(const ADD &add, const QUERY &query, int x, int xl, int xr) {
		int t = dsu.time();
		for (int i = 0; i < queries[x].size(); ++i) {
			add(dsu, queries[x][i]);
		}
		if (xl == xr) {
			query(dsu, xl);
			dsu.rollback(t);
			return;
		}
		int mid = (xl + xr) / 2;
		dfs(add, query, 2 * x + 1, xl, mid);
		dfs(add, query, 2 * x + 2, mid + 1, xr);
		dsu.rollback(t);
	}
	template<typename ADD, typename QUERY>
	void dfs(const ADD &add, const QUERY &query) {
		dfs(add, query, 0, 0, q - 1);
	}
};