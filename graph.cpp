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
  graph res(2 + prods + shippers * 2, 2 * (prods + shippers + nconns)); // source + dest + producers + 2 * shipping
  res.n_producers(prods);
  res.n_shippers(shippers);

  int i=0;
  for ( auto & node : res.nodes() ) {
    node.id(i); i++;
  }

  for (ssize_t producer = 0; producer < prods; producer++) {
    int production_value;
    if (!(std::cin >> production_value)) throw "failed to get a producer value";

    // connect producer vertex to dummy sourse with the capacity of the production
    res.add_edge(producer + 2, 0, production_value);
  }

  for (ssize_t s = 0; s < shippers; s++) {
    int max_cap;
    if (!(std::cin >> max_cap)) throw "failed to get a capacity value";

    // split shipper in two; the edge has the capacity
    res.add_edge(s + shippers + prods + 2, s + prods + 2, max_cap);
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

void initialize_preflow(graph &g) noexcept
{
  node & src = g.nodes()[source];
  src.height(g.V());

  for ( auto & n : g.nodes() )
    n.reset();

  src.src_discharge();
}


void relabel_to_front(graph &g) noexcept
{
  initialize_preflow(g);
  std::list<int> L;

  ssize_t sz = g.V();
  for (int i = sz - 1; i > 1; i--) {
    L.push_back(i);
  }

  for (auto it=L.begin(); it != L.end(); ++it) {
    node & u = g.nodes()[*it];
    int old_h = u.height();
    u.discharge();
    if (u.height() > old_h) {
      L.push_front(*it);
      L.erase(it);
      it = L.begin();
    }
  }
}

vector<int> shippers_in_cut(graph &g) noexcept
{
  vector<int> ship_cut;
  auto nodes = g.nodes();

  for ( unsigned i= 2 + g.n_producers() + g.n_shippers(); i < nodes.size(); i++ ) {
    if ( nodes[i].height() >= nodes[source].height()
      && nodes[i-g.n_shippers()].height() < nodes[source].height() )
      ship_cut.push_back(i-g.n_shippers());
  }

  return ship_cut;
}

vector<pair<int,int>> min_cut(graph &g) noexcept
{
  vector<pair<int,int>> ship_cut;
  auto nodes = g.nodes();

  for ( int i = 1; i < ((int) nodes.size()) - g.n_shippers() ; i++ ) {
    for ( auto * edge : nodes[i].edges() ) {
      if ( edge->cap() > 0
        && edge->dst()->id() != target
        && nodes[i].height() >= nodes[source].height()
        && edge->dst()->height() < nodes[source].height() )
      {
        int id = (edge->dst()->id() >= 2 + g.n_producers() + g.n_shippers())
          ? edge->dst()->id() - g.n_shippers()
          : edge->dst()->id();

        ship_cut.emplace_back(id, i);
      }
    }
  }

  std::sort(ship_cut.begin(), ship_cut.end());
  return ship_cut;
}
