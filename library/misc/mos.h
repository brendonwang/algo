#pragma once

struct mos {
	struct Query {
		int l, r, i;
		ll ord;
	};

	static ll hilbert_order(int x, int y, int pow = 21, int rotate = 0) {
		if (pow == 0) {
			return 0;
		}
		int hpow = 1 << (pow - 1);
		int seg = (x < hpow)
					  ? (
						  (y < hpow) ? 0 : 3
					  )
					  : (
						  (y < hpow) ? 1 : 2
					  );
		seg = (seg + rotate) & 3;
		const int rotateDelta[4] = {3, 0, 0, 1};
		int nx = x & (x ^ hpow), ny = y & (y ^ hpow);
		int nrot = (rotate + rotateDelta[seg]) & 3;
		ll subSquareSize = 1ll << (2 * pow - 2);
		ll ans = seg * subSquareSize;
		ll add = hilbert_order(nx, ny, pow - 1, nrot);
		ans += (seg == 1 || seg == 2) ? add : (subSquareSize - add - 1);
		return ans;
	}

	vector<Query> queries;

	void add_query(int l, int r, int i) {
		queries.push_back({l, r, i});
	}

	template<typename Add, typename Erase, typename Answer>
	auto process(Add add, Erase erase, Answer answer) {
		using T = decay_t<invoke_result_t<Answer&>>;

		for (auto& q: queries) {
			q.ord = hilbert_order(q.l, q.r);
		}

		sort(queries.begin(), queries.end(), [](const Query& a, const Query& b) {
			return a.ord < b.ord;
		});

		vector<T> ans(queries.size());
		int ql = 0, qr = -1;
		for (auto& [l, r, i, _]: queries) {
			while (l < ql) add(--ql);
			while (qr < r) add(++qr);
			while (l > ql) erase(ql++);
			while (qr > r) erase(qr--);
			ans[i] = answer();
		}
		return ans;
	}
};