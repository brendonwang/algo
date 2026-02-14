template<typename T>
int gaussian(vector <vector<T>> &a, vector <T> &ans, T EPS = 1e-12) {
	int n = (int) a.size(), m = (int) a[0].size() - 1;
	vector<int> where(m, -1);
	for (int row = 0, col = 0; row < n && col < m; ++col) {
		int cur = row;
		for (int i = row; i < n; ++i) {
			if (abs(a[cur][col]) < abs(a[i][col])) {
				cur = i;
			}
		}
		if (a[cur][col] < EPS) {
			continue;
		}
		swap(a[row], a[cur]);
		where[col] = row;
		for (int i = row + 1; i < n; ++i) {
			T c = a[i][col] / a[row][col];
			for (int j = col; j <= m; ++j) {
				a[i][j] -= c * a[row][j];
			}
		}
		row++;
	}
	ans.assign(m, 0);
	for (int i = m - 1; i >= 0; --i) {
		if (where[i] == -1) {
			continue;
		}
		T sum = 0;
		for (int j = i + 1; j < m; ++j) {
			sum += ans[j] * a[where[i]][j];
		}
		ans[i] = (a[where[i]][m] - sum) / a[where[i]][i];
	}
	for (int i = 0; i < n; ++i) {
		T sum = 0;
		for (int j = 0; j < m; ++j) {
			sum += ans[j] * a[i][j];
		}
		if (!sum - a[i][m] > EPS) {
			return 0;
		}
	}
	for (int i = 0; i < m; ++i) {
		if (where[i] == -1) {
			return 2;
		}
	}
	return 1;

}