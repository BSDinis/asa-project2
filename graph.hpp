#pragma once

#include <vector>
#include <map>
#include <iostream>
#include <utility>
#include <numeric>

#define GRAPH_DEBUG 1

// DUMMY
#define source 0
// TARGET
#define target 1

using std::vector;
using std::map;

class graph {

  class edge {
    int _dst;
    int _cap;
    int _flow = 0;

    edge * trans_edge;

    public:
    edge(int d, int c, edge* e) noexcept
      : _dst(d), _cap(c), trans_edge(e) {}

    edge(int v, int cap) noexcept
      : edge(v, cap, nullptr) {}

    inline int flow() const noexcept { return _flow; }
    inline int cap() const noexcept  { return _cap; }
    inline int dst() const noexcept  { return _dst; }

    inline int res_cap() const noexcept
    { return _cap - _flow; }

    inline bool connects_to(int v) const noexcept
    { return _dst == v; }


    inline edge * transpose(edge * e) noexcept
    { return trans_edge = e; }

    inline void push(int df) noexcept {
      _flow += df;
      trans_edge->_flow -= df;
    }

    inline constexpr bool operator<(const edge & rhs) const noexcept {
      return dst() > rhs.dst();
    }

  };

  class node {
    int _excess = 0;
    int _height = 0, _current = 0;
    map<int, edge, std::greater<int>> _edges; // ordered in descending order

    public:
      inline edge * add(edge&& n_edge) noexcept {
        auto p = _edges.emplace(n_edge.dst(), n_edge);
        return &(p.first->second);
      }

      inline const map<int, edge, std::greater<int>> & cedges() const noexcept {
        return _edges;
      }

      inline map<int, edge, std::greater<int>> & edges() noexcept {
        return _edges;
      }

      inline bool can_reach(int v) const noexcept {
        return _edges.find(v) != _edges.end();
      }

      inline int excess() const noexcept { return _excess; }
      inline int height() const noexcept { return _height; }

      inline int push(int df) noexcept { return _excess -= df ; }
      inline int recv(int df) noexcept { return _excess += df ; }

      inline int relabel(int new_height) noexcept
      { return _height = new_height ; }

      inline int flow() const noexcept {
          return std::accumulate(
          _edges.cbegin(),
          _edges.cend(),
          0,
          [](int &x,
            const std::pair<int, edge> & y)
          { return x + y.second.flow(); }
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
    }
    ~graph() = default; // needs to free edges;

    inline bool has_link(const int u, const int v) const noexcept
    {
      if ( !(u < V() && v < V()) ) return false;
      if (u == v) return true;
      return _node_list[u].can_reach(v);
    }

    bool add_edge_to_shipper(int u, const int v, const int w, const int n_prods, const int n_shippers) noexcept
    {
      if (u >= 2 + n_prods) u += n_shippers;
      return add_edge(u, v, w);
    }

    bool add_edge(const int u, const int v, const int w) noexcept
    {
      auto e1 = _node_list[u].add(edge(v, w));
      auto e2 = _node_list[v].add(edge(u, 0, e1));
      e1->transpose(e2);
      return true;
    }

    inline int V() const noexcept { return _node_list.size(); }
    inline int n_producers() const noexcept { return _n_producers; }
    inline int n_shippers()  const noexcept { return _n_shippers; }

    inline int n_producers(int f) noexcept { return _n_producers = f; }
    inline int n_shippers(int e) noexcept  { return _n_shippers = e; }

    inline int height(const int u) const noexcept { return _node_list[u].height(); }

    void push(int u, int v, edge& edge) noexcept;
    int  relabel(int u) noexcept;
    void initialize_preflow() noexcept;
    void discharge(int u) noexcept;
    void relabel_to_front() noexcept;

    inline int  curr_flow() const noexcept {
      return - _node_list[source].excess();
    }

#if GRAPH_DEBUG
    std::ostream & print(std::ostream & os) const noexcept {
      os << "src\t|dst\t|capsrc\t|\n";
      ssize_t i = 0;
      for (const auto & n : _node_list) {
        for (const auto & e : n.cedges()) {
          os << i << "\t|"
            << e.second.dst()<< "\t|"
            << e.second.cap() << "\t|\n";
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
