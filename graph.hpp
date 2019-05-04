#pragma once

#include <vector>
#include <iostream>
#include <utility>

#define GRAPH_DEBUG 1

// DUMMY
#define source (1);
// TARGET
#define target (0);

using std::pair;
using std::make_pair;
using std::vector;
class graph {

  struct edge { // be carefull with witch node is looking at the edge
    int destination;
    int capacity;
    int flow = 0;

    edge(int v, int cap) noexcept : destination(v), capacity(cap) {}
  };

  struct node {
    int excess = 0;
    unsigned int height = 0;
    vector<struct edge> neighbours;

    void add(struct edge&& edge) noexcept {
      neighbours.emplace_back( edge );
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
    ~graph() = default; // needs to free edges;

    inline bool has_link(const int u, const int v) const noexcept
    {
      if ( !(u < V() && v < V()) ) return false;
      if (u == v) return true;
      for (const auto neighbour : _node_list[u].neighbours) {
        if (neighbour.destination == v) return true;
      }
      return false;
    }

    bool add_edge(const int u, const int v, const int w) noexcept 
    {
      //if (has_link(u, v)) return false; //not neccessary nor efficient
      _node_list[u].add(edge(v, w));
      _node_list[v].add(edge(u, 0));
      return true;
    }

    inline int V() const noexcept { return _node_list.size(); }
    inline int get_f() const noexcept { return _f; }
    inline int get_e() const noexcept { return _e; }

    inline void set_f(int f) { _f = f; }
    inline void set_e(int e) { _e = e; }

    void push(int u, int v);
    void relable(int u);
    void initialize_preflow();
    void discharge(int u, int v);
    void relable_to_front();

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
