#include <bits/stdc++.h>

using namespace std;
using ll = long long;
using pii = pair<int, int>;

#define YESNO(condition) cout << ((condition) ? "YES" : "NO")
#define sfunc(a, b, c) ((a) = c((a), (b)))
#define smin(a, b) sfunc((a), (b), min)
#define smax(a, b) sfunc((a), (b), max)
#define SZ(a) (int)(a).size()
#define readall(arr, n) for (int i = 0; i < n; i++) cin >> (arr)[i]
#define printall(arr, n) for (int i = 0; i < n; i++) cout << (arr)[i]

template<typename T, typename S> ostream &operator<<(ostream &os, const pair<T, S> &p) { return os << "(" << p.first << ", " << p.second << ")"; } template<typename C, typename T = decay<decltype(*begin( declval<C>()))>, typename enable_if<!is_same<C, string>::value>::type * = nullptr> ostream &operator<<(ostream &os, const C &c) { bool f = true; os << "["; for (const auto &x: c) { if (!f) os << ", "; f = false; os << x; } return os << "]"; }
template<typename T> void debug(string s, T x) { cerr << "\033[1;35m" << s << "\033[0;32m = \033[33m" << x << "\033[0m\n"; }
template<typename T, typename... Args> void debug(string s, T x, Args... args) { for (int i = 0, b = 0; i < (int) s.size(); i++) if (s[i] == '(' || s[i] == '{') b++; else if (s[i] == ')' || s[i] == '}') b--; else if (s[i] == ',' && b == 0) { cerr << "\033[1;35m" << s.substr(0, i) << "\033[0;32m = \033[33m" << x << "\033[31m | "; debug(s.substr(s.find_first_not_of(' ', i + 1)), args...); break; } }
template<typename T> std::vector<T> vectorize(T *a, int n) { std::vector<T> res; for (int i = 0; i < n; ++i) { res.push_back(a[i]); } return res; } template<typename T, typename... Sizes> auto vectorize(T *a, int n, Sizes... sizes) { std::vector<decltype(vectorize(a[0], sizes...))> res; for (int i = 0; i < n; ++i) { res.push_back(vectorize(a[i], sizes...)); } return res; }

#ifdef LOCAL
#define DEBUG(...) debug(#__VA_ARGS__, __VA_ARGS__)
#else
#define DEBUG(...) 42
#endif

const int inf = 2e9;
const ll infl = 4e18;
const int MOD = 998244353;
const int MAXN = 2e5 + 5;

int solve() {
  return 0;
}

int main() {
  cin.tie(0)->sync_with_stdio(0);
  int T = 1;
  //	cin >> T;
  while (T--) {
  	solve();
  }
  return 0;
}
