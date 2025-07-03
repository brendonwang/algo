auto lca(vector<vector<int>> &adj, int n) {
	vector<int> depth(n);
	int maxk = 32 - __builtin_clz(n);
	vector<vector<int>> bin_lift(n, vector<int>(maxk, -1));
	auto dfs = [&](int u, int p, auto &&self) -> void {
		bin_lift[u][0] = p;
		for (int k = 1; k < maxk; ++k) {
			if (bin_lift[u][k - 1] != -1) {
				bin_lift[u][k] = bin_lift[bin_lift[u][k - 1]][k - 1];
			} else {
				break;
			}
		}
		for (auto &v: adj[u]) {
			if (p != v) {
				depth[v] = depth[u] + 1;
				self(v, u, self);
			}
		}
	};
	dfs(0, -1, dfs);
	auto lca = [depth, maxk, bin_lift](int a, int b) {
		if (depth[a] > depth[b]) swap(a, b);
		int dist = depth[b] - depth[a];
		for (int i = 0; i < maxk; ++i) if (dist & 1 << i) b = bin_lift[b][i];
		if (a == b) return a;
		for (int i = maxk - 1; i >= 0; --i)
			if (bin_lift[a][i] != -1 && bin_lift[a][i] != bin_lift[b][i])
				a = bin_lift[a][i], b = bin_lift[b][i];
		return bin_lift[a][0];
	};
	return lca;
}