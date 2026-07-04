template<typename T>
struct compress {
  vector<T> vals;

  void add(const T& x) {
    vals.push_back(x);
  }

  void build() {
    sort(vals.begin(), vals.end());
    vals.erase(unique(vals.begin(), vals.end()), vals.end());
  }

  int get(const T& x) {
    return lower_bound(vals.begin(), vals.end(), x) - vals.begin();
  }

  T operator[](int i) const {
		return vals[i];
	}

	int size() const {
		return (int) vals.size();
	}
};