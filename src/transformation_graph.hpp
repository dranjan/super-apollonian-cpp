#ifndef TRANSFORMATION_GRAPH_HPP
#define TRANSFORMATION_GRAPH_HPP

#include <array>
#include <vector>

namespace apollonian {

template <typename Transform>
struct graph_edge {
    unsigned int type_index;  /* The index of the target node. */
    unsigned int id;          /* An easily hashable or comparable identifier
                               * for this edge or transformation.
                               */
    Transform transform;      /* The transformation itself. */
};

template <unsigned int N, typename Transform>
class transformation_graph {
public:
    template <typename... Args>
    transformation_graph(Args&&... args);

public:
    /* The adjacency list of the transformation graph. */
    std::array<std::vector<graph_edge<Transform>>, N> edges_;
};

template <unsigned int N, typename Transform>
template <typename... Args>
transformation_graph<N, Transform>::transformation_graph(Args&&... args)
    : edges_{std::forward<Args>(args)...}
{
}

} // apollonian

#endif // TRANSFORMATION_GRAPH_HPP
