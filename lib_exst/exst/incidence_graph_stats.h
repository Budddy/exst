#ifndef CLASP_INCIDENCE_GRAPH_STATS_H
#define CLASP_INCIDENCE_GRAPH_STATS_H

#include <unordered_map>
#include <map>
#include "ExstTypes.h"

/*
 * class used for calculating and saving stats of the incidence graph and the incidence graph
 */
namespace exst {
    class IncidenceGraphStats {
    public:
        //complete incidence graph
        MyGraph incidenceGraph;
        //incidence graph of reduct
        MyGraph incidenceGraphReduct;
        //mapping from atoms to vertices in the incidence graph
        std::unordered_map<uint32, uint32> atomVertexMap;
        MyGraph ruleBodyMap;
        uint32 edgecount = 0;
        uint32 nodecount = 0;
        uint32 edgecountReduct = 0;
        uint32 nodecountReduct = 0;
    };
}

#endif //CLASP_INCIDENCE_GRAPH_STATS_H
