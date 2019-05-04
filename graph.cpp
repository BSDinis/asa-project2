#include "graph.hpp"
#include <algorithm>

graph create_graph(std::istream &in)
{
  using std::vector;

  int f, e, t;
  if (!(std::cin >> f)) throw "failed to get f";
  else if (f <= 0) throw "non positive f";
  if (!(std::cin >> e)) throw "failed to get e";
  else if (e <= 0) throw "non positive e";
  if (!(std::cin >> t)) throw "failed to get t";
  else if (t <= 0) throw "non positive t";


//  We want corte minimo closer to target so we need to do Relable to Front of transposed graph
  graph res(f + e*2 + 2);
  res.set_f(f);
  res.set_e(e);

  for (ssize_t i = 0; i < f; i++) {
    int production_value;
    if (!(std::cin >> production_value)) throw "failed to get a producer value";
    res.add_edge(i + 2, 0, production_value); // connect producer vertex to dummy sourse with the capacity of the production
  }

  for (ssize_t i = 0; i < e; i++) {
    int max_cap;
    if (!(std::cin >> max_cap)) throw "failed to get a capacity value";
    res.add_edge(i*2 + f + 2, i + f + 2, max_cap); // connect dummy station to station with the capacity of station
  }

  for (ssize_t i = 0; i < t; i++) {
    int o, d, c;
    if (!(std::cin >> o)) throw "failed to get source val";
    if (o < 2) throw "invalid source value";
    if (!(std::cin >> d)) throw "failed to get destination val";
    if (d < 1) throw "invalid destination value";
    if (!(std::cin >> c)) throw "failed to get capacity val";
    if (c < 1) throw "invalid capacity value";

    if ( d > f + 2 ) d += e; // because of dummy station
    res.add_edge(d, o, c);
  }

  return res;
}

int cf(struct edge& edge) // residual capacity
{ return edge.capacity - edge.flow; }

void graph::relable(int u) // h[u] = 1 + min {h[v] : (u,v) ∈ Ef }
{
  unsigned int min_h = -1;
  for ( auto edge : _node_list[u].neighbours )
    min_h = std::min(_node_list[edge.destination].height, min_h);

  if ( min_h != (unsigned int)-1 )
    _node_list[u].height = min_h + 1;
}

void graph::push(int u, int v, struct edge& edge) /*
                                * 1 df(u,v) = min(e[u],cf[u,v])
                                * 2 f[u,v] = f[u,v] + df(u,v)
                                * 3 f[v,u] = −f[u,v]
                                * 4 e[u] = e[u]−df(u,v)
                                * 5 e[v] = e[v] +df(u,v)
                                */
{
  int df = std::min( _node_list[u].excess, cf(edge) );
  edge.flow += df;

  // 3 f[v,u] = −f[u,v;: how to easily access transposed edge??

  _node_list[u].excess -= df;
  _node_list[v].excess += df;
}

void graph::initialize_preflow() // all heights, excesses and flow already at 0
{
  _node_list[source].height = V();

  for ( auto edge : _node_list[source].neighbours ) {
    edge.flow = edge.capacity;
    // f[u,s] = −c(s,u);
    _node_list[edge.destination].excess = edge.capacity;
    _node_list[source].excess -= edge.capacity;
  }

}
void graph::discharge(int u, int v) {}
void graph::relable_to_front() {}