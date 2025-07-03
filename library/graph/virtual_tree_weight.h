vector<int> depth(n);
int maxk = 32 - __builtin_clz(n);
vector<vector<int>> bin_lift(n, vector<int>(maxk, -1));
vector<vector<int>> min_edge(n, vector<int>(maxk, inf));
vector<int> tin(n);
int t = 0;
auto dfs = [&](int u, int p, auto &&self) -> void {
	tin[u] = t++;
	bin_lift[u][0] = p;
	for (int k = 1; k < maxk; ++k) {
		if (bin_lift[u][k - 1] != -1) {
			bin_lift[u][k] = bin_lift[bin_lift[u][k - 1]][k - 1];
			min_edge[u][k] = min(min_edge[u][k - 1], min_edge[bin_lift[u][k - 1]][k - 1]);
		} else {
			break;
		}
	}
	for (auto &[v, w]: adj[u]) {
		if (p != v) {
			depth[v] = depth[u] + 1;
			min_edge[v][0] = w;
			self(v, u, self);
		}
	}
};
dfs(0, -1, dfs);
auto lca = [&](int a, int b) {
	if (depth[a] > depth[b]) swap(a, b);
	int dist = depth[b] - depth[a];
	for (int i = 0; i < maxk; ++i) if (dist & 1 << i) b = bin_lift[b][i];
	if (a == b) return a;
	for (int i = maxk - 1; i >= 0; --i)
		if (bin_lift[a][i] != -1 && bin_lift[a][i] != bin_lift[b][i])
			a = bin_lift[a][i], b = bin_lift[b][i];
	return bin_lift[a][0];
};
auto min_w = [&](int a, int b) {
	if (depth[a] > depth[b]) {
		swap(a, b);
	}
	int dist = depth[b] - depth[a];
	int weight = inf;
	for (int i = 0; i < maxk; ++i) {
		if (dist & 1 << i) {
			smin(weight, min_edge[b][i]);
			b = bin_lift[b][i];
		}
	}
	return weight;
};
vector<vector<pii>> vadj(n);
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
	std::sort(vals.begin(), vals.end(), [&](auto a, auto b) { return tin[a] < tin[b]; });
	for (int j = 1; j < k; ++j) { vals.push_back(lca(vals[j - 1], vals[j])); }
	vals.push_back(0);
	std::sort(vals.begin(), vals.end(), [&](auto a, auto b) { return tin[a] < tin[b]; });
	vals.erase(unique(vals.begin(), vals.end()), vals.end());
	for (auto &val: vals) {
	vadj[val].clear();
	}
	for (int i = 1; i < (int) vals.size(); i++) {
	int p = lca(vals[i - 1], vals[i]);
	vadj[p].push_back({vals[i], min_w(p, vals[i])});
	}
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
