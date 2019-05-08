#include "../graph.hpp"
#include <iostream>

int main() {
#if 0
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
#endif

  graph g = create_graph(std::cin);
  //std::cerr << g << "\n";
  relabel_to_front(g);

  //std::cerr << g << "\n ========== \n";

  std::cout << g.curr_flow() << '\n';

  auto ids = shippers_in_cut(g);
  if ( ids.size() > 0 )
  	std::cout << ids[0];

  for ( unsigned i = 1; i < ids.size(); i++ )
  	std::cout << " " << ids[i];

  std::cout << "\n";

  auto nodes = min_cut(g);
  for ( auto node = nodes.begin(); node != nodes.end(); node++ ) {
  	std::cout << *node << " "; node++;
  	std::cout << *node << "\n";
  }


  return 0;
}
