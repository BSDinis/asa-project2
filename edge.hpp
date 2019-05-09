#pragma once
#include <vector>

using std::vector;

class node;

class edge {
  node * _src;
  node * _dst;
  int _cap;
  int _flow = 0;

  public:
  edge() : edge(nullptr, nullptr, 0) {}
  edge(node * s, node *d, int c)
    : _src(s), _dst(d), _cap(c) {}

  node *dst(int id) const noexcept ;
  int cap(int id) const noexcept ;
  int flow(int id) const noexcept ;
  int res_cap(int id) const noexcept ;
  void add_flow(int id, int df) noexcept ;
};
