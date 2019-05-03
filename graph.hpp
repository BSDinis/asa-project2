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

  struct node {
    using neighbour = pair<int,int>;
    int excess   = 0;
    vector<neighbour> neighbours;

    void add(int v, int w) noexcept {
      neighbours.emplace_back(v, w);
    }
  };

  vector<node> node_list;
  public:
    graph()          noexcept : graph(128) {} // reserve
    graph(int n)     noexcept : graph(static_cast<size_t>(n)) { } // reserve
    graph(ssize_t n) noexcept : graph(static_cast<size_t>(n)) {} // reserve
    graph(size_t n)  noexcept : node_list(n) { // reserv
      while (n-- > 0) node_list.emplace_back();
    }
    ~graph() = default;

    inline bool has_link(const int u, const int v) const noexcept
    {
      if (!(u < V() && v < V())) return false;
      if (u == v) return true;
      for (const auto neighbour : node_list[u].neighbours) {
        if (neighbour.first == v) return true;
      }
      return false;
    }


    bool add_edge(const int u, const int v, const int w) noexcept
    {
      if (has_link(u, v)) return false;
      node_list[u].add(v, w);
      return true;
    }

    inline int V() const noexcept { return node_list.size(); }

#if GRAPH_DEBUG
    std::ostream & print(std::ostream & os) const noexcept {
      os << "src\t|dst\t|edgcap\t|\n";
      const ssize_t sz = node_list.size();
      for (int i = 0; i < sz; i++) {
        for (const auto & dst : node_list[i].neighbours) {
          os << i << "\t|"
            << dst.first << "\t|"
            << dst.second << "\t|\n";
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
