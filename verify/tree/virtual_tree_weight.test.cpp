// PROBLEM SOURCE: loj.ac/p/3789
#define STANDALONE

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

#include "../../library/tree/virtual_tree_weight.h"

int solve_vtree() {
	int n;
	cin >> n;
	vector<vector<pll>> adj(n);
	for (int i = 0; i < n - 1; ++i) {
		int a, b, l;
		cin >> a >> b >> l;
		a--, b--;
		adj[a].push_back({b, l});
		adj[b].push_back({a, l});
	}
	int m;
	cin >> m;
	vector<vector<pll>> vadj(n);
	auto vtree = gen_vtree(adj, vadj, n);
	for (int i = 0; i < m; ++i) {
		int k;
		cin >> k;
		safe_map<int, ll> dp;
		vector<int> vals;
		for (int j = 0; j < k; ++j) {
			int h;
			cin >> h;
			h--;
			vals.push_back(h);
			dp[h] = inf;
		}
		vtree(vals);
		auto dfs1 = [&](int u, int p, auto &&self) -> void {
			for (auto &[v, w]: vadj[u]) {
				if (p != v) {
					self(v, u, self);
					dp[u] += min(dp[v], w);
				}
			}
		};
		dfs1(0, -1, dfs1);
		cout << dp[0] << '\n';
	}
	return 0;
}

int solve_brute() {
	int n;
	cin >> n;
	vector<vector<pll>> adj(n);
	for (int i = 0; i < n - 1; ++i) {
		int a, b, l;
		cin >> a >> b >> l;
		a--, b--;
		adj[a].push_back({b, l});
		adj[b].push_back({a, l});
	}
	int m;
	cin >> m;
	for (int i = 0; i < m; ++i) {
		int k;
		cin >> k;
		safe_map<int, ll> dp;
		vector<int> vals;
		for (int j = 0; j < k; ++j) {
			int h;
			cin >> h;
			h--;
			vals.push_back(h);
			dp[h] = inf;
		}
		auto dfs = [&](int u, int p, auto &&self) -> void {
			for (auto &[v, w]: adj[u]) {
				if (p != v) {
					self(v, u, self);
					dp[u] += min(dp[v], w);
				}
			}
		};
		dfs(0, -1, dfs);
		cout << dp[0] << '\n';
	}
	return 0;
}

struct random {
	mt19937_64 rng;

	random() = default;

	explicit random(int s) {
		seed(s);
	}

	ll randint(ll a, ll b) {
		return uniform_int_distribution(a, b)(rng);
	}

	ll randint(ll a) {
		return randint(0, a);
	}

	long double rand(long double a, long double b) {
		return uniform_real_distribution(a, b)(rng);
	}

	long double rand(long double a) {
		return rand(0, a);
	}

	long double rand() {
		return rand(0, 1);
	}

	void seed(ll s) {
		rng.seed(s);
	}

	template<typename T>
	void rand_shuffle(T l, T r) {
		shuffle(l, r, rng);
	}

	template<typename T>
	vector<T> rand_shuffle(vector<T> arr) {
		shuffle(arr.begin(), arr.end(), rng);
		return arr;
	}

	vector<array<int, 3>> gen_tree(int n) {
		vector<array<int, 3>> edges(n - 1);
		vector<int> ord(n);
		iota(begin(ord), end(ord), 0);
		rand_shuffle(begin(ord), end(ord));
		for (int i = 1; i < n; i++) {
			int p = ord[randint(i - 1)];
			edges[i - 1] = {min(p, ord[i]), max(p, ord[i]), (int) randint(1, 1e5)};
		}
		return edges;
	}

	string print_graph(const vector<array<int, 3>> &edges) {
		string final;
		for (auto &[a, b, c]: edges) {
			final += to_string(a + 1) + " " + to_string(b + 1) + " " + to_string(c) + "\n";
		}
		return final;
	}

	template<typename... Args>
	vector<int> gen_array(int n, Args... args) {
		vector<int> arr(n);
		for (int i = 0; i < n; ++i) {
			arr[i] = randint(args...);
		}
		return arr;
	}

	vector<int> gen_distinct_array(int n, int min, int max) {
		assert(max - min + 1 >= n);
		int prev = min - 1;
		vector<int> arr;
		for (int i = 0; i < n; ++i) {
			arr.push_back(prev = randint(prev + 1, max - (n - i + 1)));
		}
		return arr;
	}

	template<typename T>
	string print_vector(const vector<T> &a) {
		string final;
		for (auto &val: a) {
			final += to_string(val) + ' ';
		}
		return final;
	}
};


int gen(int seed) {
	struct random rng;
//	rng.seed(chrono::steady_clock::now().time_since_epoch().count());
	rng.seed(seed);
	int n = rng.randint(5, 10);
	cout << n << '\n';
	cout << rng.print_graph(rng.gen_tree(n));
	cout << 10 << '\n';
	for (int i = 0; i < 10; ++i) {
		int k = rng.randint(1, n);
		cout << k << " ";
		vector<int> possible(n);
		iota(possible.begin(), possible.end(), 1);
		for (int j = 1; j <= k; ++j) {
			int val = possible[rng.randint(0, possible.size() - 1)];
			possible.erase(find(possible.begin(), possible.end(), val));
			cout << val << " ";
		}
	}
	return 0;
}

string strip(const string &s) {
	auto start = s.begin();
	while (start != s.end() && isspace(*start)) start++;
	auto end = s.end();
	while (end != start && isspace(*(end - 1))) end--;
	return string(start, end);
}


int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);
	for (int rep = 1; rep <= 2; rep++) {
		stringstream input;
		streambuf *cinbuf = cin.rdbuf();
		streambuf *coutbuf = cout.rdbuf();

		// Capture test input from Generator::main
		cout.rdbuf(input.rdbuf());
		gen(rep);
		string val = input.str();

		// Run Brute solution using the same input
		stringstream inputBrute(val), outputBrute;
		cin.rdbuf(inputBrute.rdbuf());
		cout.rdbuf(outputBrute.rdbuf());
		solve_brute();

		// Run Code solution using the same input
		stringstream inputCode(val), outputCode;
		cin.rdbuf(inputCode.rdbuf());
		cout.rdbuf(outputCode.rdbuf());
		solve_vtree();

		// Restore original stream buffers
		cin.rdbuf(cinbuf);
		cout.rdbuf(coutbuf);

		string outBrute = strip(outputBrute.str());
		string outCode = strip(outputCode.str());
		assert(outBrute == outCode);
	}
	return 0;
}