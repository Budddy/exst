#ifndef CLASP_GRAPH_STATS_CALCULATOR_H
#define CLASP_GRAPH_STATS_CALCULATOR_H

#include <unordered_map>
#include <exst/incidence_graph_stats.h>
#include <exst/dependency_graph_stats.h>
#include <exst/ExstTypes.h>

namespace exst
{
    class GraphStatsCalculator
    {
    public:
        GraphStatsCalculator(std::unordered_map<uint32_t, uint32_t> &atomIds,
                             std::unordered_map<uint32_t, bool> &selectedAtoms) :
                atomIds(atomIds), selectedAtoms(selectedAtoms), incidenceGraphStats(atomIds, selectedAtoms)
        {
        }

        //calculation for statistics of dependency graph
        DependencyGraphStats dependencyGraphStats;

        //
        std::unordered_map<uint32_t, uint32_t> &atomIds;

        //
        std::unordered_map<uint32_t, bool> &selectedAtoms;

        //calculator for statistics of incidence graph
        IncidenceGraphStats incidenceGraphStats;
    };
}

#endif //CLASP_GRAPH_STATS_CALCULATOR_H
