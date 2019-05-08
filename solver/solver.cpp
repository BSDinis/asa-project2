#include "../graph.hpp"
#include <iostream>

int main() {
#if 0
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
#endif

  graph g = create_graph(std::cin);
  std::cerr << g << "\n";
  //g.relabel_to_front();
  std::cerr << g << "\n ========== \n";

  //std::cout << g.curr_flow() << '\n';

  return 0;
}
