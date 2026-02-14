template<typename T>
int gaussian(vector<vector<T>> a, vector<T> b, vector<vector<T>> &ans) {
	int n = (int) a.size(), m = (int) a[0].size();
	for (int i = 0; i < n; ++i) {
		a[i].push_back(b[i]);
	}
	vector<int> where(m, -1);
	for (int row = 0, col = 0; row < n && col < m; ++col) {
		for (int i = row; i < n; ++i) {
			if (a[i][col] != 0) {
				swap(a[row], a[i]);
				break;
			}
		}
		if (a[row][col] == 0) {
			continue;
		}
		where[col] = row;
		for (int i = row + 1; i < n; ++i) {
			T c = a[i][col] / a[row][col];
			for (int j = col; j <= m; ++j) {
				a[i][j] -= c * a[row][j];
			}
		}
		row++;
	}
	vector<int> free;
	for (int i = 0; i < m; ++i) {
		if (where[i] == -1) {
			free.push_back(i);
		}
	}
	ans.assign(free.size() + 1, vector<T>(m, 0));
	for (int col = 0; col <= free.size(); ++col) {
		if (col) {
			ans[col][free[col - 1]] = 1;
		}
		for (int i = m - 1; i >= 0; --i) {
			if (where[i] == -1) {
				continue;
			}
			T sum = 0;
			for (int j = i + 1; j < m; ++j) {
				sum += ans[col][j] * a[where[i]][j];
			}
			ans[col][i] = ((col == 0 ? a[where[i]][m] : 0) - sum) / a[where[i]][i];
		}
	}
	for (int i = 0; i < n; ++i) {
		T sum = 0;
		for (int j = 0; j < m; ++j) {
			sum += ans[0][j] * a[i][j];
		}
		if (sum - a[i][m] != 0) {
			ans.clear();
			return -1;
		}
	}
	return free.size();
}