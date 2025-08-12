template<typename T, typename U>
pair<vector<U>, vector<T>> dijkstra(const vector<vector<pair<T, U>>> &adj, T start) {
	int n = adj.size();
	vector<U> dist(n, numeric_limits<U>::max());
	vector<T> par(n, -1);
	priority_queue<pair<U, T>, vector<pair<U, T>>, greater<>> pq;
	pq.push({dist[start] = 0, start});
	while (!pq.empty()) {
		auto [cur_dist, u] = pq.top();
		pq.pop();
		if (cur_dist != dist[u]) continue;
		for (auto [v, w]: adj[u]) {
			if (cur_dist + w < dist[v]) {
				par[v] = u;
				pq.push({dist[v] = cur_dist + w, v});
			}
		}
	}
	return {dist, par};
}