#pragma once

#include <vector>
#include <map>
#include <iostream>
#include <utility>
#include <numeric>

#define GRAPH_DEBUG 1

constexpr int source = 1;
constexpr int target = 0;

using std::vector;
using std::map;

class graph {
  class edge {
    int _dst;
    int _cap, _flow;
    edge * _back;

    public:
    edge(int d, int c) : _dst(d), _cap(c), _flow(0), _back(nullptr) {}

    inline int  dst() const noexcept { return _dst; }
    inline int  cap() const noexcept { return _cap; }
    inline int flow() const noexcept { return _flow; }

    inline edge * back()  const noexcept { return _back; }
    inline edge * set_back(edge *e) noexcept { return _back = e; }

    inline int res_cap() const noexcept { return _cap - _flow; }

    inline void add_flow(int df) noexcept {
      _flow += df;
      _back->sub_flow(df);
    }

    inline void sub_flow(int df) noexcept {
      _flow -= df;
    }
  };

  class node {
    int _excess = 0;
    int _height = 0;
    vector<edge> _edges;
    vector<edge>::iterator _current;

    public:
      node() { _current = _edges.end(); }

      inline void add(edge & n_edge) noexcept {
        _edges.push_back(n_edge);
      }

      inline void add_transpose_to_last(edge * e) noexcept {
        _edges.back().set_back(e);
      }

      inline edge * last() noexcept {
        return &_edges[_edges.size() - 1];
      }

      inline const vector<edge> & cedges() const noexcept {
        return _edges;
      }

      inline vector<edge> & edges() noexcept {
        return _edges;
      }

      inline decltype(_current) current() noexcept { return _current; }
      inline decltype(_current) current(decltype(_current) c) noexcept { return _current = c; }

      inline int excess() const noexcept { return _excess; }
      inline int height() const noexcept { return _height; }

      inline int push(int df) noexcept { return _excess -= df ; }
      inline int recv(int df) noexcept { return _excess += df ; }

      inline int relabel(int new_height) noexcept
      { return _height = new_height ; }
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

    bool add_edge_to_shipper(const int u, int v, const int w, const int n_prods, const int n_shippers) noexcept
    {
      if (v >= 2 + n_prods) v += n_shippers;
      return add_edge(u, v, w);
    }

    bool add_edge(const int u, const int v, const int w) noexcept
    {
      edge e1(v, w);
      edge e2(u, 0);
      _node_list[u].add(e1);
      _node_list[v].add(e2);
      _node_list[u].add_transpose_to_last(_node_list[v].last());
      _node_list[v].add_transpose_to_last(_node_list[u].last());
      return true;
    }

    inline int V() const noexcept { return _node_list.size(); }
    inline int n_producers() const noexcept { return _n_producers; }
    inline int n_shippers()  const noexcept { return _n_shippers; }

    inline int n_producers(int f) noexcept { return _n_producers = f; }
    inline int n_shippers(int e) noexcept  { return _n_shippers = e; }

    inline int height(const int u) const noexcept { return _node_list[u].height(); }

    void push(int u, edge& edge) noexcept;
    int  relabel(int u) noexcept;
    void initialize_preflow() noexcept;
    void discharge(int u) noexcept;
    void relabel_to_front() noexcept;

    inline int  curr_flow() const noexcept {
      return _node_list[target].excess();
    }

#if GRAPH_DEBUG
    std::ostream & print(std::ostream & os) const noexcept {
      os << "src\t|dst\t|capsrc\t|flow\t\n";
      ssize_t i = 0;
      for (const auto & n : _node_list) {
        for (const auto & e : n.cedges()) {
          os << i << "\t|"
            << e.dst()<< "\t|"
            << e.cap() << "\t|"
            << e.flow() << "\t|\n";
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
