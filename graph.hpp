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

  class node {
    int _excess = 0;
    int _height = 0, _current = 0;
    vector<edge> _edges;

    public:
      inline edge * add(edge&& n_edge) noexcept {
        _edges.emplace_back( n_edge );
        return &_edges.back();
      }

      inline vector<edge> & edges() noexcept {
        return _edges;
      }

      inline bool can_reach(int v) const noexcept {
        for (const auto & e : _edges)
          if (e.destination == v) return true;

        return false;
      }

      inline int excess() const noexcept { return _excess; }
      inline int height() const noexcept { return _height; }

      inline int push(int df) noexcept { return _excess -= df ; }
      inline int recv(int df) noexcept { return _excess += df ; }

      inline int relabel(int new_height) noexcept { return _height = new_height ; }

      inline int flow() const noexcept {
          return std::accumulate(
          _edges.begin(),
          _edges.end(),
          0,
          [](int &x, const edge &y) { return x + y.flow; }
          );
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
      return _node_list[u].can_reach(v);
    }

    bool add_edge(const int u, const int v, const int w) noexcept
    {
      auto e1 = _node_list[u].add(edge(v, w));
      auto e2 = _node_list[v].add(edge(u, 0, e1));
      e1->trans_edge = e2;
      return true;
    }

    inline int V() const noexcept { return _node_list.size(); }
    inline int n_producers() const noexcept { return _n_producers; }
    inline int n_shippers()  const noexcept { return _n_shippers; }

    inline int n_producers(int f) noexcept { return _n_producers = f; }
    inline int n_shippers(int e) noexcept  { return _n_shippers = e; }

    inline int height(const int u) const noexcept { return _node_list[u].height(); }

    inline int cf(struct edge& edge) noexcept;

    void push(int u, int v, edge& edge) noexcept;
    int  relabel(int u) noexcept;
    void initialize_preflow() noexcept;
    void discharge(int u) noexcept;
    void relabel_to_front() noexcept;

    inline int  curr_flow() const noexcept {
      return _node_list[source].flow();
    }

#if GRAPH_DEBUG
    std::ostream & print(std::ostream & os) const noexcept {
      os << "src\t|dst\t|capsrc\t|edgcap\t|\n";
      ssize_t i = 0;
      for (const auto & n : _node_list) {
        for (const auto & e : n.edges()) {
          os << i << "\t|"
            << e.destination << "\t|"
            << e.capacity << "\t|\n";
        }
        i++;
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
