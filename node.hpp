#pragma once

#include <vector>
#include <limits>
#include "edge.hpp"

using std::vector;
using vec_it = std::vector<edge>::iterator;

class node {
  int _overflow = 0;
  int _height   = 0;

  vector<edge> _edges;
  vec_it       _curr = _edges.end(); // hey

  inline int src_push(edge & e) noexcept {
    int df = e.cap();
    _overflow -= df;
    e.add_flow(df);
    e.dst()->recv(df);
    return df;
  }

  public:

  inline int overflow() const noexcept { return _overflow; }
  inline int height()   const noexcept { return _height;   }
  inline int height(int h)    noexcept { return _height = h; }

  int relabel() noexcept {
    int min_height(std::numeric_limits<int>::max());
    for (const auto & e : _edges) {
      if (e.dst()->height() >= _height)
        min_height = std::min(min_height, e.dst()->height());
    }

    return _height = min_height + 1;
  }

  inline vec_it reset() noexcept { return _curr = _edges.begin(); }
  inline int recv(int df) noexcept { return _overflow += df; }

  inline int push(edge & e) noexcept {
    int df = std::min(e.res_cap(), _overflow);
    _overflow -= df;
    e.add_flow(df);
    e.dst()->recv(df);
    return df;
  }

  int src_discharge() noexcept {
    int sum = 0;
    for (auto & e : _edges)
      sum += src_push(e);

    return sum;
  }

  void discharge() noexcept {
    auto it = _curr;
    while (overflow() > 0) {
      if (it == _edges.end()) {
        relabel();
        it = _edges.begin();
        continue;
      }

      edge & e = *it;
      if (e.res_cap() > 0 && _height == e.dst()->height() + 1) {
        push(e);
      }
      else {
        it++;
      }
    }

    _curr = it;
  }

  edge * add_edge(node * dst, int cap, edge *in) noexcept {
    _edges.emplace_back(edge(dst, cap, in));
    return &_edges.back();
  }

  edge * add_edge(node * dst, int cap) noexcept {
    _edges.emplace_back(edge(dst, cap));
    edge * out = &_edges.back();
    out->set_back(dst->add_edge(this, 0, out));
    return out;
  }


  inline vector<edge> & edges() noexcept { return _edges; }
  inline const vector<edge> & cedges() const noexcept { return _edges; }
};