#define PROBLEM "https://judge.yosupo.jp/problem/line_add_get_min"

#include <bits/stdc++.h>

using namespace std;
using ll = long long;
using pii = pair<int, int>;

#define YESNO(condition) cout << ((condition) ? "YES" : "NO")
#define sfunc(a, b, c) ((a) = c((a), (b)))
#define smin(a, b) sfunc((a), (b), min)
#define smax(a, b) sfunc((a), (b), max)
#define sz(a) (int)(a).size()
#define readall(arr, n) for (int i = 0; i < n; i++) cin >> (arr)[i]
#define printall(arr, n) for (int i = 0; i < n; i++) cout << (arr)[i] <<

const int inf = 2e9;
const ll infl = 4e18;
const ll MOD = 998244353;
const ll MAXN = 2e5 + 5;

#include "../../library/data-structures/li_chao_tree.h"

int solve() {
	int n, q;
	cin >> n >> q;
	lct<__int128, (__int128) 1e20> hull;
	for (int i = 0; i < n; ++i) {
		ll a, b;
		cin >> a >> b;
		hull.insert(a, b);
	}
	for (int i = 0; i < q; ++i) {
		int type;
		cin >> type;
		if (type == 0) {
			ll a, b;
			cin >> a >> b;
			hull.insert(a, b);
		} else {
			ll p;
			cin >> p;
			cout << (ll) hull.query(p) << '\n';
		}
	}
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