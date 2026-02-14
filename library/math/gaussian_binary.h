template<int N = 20>
int gaussian(vector<bitset<N>> a, bitset<N> b, vector<bitset<N>> &ans, int n, int m) {
	for (int i = 0; i < n; ++i) a[i][m] = b[i];
	vector<int> where(m, -1);
	for (int row = 0, col = 0; row < n && col < m; ++col) {
		for (int i = row; i < n; ++i) {
			if (a[i][col] != 0) {
				swap(a[row], a[i]);
				break;
			}
		}
		if (a[row][col] == 0) continue;
		where[col] = row;
		for (int i = row + 1; i < n; ++i) if (a[i][col]) a[i] ^= a[row];
		row++;
	}
	vector<int> free;
	for (int i = 0; i < m; ++i) { if (where[i] == -1) { free.push_back(i); }}
	ans.assign(free.size() + 1, bitset<N>(0));
	for (int col = 0; col <= free.size(); ++col) {
		if (col) { ans[col][free[col - 1]] = 1; }
		for (int i = m - 1; i >= 0; --i) {
			if (where[i] == -1) { continue; }
			ans[col][i] = (col == 0 ? a[where[i]][m] : 0) ^ ((ans[col] & a[where[i]]).count() & 1);
		}
	}
	for (int i = 0; i < n; ++i) {
		if (((ans[0] & a[i]).count() & 1) != a[i][m]) {
			ans.clear();
			return -1;
		}
	}
	return (int) free.size();
}