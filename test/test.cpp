#include "../graph.hpp"
#include <iostream>

int main() {
  graph g;
  try {
    g = create_graph(std::cin);
  }
  catch (const char * str) {
    std::cerr << "ERROR: " << str << '\n';
  }
  std::cout << g;

  return 0;
}
