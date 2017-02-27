#ifndef CLASP_DEPENDENCY_GRAPH_STATS_H
#define CLASP_DEPENDENCY_GRAPH_STATS_H

#include <unordered_map>
#include <list>
#include <iostream>
#include <exst/exst_types.h>
#include <exst/dependency_graph_stats.h>
#include <exst/interfaces.h>
#include <exst/program_parameter.h>

namespace exst {
    struct DependencyGraphStatistics {
        ///dependency graph
        MyGraph dependencyGraph;
        ///mapping from graph vertices to literals
        std::unordered_map<uint32_t, uint32_t> atomVertexMap;
        ///
        MyGraph edgeMap;
    };

    /**
     * class used for calculating and saving stats of the dependency graph and the actual dependency graph
     */
    class DependencyGraphStatsCalculator : public StatisticsCalculator {
    public:

        /**
         * adds a rule to the dependency graph
         * @param body body of the rule
         * @param head head of the rule
         */
        virtual void addRule(std::list<lit_type> body, std::list<lit_type> head);

        virtual std::list<std::pair<std::string, std::string>> getStatistics();

        virtual std::list<std::string> getAdditionalStatistics();

        /**
         * Getter for the Dependency Graph.
         * @return the dependency graph
         */
        MyGraph &getDependencyGraph() {
            return dGraphStatistics.dependencyGraph;
        }

        DependencyGraphStatistics dGraphStatistics;
    };
}

#endif //CLASP_DEPENDENCY_GRAPH_STATS_H
