#line 1 "testing/data-structures/SegmentTree.test.cpp"
// competitive-verifier: PROBLEM https://judge.yosupo.jp/problem/point_add_range_sum

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

#line 2 "data-structures/SegmentTree.h"

template<class T>
struct segtree {
	vector <T> seg;
	int n;
	T unit;
	function<T(T, T)> f;

	segtree(int n, T unit, function<T(T, T)> f) : n(n), unit(unit), f(f) {
		seg.assign(2 * n, unit);
	}

	segtree(int n, vector <T> &a, T unit, function<T(T, T)> f) : n(n), unit(unit), f(f), seg(2 * n, unit) {
		build(a);
	}

	segtree(vector <T> &a, T unit, function<T(T, T)> f) : n(a.size()), unit(unit), f(f), seg(2 * n, unit) {
		build(a);
	}

	void build(vector <T> &a) {
		for (int i = 0; i < n; ++i) {
			seg[i + n] = a[i];
		}
		for (int i = n - 1; i > 0; --i) {
			seg[i] = f(seg[i << 1], seg[i << 1 | 1]);
		}
	}

	void update(int i, T val) {
		for (seg[i += n] += val; i >>= 1;) {
			seg[i] = f(seg[i << 1], seg[i << 1 | 1]);
		}
	}

	void set(int i, T val) {
		for (seg[i += n] = val; i >>= 1;) {
			seg[i] = f(seg[i << 1], seg[i << 1 | 1]);
		}
	}

	T query(int l, int r) {
		T ra = unit, rb = unit;
		for (l += n, r += n; l < r; l >>= 1, r >>= 1) {
			if (l & 1) {
				ra = f(ra, seg[l++]);
			}
			if (r & 1) {
				rb = f(seg[--r], rb);
			}
		}
		return f(ra, rb);
	}
};
#line 93 "testing/data-structures/SegmentTree.test.cpp"

int solve() {
	int n, q;
	cin >> n >> q;
	segtree<ll> st(n, 0, [](auto a, auto b) { return a + b; });
	vector<ll> v(n);
	for (auto &v_i: v) {
		cin >> v_i;
	}
	st.build(v);
	while (q--) {
		int type, a, b;
		cin >> type >> a >> b;
		if (type == 0) {
			st.update(a, b);
		} else {
			cout << st.query(a, b) << '\n';
		}
	}

	return 0;
}

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);
	int T = 1;
//	cin >> T;
	while (T--) {
		solve();
	}
}
