#ifndef CLASP_DEPENDENCY_GRAPH_STATS_H
#define CLASP_DEPENDENCY_GRAPH_STATS_H

#include <htd/LabeledHypergraph.hpp>
#include <bits/unordered_map.h>
#include <clasp/exst/graph_stats_calculator.h>

/*
 * class used for calculating and saving stats of the dependency graph and the actual dependency graph
 */

namespace exst
{
    class DependencyGraphStats
    {
    public:
        //dependency graph
        htd::LabeledHypergraph dependencyGraph;
        //mapping from graph vertices to literals
        std::unordered_map<int32_t, htd::vertex_t> atomVertexMap;
        std::unordered_map<uint32_t, std::unordered_map<uint32_t, uint32_t>> edgeMap;
    };
}

#endif //CLASP_DEPENDENCY_GRAPH_STATS_H
