#ifndef CLASP_INCIDENCE_GRAPH_STATS_H
#define CLASP_INCIDENCE_GRAPH_STATS_H

#include <unordered_map>
#include <exst/ExstTypes.h>
#include <list>
#include <htd/main.hpp>
#include <htd/MultiGraphFactory.hpp>

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
            libraryInstance = htd::createManagementInstance(htd::Id::FIRST);
            htd::MultiGraphFactory &factory = libraryInstance->multiGraphFactory();
            iGraph = factory.getMultiGraph();
        }

        htd::LibraryInstance *libraryInstance;

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
         * prints the incidence graph of the reduct
         */
        void printIGraphReduct();

        //complete incidence graph
        MyGraph incidenceGraph;
        htd::IMutableMultiGraph *iGraph;

        //incidence graph of reduct
        MyGraph incidenceGraphReduct;
        htd::IMutableMultiGraph *iGraphReduct;

        //mapping from atoms to vertices in the incidence graph
        std::unordered_map<uint32_t, uint32_t> atomVertexMap;

        //maps the rules to the bodies
        MyGraph ruleBodyMapReduct;

        //maps the rules to the bodies
        MyGraph ruleBodyMap;

        //maps atoms and ids
        std::unordered_map<uint32_t, uint32_t> &atomIds;

        //map for the selected atoms
        std::unordered_map<uint32_t, bool> &selectedAtoms;

        //list of size reductions incidence graph
        std::list<float> reds;

        size_t getTreewidth(htd::IMutableMultiGraph *graph);

        uint32_t edgecount = 0;
    };
}

#endif //CLASP_INCIDENCE_GRAPH_STATS_H
