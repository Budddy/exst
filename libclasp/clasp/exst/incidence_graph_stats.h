#ifndef CLASP_INCIDENCE_GRAPH_STATS_H
#define CLASP_INCIDENCE_GRAPH_STATS_H

#include <htd/LabeledHypergraph.hpp>
#include <htd/Label.hpp>
#include <bits/unordered_map.h>
#include <clasp/exst/graph_stats_calculator.h>

/*
 * class used for calculating and saving stats of the incidence graph and the incidence graph
 */
namespace exst
{
    class IncidenceGraphStats
    {
    public:
        struct GraphInfo
        {
            bool head = false;
            bool negative = false;
        };
        //complete incidence graph
        htd::LabeledHypergraph incidenceGraph;
        //minimal incidence graph
        htd::LabeledHypergraph minIncidenceGraph;
        //incidence graph of reduct
        htd::LabeledHypergraph incidenceGraphReduct;
        //rule counter
        uint32_t rules = 1;
        //mapping from atoms to vertices in the incidence graph
        std::unordered_map<int32_t, htd::vertex_t> atomVertexMap;
        //mapping from rules to vertices in the incidence graph
        std::unordered_map<int32_t, htd::vertex_t> ruleVertexMap;
        std::unordered_map<uint32_t, std::map<uint32_t, bool>> bodyRuleMap;
        std::unordered_map<uint32_t, std::map<uint32_t, bool>> ruleBodyMap;
    };
}

#endif //CLASP_INCIDENCE_GRAPH_STATS_H
