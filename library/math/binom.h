#pragma once

#include "../number-theory/mint.h"

template<auto &M = MOD>
struct binomial {
	using T = mint<M>;
	int MAXN;
	static inline vector<T> fact{T(1)};
	static inline vector<T> inv{T(1)};

	binomial() = default;

	explicit binomial(int maxN) : MAXN(maxN) {
		// only extend if needed
		int prev = (int) fact.size() - 1;
		int N = min(MAXN, M - 1);
		if (N > prev) {
			fact.resize(N + 1);
			inv.resize(N + 1);
			for (int i = prev + 1; i <= N; ++i) {
				fact[i] = fact[i - 1] * i;
			}
			inv[N] = modinv(fact[N]);
			for (int i = N - 1; i > prev; --i) {
				inv[i] = inv[i + 1] * (i + 1);
			}
		}
	}

	T C(int n, int k) {
		if (n < k || n < 0 || k < 0 || n >= MAXN) {
			return 0;
		}
		return fact[n] * inv[k] * inv[n - k];
	}
};