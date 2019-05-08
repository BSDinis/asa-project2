#include "../graph.hpp"
#include <iostream>

int main() {
  graph g;
  g = create_graph(std::cin);
  for (auto &n : g.nodes())
    n.reset();
  std::cout << g;

  return 0;
}
