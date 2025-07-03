#define PROBLEM "https://judge.yosupo.jp/problem/pow_of_formal_power_series"

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

template<class T>
T nxt() {
	T x;
	cin >> x;
	return x;
}

/* MACROS */
#define clearall(arr) memset((arr), 0, sizeof (arr))
#define clearn(arr, n) memset((arr), 0, n * sizeof (arr)[0])
#define resetall(arr) memset((arr), -1, sizeof (arr))
#define resetn(arr, n) memset((arr), -1, n * sizeof (arr)[0])
#define YESNO(condition) cout << ((condition) ? "YES" : "NO")
#define sfunc(a, b, c) ((a) = c((a), (b)))
#define smin(a, b) sfunc((a), (b), min)
#define smax(a, b) sfunc((a), (b), max)
#define ALL(x) begin((x)), end((x))
#define SZ(a) (int)(a).size()
#define readall(arr, n) for (int i = 0; i < n; i++) cin >> (arr)[i]
#define printall(arr, n) for (int i = 0; i < n; i++) cout << (arr)[i] << " "
#define printalldel(arr, n, del) for (int i = 0; i < n; i++) cout << (arr)[i] << del
#define mx_val(arr) (*max_element(ALL(arr)))

/* DEBUG TEMPLATE*/
template<typename T, typename S>
ostream &operator<<(ostream &os, const pair<T, S> &p) { return os << "(" << p.first << ", " << p.second << ")"; }

template<typename C, typename T = decay<decltype(*begin(
		declval<C>()))>, typename enable_if<!is_same<C, string>::value>::type * = nullptr>
ostream &operator<<(ostream &os, const C &c) {
	bool f = true;
	os << "[";
	for (const auto &x: c) {
		if (!f) os << ", ";
		f = false;
		os << x;
	}
	return os << "]";
}

template<typename T>
void debug(string s, T x) { cerr << "\033[1;35m" << s << "\033[0;32m = \033[33m" << x << "\033[0m\n"; }

template<typename T, typename... Args>
void debug(string s, T x, Args... args) {
	for (int i = 0, b = 0; i < (int) s.size(); i++)
		if (s[i] == '(' || s[i] == '{') b++;
		else if (s[i] == ')' || s[i] == '}') b--;
		else if (s[i] == ',' && b == 0) {
			cerr << "\033[1;35m" << s.substr(0, i) << "\033[0;32m = \033[33m" << x << "\033[31m | ";
			debug(s.substr(s.find_first_not_of(' ', i + 1)), args...);
			break;
		}
}

template<typename T>
std::vector<T> vectorize(T *a, int n) {
	std::vector<T> res;
	for (int i = 0; i < n; ++i) {
		res.push_back(a[i]);
	}
	return res;
}

template<typename T, typename... Sizes>
auto vectorize(T *a, int n, Sizes... sizes) {
	std::vector<decltype(vectorize(a[0], sizes...))> res;
	for (int i = 0; i < n; ++i) {
		res.push_back(vectorize(a[i], sizes...));
	}
	return res;
}

#ifdef LOCAL
#define DEBUG(...) debug(#__VA_ARGS__, __VA_ARGS__)
#else
#define DEBUG(...) 42
#endif

/* CONSTANTS */
const int inf = 2e9;
const ll infl = 4e18;
const ll MOD = 998244353;
const ll MAXN = 2e5 + 5;

#include "../../library/polynomial/polymod.h"

int solve() {
	ll n, m;
	cin >> n >> m;
	vector<ll> a(n);
	for (auto &a_i: a) {
		cin >> a_i;
	}
	poly ap(a);
	ap.pow_inplace(m);
//	ap = ap.pow(m);
	for (int i = 0; i < n; ++i) {
		cout << ap[i] << " ";
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
	return 0;
}