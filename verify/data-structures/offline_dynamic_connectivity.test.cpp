#define PROBLEM "https://judge.yosupo.jp/problem/dynamic_graph_vertex_add_component_sum"

#include <bits/stdc++.h>

#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>

/* NAMESPACE */

using namespace std;
using namespace __gnu_pbds;

/* HASH */

struct custom_hash {
	static uint64_t splitmix64(uint64_t x) {
		// http://xorshift.di.unimi.it/splitmix64.c
		x += 0x9e3779b97f4a7c15;
		x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9;
		x = (x ^ (x >> 27)) * 0x94d049bb133111eb;
		return x ^ (x >> 31);
	}

	size_t operator()(uint64_t x) const {
		static const uint64_t FIXED_RANDOM = chrono::steady_clock::now().time_since_epoch().count();
		return splitmix64(x + FIXED_RANDOM);
	}
};

template<class K, class V> using safe_map = unordered_map<K, V, custom_hash>;
template<class K, class V> using safe_ht = gp_hash_table<K, V, custom_hash>;
template<class K> using safe_set = unordered_set<K, custom_hash>;
template<class K> using safe_htset = gp_hash_table<K, null_type, custom_hash>;

/* CLASSES */

using ll = long long;
using pii = pair<int, int>;
template<class T> using pp = pair<T, T>;
using pll = pp<ll>;
template<class T> using ordered_set = tree<T, null_type, less<T>, rb_tree_tag, tree_order_statistics_node_update>;
template<class T> using greater_pq = priority_queue<T, vector<T>, greater<>>;
template<class T> using V = vector<T>;
using vi = V<int>;
using vvi = V<vi>;
using vvvi = V<vvi>;
using vll = V<ll>;

/* FUNCTIONS */

template<class T, class U>
T max(T a, U b) {
	return (a > b) ? a : b;
}

template<class T, class U>
T min(T a, U b) {
	return (a < b) ? a : b;
}

template<class T>
T power(T x, T y) {
	T res = 1;
	for (T i = 0; i < y; i++) {
		res *= x;
	}
	return res;
}



/* MACROS */
#define clearall(arr) memset(arr, 0, sizeof arr)
#define clearn(arr, n) memset(arr, 0, n * sizeof arr[0])
#define resetall(arr) memset(arr, -1, sizeof arr)
#define resetn(arr, n) memset(arr, -1, n * sizeof arr[0])
#define YESNO(condition) cout << ((condition) ? "YES" : "NO")
#define sfunc(a, b, c) a = c((a), (b))
#define smin(a, b) sfunc((a), (b), min)
#define smax(a, b) sfunc((a), (b), max)
#define ALL(x) begin(x), end(x)
#define SZ(a) (int)(a).size()
#define readall(arr, n) for (int i = 0; i < n; i++) cin >> arr[i]
#define printall(arr, n) for (int i = 0; i < n; i++) cout << arr[i] << " "

/* CONSTANTS */
const int inf = 2e9;
const ll infl = 4e18;
const ll MOD = 1e9 + 7;
const ll MAXN = 2e5 + 5;

#define int ll

#include "../../library/data-structures/offline_dynamic_connectivity.h"

int solve() {
	int n, q;
	cin >> n >> q;
	query_tree<ll> qt(n, q);
	vector<int> a(n);
	for (auto &a_i: a) {
		cin >> a_i;
	}
	map<pii, vector<int>> start;
	map<pii, vector<int>> end;
	vector<array<int, 2>> queries(q);
	for (int i = 0; i < q; ++i) {
		int t;
		cin >> t;
		if (t == 0) {
			int u, v;
			cin >> u >> v;
			start[{min(u, v), max(u, v)}].push_back(i);
		} else if (t == 1) {
			int u, v;
			cin >> u >> v;
			end[{min(u, v), max(u, v)}].push_back(i);
		} else if (t == 2) {
			int v, x;
			cin >> v >> x;
			qt.add_query({1, {v, x}}, i, q - 1);
		} else {
			int v;
			cin >> v;
			queries[i] = {t, v};
		}
	}
	for (auto &[query, starts]: start) {
		for (int i = 0; i < starts.size(); ++i) {
			qt.add_query({0, query}, starts[i], (end[query].size() <= i ? q - 1 : end[query][i] - 1));
		}
	}
	vector<int> sums = a;
	vector<ll> ans;
	vector<pll> hist;
	auto add = [&](DSU &dsu, pair<int, pii> q) {
		if (q.first == 0) {
			auto [u, v] = q.second;
			u = dsu.find(u), v = dsu.find(v);
			if (dsu.e[u] > dsu.e[v]) {
				swap(u, v);
			}
			if (!dsu.connected(u, v)) {
				dsu.hist.push_back({sums[u], sums[u]});
				sums[u] += sums[v];
				dsu.unite(u, v);
			}
		} else {
			assert(q.first == 1);
			auto [v, x] = q.second;
			v = dsu.find(v);
			dsu.hist.push_back({sums[v], sums[v]});
			sums[v] += x;
		}
	};
	auto query = [&](DSU &dsu, int idx) {
		if (queries[idx][0] == 3) {
			ans.push_back(sums[dsu.find(queries[idx][1])]);
		}
	};
	qt.dfs(add, query);
	for (int i = 0; i < ans.size(); ++i) {
		cout << ans[i] << '\n';
	}
	return 0;
}

signed main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);
	int T = 1;
//	cin >> T;
	while (T--) {
		solve();
	}
}