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

int graph::relabel(int u) noexcept // h[u] = 1 + min {h[v] : (u,v) ∈ Ef }
{
  auto min_h = std::min_element(
      _node_list.begin(), _node_list.end(),
      [](const node & a, const node & b) -> bool
      { return a.height() < b.height(); }
      );

  if (min_h != _node_list.end())
    return _node_list[u].relabel(min_h->height() + 1);

  return -1;
}

void graph::push(int u, int v, edge& e) noexcept
{
  int df = std::min( _node_list[u].excess(), e.res_cap() );
  e.push(df);
  _node_list[u].push(df);
  _node_list[v].recv(df);
}

void graph::initialize_preflow() noexcept
{
  // all heights, excesses and flow already at 0
  _node_list[source].relabel(V());

  for ( auto edge : _node_list[source].edges() ) {
    edge.push(edge.cap());
    _node_list[source].push(edge.cap());
    _node_list[edge.dst()].recv(edge.cap());
  }

}

void graph::discharge(int u) noexcept
{
  // refactor
}

void graph::relabel_to_front() noexcept
{
  initialize_preflow();
  std::list<int> L;
  ssize_t sz = V();
  for (int i = 2; i < sz; i++)
    L.push_back(i);

  for (std::list<int>::iterator it=L.begin() ; it != L.end(); ++it) {
    int u = *it;
    int old_h = height(u);
    discharge(u);
    if (height(u) > old_h) {
      L.push_front(u);
      L.erase(it);
      it = L.begin();
    }
  }
}
