#include "graph.hpp"

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

  graph res(f + 2*e + 2);
  for (ssize_t i = 0; i < f; i++) {
    int production_value;
    if (!(std::cin >> production_value)) throw "failed to get a producer value";
    res.add_edge(0, i + 2, production_value); // connect dummy source to producer vertex with the capacity of the production
  }

  for (ssize_t i = 0; i < e; i++) {
    int max_cap;
    if (!(std::cin >> max_cap)) throw "failed to get a capacity value";
    res.add_edge(i + f + 2, i + e + f + 2, max_cap);
  }

  for (ssize_t i = 0; i < t; i++) {
    int o, d, c;
    if (!(std::cin >> o)) throw "failed to get source val";
    if (o < 2) throw "invalid source value";
    if (!(std::cin >> d)) throw "failed to get destination val";
    if (d < 1) throw "invalid destination value";
    if (!(std::cin >> c)) throw "failed to get capacity val";
    if (c < 1) throw "invalid capacity value";
    res.add_edge(o, d, c);
  }

  return res;
}

