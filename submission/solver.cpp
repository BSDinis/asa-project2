#include <iostream>

#include <vector>
#include <queue>
#include <iostream>
#include <utility>
#include <numeric>
#include <list>
#include <queue>
#include <algorithm>
#include <limits>

constexpr int source = 1;
constexpr int target = 0;

using std::vector;
using std::pair;

class node;

class edge {
  node * _src;
  node * _dst;
  int _cap;
  int _flow = 0;

  public:
  edge() : edge(nullptr, nullptr, 0) {}
  edge(node * s, node *d, int c)
    : _src(s), _dst(d), _cap(c) {}

  node *dst(int id) const noexcept ;
  int cap(int id) const noexcept ;
  int flow(int id) const noexcept ;
  int res_cap(int id) const noexcept ;
  void add_flow(int id, int df) noexcept ;
};

using vec_it = std::vector<edge *>::iterator;
class node {
  int _idx = 0;
  int _overflow = 0;
  int _height   = 0;

  vector<edge *> _edges;
  vec_it _curr = _edges.end();

  inline int src_push(edge & e) noexcept {
    int df = e.cap(_idx);
    _overflow -= df;
    e.add_flow(_idx, df);
    e.dst(_idx)->recv(df);
    return df;
  }

  public:

  inline int  id()  const noexcept { return _idx; }
  inline void id(int idx) noexcept { _idx = idx; }
  bool is_id(int id) noexcept { return _idx == id; }

  inline int overflow() const noexcept { return _overflow; }
  inline int height()   const noexcept { return _height;   }
  inline int height(int h)    noexcept { return _height = h; }

  int relabel() noexcept {
    int min_height(std::numeric_limits<int>::max());
    for (const auto * e : _edges) {
      if (e->dst(_idx)->height() >= _height)
        min_height = std::min(min_height, e->dst(_idx)->height());
    }

    return _height = min_height + 1;
  }

  inline vec_it reset() noexcept { return _curr = _edges.begin(); }
  inline int recv(int df) noexcept { return _overflow += df; }

  inline int push(edge & e) noexcept {
    int df = std::min(e.res_cap(_idx), _overflow);
    _overflow -= df;
    e.add_flow(_idx, df);
    e.dst(_idx)->recv(df);
    return df;
  }

  std::queue<int> src_discharge() noexcept {
    std::queue<int> res;
    for (auto * e : _edges) {
      src_push(*e);
      res.push(e->dst(_idx)->id());
    }

    return res;
  }

  vector<int> discharge() noexcept {
    auto it = _curr;
    vector<int> new_actives;
    while (_overflow > 0) {
      if (it == _edges.end()) {
        relabel();
        it = _edges.begin();
        continue;
      }

      edge & e = **it;
      if (e.res_cap(_idx) > 0 && _height == e.dst(_idx)->height() + 1) {
        bool inactive = e.dst(_idx)->overflow() == 0;
        push(e);
        if (inactive && e.dst(_idx)->id() > 1)
          new_actives.push_back(e.dst(_idx)->id());
      }
      else {
        it++;
      }
    }

    _curr = it;
    return new_actives;
  }

  inline void add_edge(edge *out) noexcept { _edges.push_back(out); }
  inline vector<edge *> & edges() noexcept { return _edges; }
  inline const vector<edge *> & cedges() const noexcept { return _edges; }
};


class graph {

  int _n_producers=0, _n_shippers=0;
  vector<node> _node_list;
  vector<edge> _edge_list;

  public:
    graph(int n, int m) noexcept : graph(static_cast<size_t>(n), static_cast<size_t>(m)) {} // reserve
    graph(ssize_t n, ssize_t m) noexcept : graph(static_cast<size_t>(n), static_cast<size_t>(m)) {} // reserve
    graph(size_t n, size_t m)  noexcept :
      _node_list(n), _edge_list(m) { /* reserve */ }
    ~graph() = default;

    bool add_edge_to_shipper(const int u, int v, const int w, const int n_prods, const int n_shippers) noexcept
    {
      if (v >= 2 + n_prods) v += n_shippers;
      return add_edge(u, v, w);
    }

    bool add_edge(const int u, const int v, const int cap) noexcept
    {
      _edge_list.emplace_back(&_node_list[u], &_node_list[v], cap);
      edge * out = &_edge_list.back();

      _node_list[u].add_edge(out);
      _node_list[v].add_edge(out);
      return true;
    }

    inline int V() const noexcept { return _node_list.size(); }
    inline int n_producers() const noexcept { return _n_producers; }
    inline int n_shippers()  const noexcept { return _n_shippers; }

    inline int n_producers(int f) noexcept { return _n_producers = f; }
    inline int n_shippers(int e) noexcept  { return _n_shippers = e; }

    inline vector<node> & nodes() noexcept { return _node_list; }

    inline int  curr_flow() const noexcept {
      return _node_list[target].overflow();
    }

};

graph create_graph(std::istream &in) ;
std::queue<int> initialize_preflow(graph & g) noexcept;
void relabel_to_front(graph & g) noexcept;
vector<int> shippers_in_cut(graph & g) noexcept;
vector<pair<int,int>> min_cut(graph &g) noexcept;

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);

  graph g = create_graph(std::cin);
  relabel_to_front(g);
  std::cout << g.curr_flow() << '\n';

  auto ids = shippers_in_cut(g);
  if ( ids.size() > 0 )
  	std::cout << ids[0];

  for ( unsigned i = 1; i < ids.size(); i++ )
  	std::cout << " " << ids[i];

  std::cout << "\n";

  auto nodes = min_cut(g);
  for ( auto node = nodes.begin(); node != nodes.end(); node++ ) {
  	std::cout << node->first << ' ' << node->second << '\n';
  }


  return 0;
}

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
  graph res(2 + prods + shippers * 2, prods + shippers + nconns); // source + dest + producers + 2 * shipping
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

std::queue<int> initialize_preflow(graph &g) noexcept
{
  node & src = g.nodes()[source];
  src.height(g.V());

  for ( auto & n : g.nodes() )
    n.reset();

  return src.src_discharge();
}


void relabel_to_front(graph &g) noexcept
{
  std::queue<int> actives = initialize_preflow(g);

  while (!actives.empty()) {
    int id = actives.front();
    actives.pop();
    for ( const auto & idx : g.nodes()[id].discharge() ) {
      actives.push(idx);
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
      if ( edge->cap(i) > 0
        && edge->dst(i)->id() != target
        && nodes[i].height() >= nodes[source].height()
        && edge->dst(i)->height() < nodes[source].height() )
      {
        int id = (edge->dst(i)->id() >= 2 + g.n_producers() + g.n_shippers())
          ? edge->dst(i)->id() - g.n_shippers()
          : edge->dst(i)->id();

        ship_cut.emplace_back(id, i);
      }
    }
  }

  std::sort(ship_cut.begin(), ship_cut.end());
  return ship_cut;
}


node *edge::dst(int id) const noexcept {
  return _src->is_id(id) ? _dst : _src;
}
int edge::cap(int id) const noexcept {
  return _src->is_id(id) ? _cap : 0;
}
int edge::flow(int id) const noexcept {
  return _src->is_id(id) ? _flow : -_flow;
}

int edge::res_cap(int id) const noexcept {
  return _src->is_id(id) ? _cap - _flow : _flow;
}

void edge::add_flow(int id, int df) noexcept {
  _src->is_id(id) ? _flow += df : _flow -=df;
}
