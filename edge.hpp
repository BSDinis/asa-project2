#pragma once
#include <vector>
#include "node.hpp"

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

  inline node *dst(int id) const noexcept {
    return _src->is_id(id) ? _dst : _src;
  }
  inline int cap(int id) const noexcept {
    return _src->is_id(id) ? _cap : 0;
  }
  inline int flow(int id) const noexcept {
    return _src->is_id(id) ? _flow : -_flow;
  }

  inline int res_cap(int id) const noexcept {
    return _src->is_id(id) ? _cap - _flow : _flow; // cap(id) - flow(id);
  }

  inline void add_flow(int id, int df) noexcept {
    _src->is_id(id) ? _flow += df : _flow -=df;
  }
};
