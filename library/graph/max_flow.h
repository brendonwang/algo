#pragma once

template<typename T>
T max_flow(vector<vector<int>> &adj, vector<vector<T>> &capacity, int source, int sink) {
	int n = adj.size();
	int flow = 0;
	while (true) {
		bool found = false;
		vector<int> prev(n, -2);
		queue<int> q;

		prev[source] = -1;
		q.push(source);

		while (!q.empty()) {
			int v = q.front();
			q.pop();
			if (v == sink) {
				found = true;
				while (true) {
					int p = prev[v];
					if (p == -1) {
						break;
					}
					capacity[p][v]--;
					capacity[v][p]++;
					v = p;
				}
				flow++;
				break;
			}
			for (int u: adj[v]) {
				if (prev[u] == -2 && capacity[v][u]) {
					prev[u] = v;
					q.push(u);
				}
			}
		}
		if (!found) {
			break;
		}
	}
	return flow;
}