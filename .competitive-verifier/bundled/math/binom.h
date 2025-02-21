#line 2 "math/binom.h"

template<typename T>
struct binomial {

	int MOD, MAXN;
	vector<T> fact;
	vector<T> inv;

	binomial() = default;

	binomial(int MOD, int MAXN) : MOD(MOD), MAXN(MAXN) {
		fact.resize(MAXN + 1);
		inv.resize(MAXN + 1);
		int N = min(MAXN, MOD) - 1;
		fact[0] = 1;
		for (int i = 1; i <= N; i++) {
			fact[i] = (T) (1LL * fact[i - 1] * i % MOD);
		}
		inv[N] = pow(fact[N], MOD - 2);
		for (int i = N - 1; i >= 0; i--) {
			inv[i] = (T) (1LL * inv[i + 1] * (i + 1) % MOD);
		}
	}

	T pow(T a, int b) {
		T res = 1;
		while (b) {
			if (b & 1) res = (T) (1LL * res * a % MOD);
			a = (T) (1LL * a * a % MOD);
			b >>= 1;
		}
		return res;
	}

	T C(int n, int k) {
		if (n < k || n < 0 || k < 0 || n >= MAXN) return 0;
		return (T) (1LL * fact[n] * inv[k] % MOD * inv[n - k] % MOD);
	}
};
