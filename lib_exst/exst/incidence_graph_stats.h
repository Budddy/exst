#ifndef CLASP_INCIDENCE_GRAPH_STATS_H
#define CLASP_INCIDENCE_GRAPH_STATS_H

#include <unordered_map>
#include <exst/ExstTypes.h>
#include <list>

/*
 * class used for calculating and saving stats of the incidence graph and the incidence graph
 */
namespace exst
{
    class IncidenceGraphStats
    {
    public:
        IncidenceGraphStats(std::unordered_map<uint32_t, uint32_t> &atomIds,
                            std::unordered_map<uint32_t, bool> &selectedAtoms) :
                atomIds(atomIds), selectedAtoms(selectedAtoms)
        {
        }

        /*
         * adds a rule to the incidence graph
         */
        void addRuleIncidenceGraph(std::list<lit_type> bodies, std::list<lit_type> head);

        /*
         * adds a literal set during solving
         */
        void addAtomReduct(lit_type lit);

        /*
         * resets the incidence graph
         */
        void resetAssignment();

        /*
         *
         */
        void reduceGraph(lit_type lit);

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
        std::unordered_map<uint32_t, uint32_t> atomVertexMap;

        //maps the rules to the bodies
        MyGraph ruleBodyMap;

        //numer of edges in the incidence graph
        uint32_t edgecount = 0;

        //numer of nodes in the incidence graph
        uint32_t nodecount = 0;

        //numer of edges in the reduct of the incidence graph
        uint32_t edgecountReduct = 0;

        //numer of nodes in the reduct of the incidence graph
        uint32_t nodecountReduct = 0;

        //maps atoms and ids
        std::unordered_map<uint32_t, uint32_t> &atomIds;

        //map for the selected atoms
        std::unordered_map<uint32_t, bool> &selectedAtoms;
    };
}

#endif //CLASP_INCIDENCE_GRAPH_STATS_H
