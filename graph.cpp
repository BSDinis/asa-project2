#include "graph.hpp"
#include <list>
#include <algorithm>

graph create_graph(std::istream &in)
{
  using std::vector;

  int prods, shippers, nconns;
  if (!(std::cin >> prods)) throw "failed to get f";
  else if (prods <= 0) throw "non positive f";
  if (!(std::cin >> shippers)) throw "failed to get e";
  else if (shippers <= 0) throw "non positive e";
  if (!(std::cin >> nconns)) throw "failed to get t";
  else if (nconns <= 0) throw "non positive t";


  //  We want minimal cut closer to target so we need
  //  to do Relable to Front of transposed graph
  graph res(1 + 1 + prods + shippers * 2); // source + dest + producers + 2 * shipping
  res.n_producers(prods);
  res.n_shippers(shippers);

  for (ssize_t producer = 0; producer < prods; producer++) {
    int production_value;
    if (!(std::cin >> production_value)) throw "failed to get a producer value";

    // connect producer vertex to dummy sourse with the capacity of the production
    res.add_edge(producer + 2, 0, production_value);
  }

  for (ssize_t shipper = 0; shipper < shippers; shipper++) {
    int max_cap;
    if (!(std::cin >> max_cap)) throw "failed to get a capacity value";

    // split shipper in two; the edge has the capacity
    res.add_edge(shipper * 2 + prods + 2, shipper + prods + 2, max_cap);
  }

  for (ssize_t connection = 0; connection < nconns; connection++) {
    int src, dst, cap;
    if (!(std::cin >> src)) throw "failed to get source val";
    if (src < 2) throw "invalid source value";
    if (!(std::cin >> dst)) throw "failed to get destination val";
    if (dst < 1) throw "invalid destination value";
    if (!(std::cin >> cap)) throw "failed to get capacity val";
    if (cap < 1) throw "invalid capacity value";

    if ( dst > prods + 2 ) dst += shippers; // because of dummy station
    res.add_edge(dst, src, cap);
  }

  return res;
}

int graph::cf(struct edge& edge) noexcept // residual capacity
{ return edge.capacity - edge.flow; }

void graph::relable(int u) noexcept // h[u] = 1 + min {h[v] : (u,v) ∈ Ef }
{
  unsigned int min_h = (unsigned) -1;
  for ( auto edge : _node_list[u].neighbours )
    min_h = std::min(_node_list[edge.destination].height, min_h);

  if ( min_h != (unsigned) -1 )
    _node_list[u].height = min_h + 1;
}

void graph::push(int u, int v, struct edge& edge) noexcept
{
  int df = std::min( _node_list[u].excess, cf(edge) );
  edge.flow += df;
  edge.trans_edge->flow = -edge.flow;
  _node_list[u].excess -= df;
  _node_list[v].excess += df;
}

void graph::initialize_preflow() noexcept // all heights, excesses and flow already at 0
{
  _node_list[source].height = V();

  for ( auto edge : _node_list[source].neighbours ) {
    edge.flow = edge.capacity;
    edge.trans_edge->flow = -edge.capacity;
    _node_list[edge.destination].excess = edge.capacity;
    _node_list[source].excess -= edge.capacity;
  }

}

void graph::discharge(int u) noexcept
{
  unsigned int i=0; // slides suggest i should be stored in u but i dont know why (implies you look at neighbers >= i first )
  struct node& node = _node_list[u];
  while ( node.excess > 0 ) {
    if ( i >= node.neighbours.size() ) {
      relable(u);
      i=0;
    } else if ( cf( node.neighbours[i] ) > 0
              && node.height == _node_list[node.neighbours[i].destination].height + 1 )
      push(u, node.neighbours[i].destination, node.neighbours[i]);

    else i++;
  }
}

void graph::relable_to_front() noexcept
{
  initialize_preflow();
  vector<int> L(_node_list.size() - 2);
  ssize_t sz = L.size();
  for (int i = 0; i < sz; i++)
    L[i] = i + 2;

  for (auto u : L) {
    int old_h = height(u);
    discharge(u);
    if (height(u) > old_h)
      L.push_back(u);
  }
}
