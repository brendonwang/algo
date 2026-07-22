// NOTE: Current implementation does not work with generic f for now.
template <typename T, typename L, typename F, typename A, typename LA>
struct treap {
  struct Node {
    T v;
    T subtree_val;
    L lazy;
    bool rev = false;
    int sz = 1;
    Node *c[2] = {nullptr, nullptr};

    Node(const T &val, const L &lazy) : v(val), subtree_val(val), lazy(lazy) {}
  };

  inline T v(Node *x) { return (x == nullptr ? unit : x->v); }

  inline L lazy(Node *x) { return (x == nullptr ? lazy_unit : x->lazy); }

  inline bool rev(Node *x) { return (x == nullptr ? false : x->rev); }

  inline T subtree_val(Node *x) {
    return (x == nullptr ? unit : x->subtree_val);
  }

  inline int sz(Node *x) { return (x == nullptr ? 0 : x->sz); }

  inline void pull(Node *x) {
    if (!x)
      return;
    x->subtree_val = f(f(subtree_val(x->c[0]), x->v), subtree_val(x->c[1]));
    x->sz = 1 + sz(x->c[0]) + sz(x->c[1]);
  }

  inline void apply(Node *x, const L &lz, bool rev) {
    if (!x)
      return;
    if (lz != lazy_unit) {
      add(x->v, lz, 1);
      add(x->subtree_val, lz, x->sz);
      lazy_add(x->lazy, lz);
    }
    if (rev) {
      x->rev ^= rev;
      swap(x->c[0], x->c[1]);
    }
  }

  inline void propagate(Node *x) {
    if (!x || (x->lazy == lazy_unit && !x->rev))
      return;
    apply(x->c[0], x->lazy, x->rev);
    apply(x->c[1], x->lazy, x->rev);
    x->lazy = lazy_unit;
    x->rev = false;
  }

  inline treap new_root(Node *x) {
    return treap(unit, f, add, lazy_unit, lazy_add, x);
  }

  static uint64_t rng() {
    static uint64_t x = chrono::steady_clock::now().time_since_epoch().count();

    x += 0x9e3779b97f4a7c15ULL;
    uint64_t z = x;
    z = (z ^ (z >> 30)) * 0xbf58476d1ce4e5b9ULL;
    z = (z ^ (z >> 27)) * 0x94d049bb133111ebULL;
    return z ^ (z >> 31);
  }

  T unit;
  F f;
  A add;
  L lazy_unit;
  LA lazy_add;

  Node *root;

  template <class X = T, class Y = L,
            std::enable_if_t<std::is_same<X, Y>::value, int> = 0>
  treap(const T &unit, const F &f, const A &add, const LA &lazy_add,
        Node *r = nullptr)
      : f(f), unit(unit), add(add), lazy_unit(unit), lazy_add(lazy_add),
        root(r) {}

  treap(const T &unit, const F &f, const A &add, const L &lazy_unit,
        const LA &lazy_add, Node *r = nullptr)
      : f(f), unit(unit), add(add), lazy_unit(lazy_unit), lazy_add(lazy_add),
        root(r) {}
  Node *find(int idx) {
    Node *x = root, *last = nullptr;
    while (x) {
      propagate(x);

      last = x;
      if (sz(x->c[0]) == idx) {
        return x;
      }
      if (idx < sz(x->c[0]))
        x = x->c[0];
      else {
        idx -= sz(x->c[0]) + 1;
        x = x->c[1];
      }
    }
    return nullptr;
  }

  pair<Node*, Node*> split(Node* x, int idx) {
    if (!x)
      return {nullptr, nullptr};
    propagate(x);
    if (idx < sz(x->c[0])) {
      auto [l, r] = split(x->c[0], idx);
      x->c[0] = r;
      pull(x);
      return {l, x};
    } else {
      auto [l, r] = split(x->c[1], idx - sz(x->c[0]) - 1);
      x->c[1] = l;
      pull(x);
      return {x, r};
    }
  }

  Node* merge(Node* l, Node* r) {
    if (!l) return r;
    if (!r) return l;
    propagate(l);
    propagate(r);
    if (((__uint128_t)rng() * (sz(l) + sz(r)) >> 64) < sz(l)) {
      l->c[1] = merge(l->c[1], r);
      pull(l);
      return l;
    } else {
      r->c[0] = merge(l, r->c[0]);
      pull(r);
      return r;
    }
  }

  void insert(int idx, const T &val) {
    auto [l, r] = split(root, idx - 1);
    Node* x = new Node(val, lazy_unit);
    root = merge(l, merge(x, r));
    pull(root);
  }

  array<Node*, 3> split(int l, int r) {
    auto [left, midright] = split(l - 1);
    auto [mid, right] = midright.split(r - l);
    return {left, mid, right};
  }

  void erase(int idx) {
    auto [l, x, r] = split(idx, idx + 1);
    root = merge(l, r);
  }

  T query(int l, int r) {
    auto [left, mid, right] = split(l, r);
    T ans = subtree_val(mid);
    root = merge(left, merge(mid, right));
    return ans;
  }

  template <typename APPLY> void change(int idx, const T &val, APPLY apply) {
    Node *x = find(idx); // TODO: Check
    if (x) {
      propagate(x);
      apply(x->v, val);
      pull(x);
    }
  }

  void apply(int l, int r, const L &lz) {
    auto [left, mid, right] = split(l, r);
    if (mid) {
      apply(mid, lz, false);
    }
    root = merge(left, merge(mid, right));
  }

  void reverse(int l, int r) {
    auto [left, mid, right] = split(l, r);
    if (mid) {
      apply(mid, lazy_unit, true);
    }
    root = merge(left, merge(mid, right));
  }
};
