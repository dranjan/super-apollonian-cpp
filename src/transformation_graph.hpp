#ifndef TRANSFORMATION_GRAPH_HPP
#define TRANSFORMATION_GRAPH_HPP

#include <array>
#include <vector>

namespace apollonian {

template <typename Transform>
struct GraphEdge {
    unsigned int type_index;
    unsigned int id;
    Transform transform;
};

template <unsigned int N, typename Transform>
class TransformationGraph {
public:
    template <typename... Args>
    TransformationGraph(Args&&... args);

public:
    std::array<std::vector<GraphEdge<Transform>>, N> edges_;
};

template <unsigned int N, typename Transform>
template <typename... Args>
TransformationGraph<N, Transform>::TransformationGraph(Args&&... args)
    : edges_{std::forward<Args>(args)...}
{
}

} // apollonian

#endif // TRANSFORMATION_GRAPH_HPP
