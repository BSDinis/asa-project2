#pragma once

#include <vector>
#include <iostream>
#include <utility>
#include <numeric>

#include "node.hpp"
#include "edge.hpp"

#define GRAPH_DEBUG 1

constexpr int source = 1;
constexpr int target = 0;

using std::vector;

class graph {

  int _n_producers=0, _n_shippers=0;
  vector<node> _node_list;
  vector<edge> _edge_list;

  public:
    graph(int n, int m) noexcept : graph(static_cast<size_t>(n), static_cast<size_t>(m)) {} // reserve
    graph(ssize_t n, ssize_t m) noexcept : graph(static_cast<size_t>(n), static_cast<size_t>(m)) {} // reserve
    graph(size_t n, size_t m)  noexcept :
      _node_list(n), _edge_list(m) { /* reserve */ }
    ~graph() = default; // needs to free edges;

    bool add_edge_to_shipper(const int u, int v, const int w, const int n_prods, const int n_shippers) noexcept
    {
      if (v >= 2 + n_prods) v += n_shippers;
      return add_edge(u, v, w);
    }

    bool add_edge(const int u, const int v, const int cap) noexcept
    {
      _edge_list.emplace_back(&_node_list[u], &_node_list[v], cap);
      edge * out = &_edge_list.back();

      _edge_list.emplace_back(&_node_list[v], &_node_list[u], 0);
      edge * in  = &_edge_list.back();

      out->set_back(in);
      in->set_back(out);

      _node_list[u].add_edge(out);
      _node_list[u].add_edge(in);
      return true;
    }

    inline int V() const noexcept { return _node_list.size(); }
    inline int n_producers() const noexcept { return _n_producers; }
    inline int n_shippers()  const noexcept { return _n_shippers; }

    inline int n_producers(int f) noexcept { return _n_producers = f; }
    inline int n_shippers(int e) noexcept  { return _n_shippers = e; }

    inline vector<node> & nodes() noexcept { return _node_list; }

    inline int  curr_flow() const noexcept {
      return _node_list[target].overflow();
    }

#if GRAPH_DEBUG
    std::ostream & print(std::ostream & os) const noexcept {
      /*
      os << "src\t|dst\t|capsrc\t|flow\t\n";
      */
      ssize_t idx = 0;
      for (const auto & n : _node_list) {
        os << idx++ << "[" << &n
          << "] h = " << n.height()
          << "; e = " << n.overflow() << ";\n";
        for (const auto * e : n.cedges()) {
          os << " |- " << (e->dst() - &(_node_list[0]))
            << "[" << e << "] (" << e->back() << ") : "
            << e->flow() << " / " << e->cap()
            << "\n";
        }
        os << '\n';
      }

      return os << " ============================================== ";
    }
#endif
};

graph create_graph(std::istream &in) ;
void initialize_preflow(graph & g) noexcept;
void relabel_to_front(graph & g) noexcept;

#if GRAPH_DEBUG
inline std::ostream & operator<<(std::ostream &os, const graph & g)
{
  return g.print(os);
}
#endif
