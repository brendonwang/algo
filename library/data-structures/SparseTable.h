#pragma once

// @formatter:off
template<class T>
struct SparseTable {
	vector<vector<T>> jmp; T unit; function<T(T, T)> f; SparseTable(const vector <T> &v, T unit = 2e9, function<T(T, T)> f = [](T a, T b) { return min(a, b); }) : jmp(1, v), unit(unit), f(f) { int n = int(v.size()); for (int j = 1; j <= 31 - __builtin_clz(n); ++j) { jmp.push_back(vector<T>(n, unit)); for (int i = 0; i + (1 << j) <= n; ++i) { jmp[j][i] = f(jmp[j - 1][i], jmp[j - 1][i + (1 << (j - 1))]); } } } T query(int l, int r) { assert(l < r); if (l == r) { return unit; } int sz = 31 - __builtin_clz(r - l); return f(jmp[sz][l], jmp[sz][r - (1 << sz)]); }
};
// @formatter:on
