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
  //  to do Relabel to Front of transposed graph
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
    res.add_edge(shipper + shippers + prods + 2, shipper + prods + 2, max_cap);
  }

  for (ssize_t connection = 0; connection < nconns; connection++) {
    int src, dst, cap;
    if (!(std::cin >> src)) throw "failed to get source val";
    if (src < 2) throw "invalid source value";
    if (!(std::cin >> dst)) throw "failed to get destination val";
    if (dst < 1) throw "invalid destination value";
    if (!(std::cin >> cap)) throw "failed to get capacity val";
    if (cap < 1) throw "invalid capacity value";

    // inverted
    res.add_edge_to_shipper(dst, src, cap, prods, shippers);
  }

  return res;
}

int graph::relabel(int u) noexcept // h[u] = 1 + min {h[v] : (u,v) ∈ Ef }
{
  unsigned int min_h = -1;

  node & node_u = _node_list[u];

  for ( auto & edge : node_u.edges() ) {
    node & node_v = _node_list[ edge.second.dst() ];
    if ( node_u.height() <= node_v.height() && (unsigned) node_v.height() < min_h )
      min_h = node_v.height();
  }

  if (min_h != (unsigned)-1)
    return node_u.relabel(min_h + 1);

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

  for ( auto & pair : _node_list[source].edges() ) {
    pair.second.push(pair.second.cap());
    _node_list[source].push(pair.second.cap());
    _node_list[pair.second.dst()].recv(pair.second.cap());
  }

}

void graph::discharge(int idx) noexcept
{
  node & u = _node_list[idx];
  auto edge_it  = u.edges().begin();
  const auto end  = u.edges().end();
  std::cerr << " ;excess: " << u.excess();
  while (u.excess() > 0) {
    edge & e = edge_it->second;
    if (edge_it == end) {
      std::cerr << " ;rlbld: " << relabel(idx);
      edge_it = u.edges().begin();
    }
    else if (e.res_cap() > 0
        && u.height() == _node_list[e.dst()].height() + 1) {
      std::cerr << " ;pushed to " << e.dst();
      push(idx, e.dst(), e);
    }
    else {
      edge_it++;
    }
  }
}

void graph::relabel_to_front() noexcept
{
  initialize_preflow();
  std::list<int> L;
  ssize_t sz = V();
  std::cerr << "L:";
  for (int i = sz - 1; i > 1; i++) {
    L.push_back(i);
    std::cerr << ' ' << i;
  }
  std::cerr << '\n';


  for (auto it=L.begin(); it != L.end(); ++it) {
    int u = *it;
    int old_h = height(u);
    std::cerr << "working on " << u << '[' << old_h << "]... ";
    discharge(u);
    std::cerr << "; discharged\n";
    if (height(u) > old_h) {
      L.push_front(u);
      L.erase(it);
      it = L.begin();
    }
  }
}
