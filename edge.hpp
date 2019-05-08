#pragma once
#include <vector>

using std::vector;

class node;
class edge {
  node * _src;
  node * _dst;
  int _cap;
  int _flow = 0;
  edge * _back;


  public:
  edge() : edge(nullptr, nullptr, 0, nullptr) {}
  edge(node * s, node *d, int c, edge * b = nullptr)
    : _src(s), _dst(d), _cap(c), _back(b) {}

  inline node *dst() const noexcept { return _dst; }
  inline int   cap() const noexcept { return _cap; }
  inline int  flow() const noexcept { return _flow; }

  inline edge * back()  const noexcept { return _back; }
  inline edge * set_back(edge *e) noexcept { return _back = e; }

  inline int res_cap() const noexcept { return _cap - _flow; }

  inline void add_flow(int df) noexcept {
    _flow += df;
    _back->sub_flow(df);
  }

  inline void sub_flow(int df) noexcept {
    _flow -= df;
  }
};
