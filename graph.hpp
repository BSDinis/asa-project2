#pragma once

#include <vector>
#include <iostream>
#include <utility>
#include <numeric>

#define GRAPH_DEBUG 1

// DUMMY
#define source 1
// TARGET
#define target 0

using std::vector;
class graph {

  struct edge {
    int destination;
    int capacity;
    int flow = 0;

    edge * trans_edge;

    edge(int v, int cap, edge* p_edge) noexcept :
      destination(v),
      capacity(cap),
      trans_edge(p_edge) {}

    edge(int v, int cap) noexcept : edge(v, cap, nullptr) {}
  };

  struct node {
    int excess = 0;
    unsigned int height = 0, current = 0;
    vector<edge> neighbours;

    void add(edge&& n_edge) noexcept {
      neighbours.emplace_back( n_edge );
    }
  };

  int _n_producers=0, _n_shippers=0;
  vector<node> _node_list;

  public:
    graph()          noexcept : graph(128) {} // reserve
    graph(int n)     noexcept : graph(static_cast<size_t>(n)) {} // reserve
    graph(ssize_t n) noexcept : graph(static_cast<size_t>(n)) {} // reserve
    graph(size_t n)  noexcept : _node_list(n) { // reserve
      while (n-- > 0) _node_list.emplace_back();
    }
    ~graph() = default; // needs to free edges;

    inline bool has_link(const int u, const int v) const noexcept
    {
      if ( !(u < V() && v < V()) ) return false;
      if (u == v) return true;
      for (const auto neighbour : _node_list[u].neighbours)
        if (neighbour.destination == v) return true;

      return false;
    }

    bool add_edge(const int u, const int v, const int w) noexcept
    {
      _node_list[u].add(edge(v, w));
      _node_list[v].add(edge(u, 0, &_node_list[u].neighbours.back()));
      _node_list[u].neighbours.back().trans_edge = &_node_list[v].neighbours.back();
      return true;
    }

    inline int V() const noexcept { return _node_list.size(); }
    inline int n_producers() const noexcept { return _n_producers; }
    inline int n_shippers()  const noexcept { return _n_shippers; }

    inline int n_producers(int f) noexcept { return _n_producers = f; }
    inline int n_shippers(int e) noexcept  { return _n_shippers = e; }

    inline int height(const int u) const noexcept { return _node_list[u].height; }

    inline int cf(struct edge& edge) noexcept;

    void push(int u, int v, edge& edge) noexcept;
    void relable(int u) noexcept;
    void initialize_preflow() noexcept;
    void discharge(int u) noexcept;
    void relabel_to_front() noexcept;

    inline int  curr_flow() const noexcept {
      return std::accumulate(
          _node_list[0].neighbours.begin(),
          _node_list[0].neighbours.end(),
          0,
          [](int &x, const edge &y) { return x + y.flow; }
          );
    }

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
