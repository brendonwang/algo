#pragma once

template<int B = 450>
struct mos {
	struct Query {
		int l, r, i;
	};

	vector<Query> queries;

	void add_query(int l, int r, int i) {
		queries.push_back({l, r, i});
	}

	template<typename Erase, typename A, typename Rollback, typename S>
	auto process(Erase erase, A answer, Rollback rollback, S snapshot) {
		using T = decay_t<invoke_result_t<A&>>;

		sort(queries.begin(), queries.end(), [](const Query& a, const Query& b) {
			if (a.l / B == b.l / B) {
				return a.r < b.r;
			}
			return a.l / B > b.l / B;
		});

		vector<T> ans(queries.size());
		int cur_block = -1;
		int ql = 0, qr = -1;
		auto full = snapshot();
		for (auto& [l, r, i]: queries) {
			if (r - l + 1 <= B) continue;
			if (l / B != cur_block) {
				rollback(full);
				cur_block = l / B;
				ql = (cur_block) * B;
				qr = ql - 1;
				for (int j = 0; j < ql - 1; ++j) {
					erase(j);
				}
			}
			while (qr > r) {
				erase(qr--);
			}
			auto time = snapshot();
			for (int j = cur_block; j < l; --j) {
				erase(j);
			}
			ans[i] = answer();
			rollback(time);
		}

		// rollback(full);
		return ans;
	}
};
