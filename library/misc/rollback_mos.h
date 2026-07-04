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

	template<typename Add, typename A, typename Rollback, typename S>
	auto process(Add add, A answer, Rollback rollback, S snapshot) {
		return process(add, add, answer, rollback, snapshot);
	}

	template<typename L, typename R, typename A, typename Rollback, typename S>
	auto process(L add_left, R add_right, A answer, Rollback rollback, S snapshot) {
		using T = decay_t<invoke_result_t<A&>>;

		sort(queries.begin(), queries.end(), [](const Query& a, const Query& b) {
			if (a.l / B == b.l / B) {
				return a.r < b.r;
			}
			return a.l / B < b.l / B;
		});

		vector<T> ans(queries.size());
		for (auto& [l, r, i]: queries) {
			if (r - l + 1 <= B) {
				auto time = snapshot();
				for (int j = l; j <= r; j++) {
					add_right(j);
				}
				ans[i] = answer();
				rollback(time);
			}
		}
		int cur_block = -1;
		int ql = 0, qr = -1;
		int empty = snapshot();
		for (auto& [l, r, i]: queries) {
			if (r - l + 1 <= B) continue;
			if (l / B != cur_block) {
				rollback(empty);
				cur_block = l / B;
				ql = (cur_block + 1) * B;
				qr = ql - 1;
			}
			while (qr < r) {
				add_right(++qr);
			}
			auto time = snapshot();
			for (int j = ql - 1; j >= l; --j) {
				add_left(j);
			}
			ans[i] = answer();
			rollback(time);
		}
		return ans;
	}
};
