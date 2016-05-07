#ifndef CLASP_DEPENDENCY_GRAPH_STATS_H
#define CLASP_DEPENDENCY_GRAPH_STATS_H

#include <unordered_map>
#include <exst/ExstTypes.h>

/*
 * class used for calculating and saving stats of the dependency graph and the actual dependency graph
 */

namespace exst {
    class DependencyGraphStats {
    public:
        //dependency graph
        MyGraph dependencyGraph;
        //mapping from graph vertices to literals
        std::unordered_map<uint32, uint32> atomVertexMap;
        MyGraph edgeMap;
        uint32 edgecount = 0;
        uint32 nodecount = 0;
    };
}

#endif //CLASP_DEPENDENCY_GRAPH_STATS_H
