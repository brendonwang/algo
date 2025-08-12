#define PROBLEM "https://judge.yosupo.jp/problem/persistent_range_affine_range_sum"

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
const ll MOD = 998244353;
const ll MAXN = 2e5 + 5;

#include "../../library/data-structures/persistent_treap.h"
#include "../../library/math/mint.h"

struct affine {
	mint<> x, y;
	affine operator+=(affine rhs) {
		x += rhs.x;
		y += rhs.y;
		return *this;
	}
	friend affine operator+(affine lhs, affine rhs) {
		return lhs += rhs;
	}
	bool operator==(const affine &rhs) const {
		return x == rhs.x && y == rhs.y;
	}
	bool operator!=(const affine &rhs) const {
		return !(rhs == *this);
	}

};

template<typename T, int MAXN = 1e6, typename lazy_tag = T>
struct range_copy : persistent_treap<T, MAXN, lazy_tag> {
	using persistent_treap<T, MAXN, lazy_tag>::persistent_treap;

	void restore(int l, int r, int t = -1, int new_t = -1) {
		if (t == -1) t = (int) this->roots.size() - 1;
		if (new_t == -1) new_t = (int) this->roots.size() - 1;
		auto [tleft, tright] = this->split(l - 1, this->roots[t]);
		auto [tmid, trest] = this->split(r - l, tright);
		auto [new_tleft, new_tright] = this->split(l - 1, this->roots[new_t]);
		auto [new_tmid, new_trest] = this->split(r - l, new_tright);
		this->roots.push_back(this->merge(this->merge(tleft, new_tmid), trest));
	}
};

int solve() {
	int n, q;
	cin >> n >> q;
	range_copy<mint<>, (int) 1e7, affine> tr(0, [](auto a, auto b) { return a + b; },
	                                         [](auto &a, auto b, int sz) {
		                                         a = (a * b.x + b.y * sz);
	                                         }, affine{1, 0}, [](auto &a, auto b) {
				a.x *= b.x;
				a.y = a.y * b.x + b.y;
			});
	for (int i = 0; i < n; ++i) {
		int v;
		cin >> v;
		tr.insert(i, v, 0, 0);
	}
	while (q--) {
		int type;
		cin >> type;
		if (type == 0) {
			int k, l, r, b, c;
			cin >> k >> l >> r >> b >> c;
			k++;
			tr.apply(l, r - 1, affine{b, c}, k);
		} else if (type == 1) {
			int k, s, l, r;
			cin >> k >> s >> l >> r;
			k++, s++;
			tr.restore(l, r - 1, k, s);
		} else {
			int k, l, r;
			cin >> k >> l >> r;
			k++;
			tr.roots.push_back(tr.roots[k]);
			cout << tr.query(l, r - 1, k) << '\n';
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