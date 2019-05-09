#pragma once

#include <vector>
#include <queue>
#include <limits>
#include "edge.hpp"

using std::vector;
using vec_it = std::vector<edge *>::iterator;

class node {
  int _idx = 0;
  int _overflow = 0;
  int _height   = 0;

  vector<edge *> _edges;
  vec_it _curr = _edges.end();

  inline int src_push(edge & e) noexcept {
    int df = e.cap(_idx);
    _overflow -= df;
    e.add_flow(_idx, df);
    e.dst(_idx)->recv(df);
    return df;
  }

  public:

  inline int  id()  const noexcept { return _idx; }
  inline void id(int idx) noexcept { _idx = idx; }
  bool is_id(int id) noexcept { return _idx == id; }

  inline int overflow() const noexcept { return _overflow; }
  inline int height()   const noexcept { return _height;   }
  inline int height(int h)    noexcept { return _height = h; }

  int relabel() noexcept {
    int min_height(std::numeric_limits<int>::max());
    for (const auto * e : _edges) {
      if (e->dst(_idx)->height() >= _height)
        min_height = std::min(min_height, e->dst(_idx)->height());
    }

    return _height = min_height + 1;
  }

  inline vec_it reset() noexcept { return _curr = _edges.begin(); }
  inline int recv(int df) noexcept { return _overflow += df; }

  inline int push(edge & e) noexcept {
    int df = std::min(e.res_cap(_idx), _overflow);
    _overflow -= df;
    e.add_flow(_idx, df);
    e.dst(_idx)->recv(df);
    return df;
  }

  std::queue<int> src_discharge() noexcept {
    std::queue<int> res;
    for (auto * e : _edges) {
      src_push(*e);
      res.push(e->dst(_idx)->id());
    }

    return res;
  }

  vector<int> discharge() noexcept {
    auto it = _curr;
    vector<int> new_actives;
    while (_overflow > 0) {
      if (it == _edges.end()) {
        relabel();
        it = _edges.begin();
        continue;
      }

      edge & e = **it;
      if (e.res_cap(_idx) > 0 && _height == e.dst(_idx)->height() + 1) {
        bool inactive = e.dst(_idx)->overflow() == 0;
        push(e);
        if (inactive && e.dst(_idx)->id() > 1)
          new_actives.push_back(e.dst(_idx)->id());
      }
      else {
        it++;
      }
    }

    _curr = it;
    return new_actives;
  }

  inline void add_edge(edge *out) noexcept { _edges.push_back(out); }
  inline vector<edge *> & edges() noexcept { return _edges; }
  inline const vector<edge *> & cedges() const noexcept { return _edges; }
};
