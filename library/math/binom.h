#pragma once

#include "../number-theory/mint.h"

template<auto &M = MOD>
struct binomial {
	using T = mint<M>;
	int MAXN;
	vector<T> fact;
	vector<T> inv;

	binomial() = default;

	binomial(int maxN) : MAXN(maxN) {
		fact.resize(MAXN + 1);
		inv.resize(MAXN + 1);
		int N = min(MAXN, (int) M) - 1;
		fact[0] = 1;
		for (int i = 1; i <= N; i++) {
			fact[i] = fact[i - 1] * i;
		}
		inv[N] = fact[N].inv(fact[N]);
		for (int i = N - 1; i >= 0; i--) {
			inv[i] = inv[i + 1] * (i + 1);
		}
	}

	T C(int n, int k) {
		if (n < k || n < 0 || k < 0 || n >= MAXN) {
			return 0;
		}
		return fact[n] * inv[k] * inv[n - k];
	}
};