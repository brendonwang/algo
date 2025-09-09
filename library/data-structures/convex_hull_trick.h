
template<typename T = long long>
struct cht {
	struct line {
		T m, b;
		friend bool check(line &l1, line &l2, line &l3) {
			// returns if intersectX(l1, l2) < intersectX(l1, l3)
			return (l2.b - l1.b) * (l1.m - l3.m) < (l3.b - l1.b) * (l1.m - l2.m);
		}
		T operator()(T x) {
			return m * x + b;
		}
	};
	deque<line> lines;
	void add(T m, T b) {
		line l = {m, b};
		while (lines.size() >= 2 && check(lines[lines.size() - 2], lines.back(), l)) {
			lines.pop_back();
		}
		lines.push_back(l);
	}
	T query(T x) {
		while (lines.size() >= 2 && lines[0](x) < lines[1](x)) {
			lines.pop_front();
		}
		return lines.front()(x);
	}
};