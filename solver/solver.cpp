#include "../graph.hpp"
#include <iostream>

int main() {
#if 0
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
#endif

  graph g;
  g = create_graph(std::cin);
  g.relabel_to_front();

  return 0;
}
