#define PROBLEM "https://judge.yosupo.jp/problem/point_set_range_composite"

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

#include "../../library/data-structures/segtree.h"
#include "../../library/math/mint.h"

struct line {
	mint<> a, b;
	mint<> operator()(int x) const {
		return a * x + b;
	}
};

int solve() {
	int n, q;
	cin >> n >> q;
	vector<line> lines(n);
	for (int i = 0; i < n; ++i) {
		int a, b;
		cin >> a >> b;
		lines[i] = {a, b};
	}
	segtree st(lines, {1, 0}, [&](const line &y, const line &x) -> line { return {x.a * y.a, x.a * y.b + x.b}; });
	for (int i = 0; i < q; ++i) {
		int t;
		cin >> t;
		if (t == 0) {
			int p, c, d;
			cin >> p >> c >> d;
			st.set(p, {c, d});
		} else {
			int l, r, x;
			cin >> l >> r >> x;
			cout << st.query(l, r)(x) << '\n';
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