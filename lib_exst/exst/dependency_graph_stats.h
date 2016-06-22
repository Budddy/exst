#ifndef CLASP_DEPENDENCY_GRAPH_STATS_H
#define CLASP_DEPENDENCY_GRAPH_STATS_H

#include <unordered_map>
#include <exst/ExstTypes.h>
#include <unordered_map>
#include <exst/incidence_graph_stats.h>
#include <exst/dependency_graph_stats.h>
#include <exst/ExstTypes.h>
#include <list>

/*
 * class used for calculating and saving stats of the dependency graph and the actual dependency graph
 */

namespace exst
{
    class DependencyGraphStats
    {
    public:
        /*
         * adds a rule to the dependency graph
         */
        void addRuleDependencyGraph(std::list<lit_type> bodies, std::list<lit_type> head);

        /*
         * prints the dependency graph
         */
        void printDepGraph();

        //dependency graph
        MyGraph dependencyGraph;
        //mapping from graph vertices to literals
        std::unordered_map<uint32_t, uint32_t> atomVertexMap;
        //
        MyGraph edgeMap;
        //number of edges in the dependency graph
        uint32_t edgecount = 0;
        //number of nodes in the dependency graph
        uint32_t nodecount = 0;
    };
}

#endif //CLASP_DEPENDENCY_GRAPH_STATS_H
