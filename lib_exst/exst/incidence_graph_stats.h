#ifndef CLASP_INCIDENCE_GRAPH_STATS_H
#define CLASP_INCIDENCE_GRAPH_STATS_H

#include <unordered_map>
#include <exst/ExstTypes.h>
#include <clasp/util/platform.h>
#include <clasp/pod_vector.h>
#include <clasp/literal.h>

/*
 * class used for calculating and saving stats of the incidence graph and the incidence graph
 */
namespace exst
{
    class IncidenceGraphStats
    {
    public:
        IncidenceGraphStats(std::unordered_map<int32, uint32> &atomIds,
                            std::unordered_map<uint32, bool> &selectedAtoms) :
                atomIds(atomIds), selectedAtoms(selectedAtoms)
        {
        }

        /*
         * adds a rule to the incidence graph
         */
        void addRuleIncidenceGraph(Clasp::WeightLitVec bodies, bk_lib::pod_vector<uint32> head, uint32 negative);

        /*
         * adds a literal set during solving
         */
        void addAtomReduct(const Clasp::Literal lit);

        /*
         * resets the incidence graph
         */
        void resetAssignment();

        /*
         *
         */
        void reduceGraph(uint32 lit, bool neg);

        /*
         * prints the incidence graoh of the reduct
         */
        void printIGraphReduct();

        /*
         * prints the incidence graph of the program
         */
        void printIncidenceGraph();

        //complete incidence graph
        MyGraph incidenceGraph;
        //incidence graph of reduct
        MyGraph incidenceGraphReduct;
        //mapping from atoms to vertices in the incidence graph
        std::unordered_map<uint32, uint32> atomVertexMap;
        //maps the rules to the bodies
        MyGraph ruleBodyMap;
        //numer of edges in the incidence graph
        uint32 edgecount = 0;
        //numer of nodes in the incidence graph
        uint32 nodecount = 0;
        //numer of edges in the reduct of the incidence graph
        uint32 edgecountReduct = 0;
        //numer of nodes in the reduct of the incidence graph
        uint32 nodecountReduct = 0;
        //maps atoms and ids
        std::unordered_map<int32, uint32> &atomIds;
        //map for the selected atoms
        //TODO maybe list?
        std::unordered_map<uint32, bool> &selectedAtoms;
    };
}

#endif //CLASP_INCIDENCE_GRAPH_STATS_H
