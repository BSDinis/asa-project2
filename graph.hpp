#pragma once

#include <vector>
#include <iostream>
#include <utility>

#define GRAPH_DEBUG 1

// DUMMY
#define source (0);
// TARGET
#define target (1);

using std::pair;
using std::make_pair;
using std::vector;
class graph {

  struct edge {
    int destination;
    int capacity;
    int flow;

    edge(int v, int cap) noexcept : destination(v), capacity(cap) {} 
    
    bool is_saturated() { return capacity == flow; }
  };

  struct node {
    int excess   = 0;
    int height   = 0;
    vector<struct edge> neighbours;

    void add(int v, int w) noexcept {
      neighbours.emplace_back( edge(v, w) );
    }
  };

  int _f=0, _e=0;
  vector<node> _node_list;
  public:
    graph()          noexcept : graph(128) {} // reserve
    graph(int n)     noexcept : graph(static_cast<size_t>(n)) { } // reserve
    graph(ssize_t n) noexcept : graph(static_cast<size_t>(n)) {} // reserve
    graph(size_t n)  noexcept : _node_list(n) { // reserv
      while (n-- > 0) _node_list.emplace_back();
    }
    ~graph() = default;

    inline bool has_link(const int u, const int v) const noexcept
    {
      if (!(u < V() && v < V())) return false;
      if (u == v) return true;
      for (const auto neighbour : _node_list[u].neighbours) {
        if (neighbour.destination == v) return true;
      }
      return false;
    }

    bool add_edge(const int u, const int v, const int w) noexcept 
    {
      //if (has_link(u, v)) return false; //not neccessary nor efficient
      _node_list[u].add(v, w);
      _node_list[v].add(u, 0); // for relable to front. might be a better way
      return true;
    }

    inline int V() const noexcept { return _node_list.size()-_e; }
    inline int get_f() const noexcept { return _f; }
    inline int get_e() const noexcept { return _e; }

    inline void set_f(int f) { _f = f; }
    inline void set_e(int e) { _e = e; }

    inline void relable(int u) noexcept { _node_list[u].height++; }

#if GRAPH_DEBUG
    std::ostream & print(std::ostream & os) const noexcept {
      os << "src\t|dst\t|capsrc\t|edgcap\t|\n";
      const ssize_t sz = _node_list.size();
      for (int i = 0; i < sz; i++) {
        for (const auto & dst : _node_list[i].neighbours) {
          os << i << "\t|"
            << dst.destination << "\t|"
            << dst.capacity << "\t|\n";
        }
      }

      return os;
    }
#endif
};

graph create_graph(std::istream &in) ;

#if GRAPH_DEBUG
inline std::ostream & operator<<(std::ostream &os, const graph & g)
{
  return g.print(os);
}
#endif
