// NOTE: Current implementation does not work with generic f for now.
template <typename T, typename L, typename F, typename A, typename LA>
struct treap {
  
  static uint64_t rng() {
    static uint64_t x = chrono::steady_clock::now().time_since_epoch().count();

    x += 0x9e3779b97f4a7c15ULL;
    uint64_t z = x;
    z = (z ^ (z >> 30)) * 0xbf58476d1ce4e5b9ULL;
    z = (z ^ (z >> 27)) * 0x94d049bb133111ebULL;
    return z ^ (z >> 31);
  }
 
  struct Node {
    T v;
    T subtree_val;
    L lazy;
    bool rev = false;
    int sz = 1;
    Node *c[2] = {nullptr, nullptr};
    uint64_t priority;

    Node(const T &val, const L &lazy) : v(val), subtree_val(val), lazy(lazy) {
      priority = rng();
    }
  };

  inline T v(Node *x) { return (x == nullptr ? unit : x->v); }

  inline L lazy(Node *x) { return (x == nullptr ? lazy_unit : x->lazy); }

  inline bool rev(Node *x) { return (x == nullptr ? false : x->rev); }

  inline T subtree_val(Node *x) {
    return (x == nullptr ? unit : x->subtree_val);
  }

  inline int sz(Node *x) { return (x == nullptr ? 0 : x->sz); }

  inline void pull(Node *x) {
    if (!x) return;
    x->subtree_val = f(f(subtree_val(x->c[0]), x->v), subtree_val(x->c[1]));
    x->sz = 1 + sz(x->c[0]) + sz(x->c[1]);
  }

  inline void apply(Node *x, const L &lz, bool rev) {
    if (!x) return;
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

  inline void push(Node *x) {
    if (!x || (x->lazy == lazy_unit && !x->rev)) return;
    apply(x->c[0], x->lazy, x->rev);
    apply(x->c[1], x->lazy, x->rev);
    x->lazy = lazy_unit;
    x->rev = false;
  }

  T unit;
  F f;
  A add;
  L lazy_unit;
  LA lazy_add;

  Node *root;

  treap(const T &unit, const F &f, const A &add, const L &lazy_unit,
        const LA &lazy_add, Node *r = nullptr)
      : f(f), unit(unit), add(add), lazy_unit(lazy_unit), lazy_add(lazy_add),
        root(r) {}
  Node *find(int idx) {
    Node *x = root;
    while (x) {
      push(x);
      
      const int left_sz = sz(x->c[0]);

      if (left_sz == idx) {
        return x;
      }
      if (idx < left_sz) {
        x = x->c[0];
      } else {
        idx -= left_sz + 1;
        x = x->c[1];
      }
    }
    return nullptr;
  }

  pair<Node*, Node*> split(Node* x, int idx) {
    if (!x) return {nullptr, nullptr};
    push(x);
    const int left_sz = sz(x->c[0]);
    if (idx < left_sz) {
      auto [l, r] = split(x->c[0], idx);
      x->c[0] = r;
      pull(x);
      return {l, x};
    } else {
      auto [l, r] = split(x->c[1], idx - left_sz - 1);
      x->c[1] = l;
      pull(x);
      return {x, r};
    }
  }

  Node* merge(Node* l, Node* r) {
    if (!l) return r;
    if (!r) return l;
    if (l->priority < r->priority) {
      push(l);
      l->c[1] = merge(l->c[1], r);
      pull(l);
      return l;
    } else {
      push(r);
      r->c[0] = merge(l, r->c[0]);
      pull(r);
      return r;
    }
  }

  void insert(int idx, const T &val) {
    auto [l, r] = split(root, idx - 1);
    Node* x = new Node(val, lazy_unit);
    root = merge(l, merge(x, r));
  }

  array<Node*, 3> split(int l, int r) {
    auto [left, midright] = split(root, l - 1);
    auto [mid, right] = split(midright, r - l);
    return {left, mid, right};
  }

  void erase(int idx) {
    auto [l, x, r] = split(idx, idx);
    root = merge(l, r);
  }

  void build(const vector<T>& a) {
    
  }

  T query(int l, int r) {
    auto [left, mid, right] = split(l, r);
    T ans = subtree_val(mid);
    root = merge(left, merge(mid, right));
    return ans;
  }


  template<typename APPLY>
  void change(Node* x, int idx, const T &val, APPLY &apply) {
    if (!x) return;

    push(x);

    const int left_sz = sz(x->c[0]);
    
    if (idx < left_sz) {
      change(x->c[0], idx, val, apply);
    } else if (idx == left_sz) {
      apply(x->v, val);
    } else {
      change(x->c[1], idx - left_sz- 1, val, apply);
    }

    pull(x);
  }
 
  template<typename APPLY> 
  void change(int idx, const T &val, APPLY &&apply) {
    change(root, idx, val, apply);
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
