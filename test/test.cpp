#include "../graph.hpp"
#include <iostream>

int main() {
  graph g;
  g = create_graph(std::cin);
  std::cout << g;

  return 0;
}
