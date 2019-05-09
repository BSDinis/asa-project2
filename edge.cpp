#include "edge.hpp"
#include "node.hpp"

inline node *edge::dst(int id) const noexcept {
  return _src->is_id(id) ? _dst : _src;
}
inline int edge::cap(int id) const noexcept {
  return _src->is_id(id) ? _cap : 0;
}
inline int edge::flow(int id) const noexcept {
  return _src->is_id(id) ? _flow : -_flow;
}

inline int edge::res_cap(int id) const noexcept {
  return _src->is_id(id) ? _cap - _flow : _flow; // cap(id) - flow(id);
}

inline void edge::add_flow(int id, int df) noexcept {
  _src->is_id(id) ? _flow += df : _flow -=df;
}
