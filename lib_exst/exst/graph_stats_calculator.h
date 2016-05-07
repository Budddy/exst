#ifndef CLASP_GRAPH_STATS_CALCULATOR_H
#define CLASP_GRAPH_STATS_CALCULATOR_H

#include <unordered_map>
#include <clasp/literal.h>
#include <exst/incidence_graph_stats.h>
#include <exst/dependency_graph_stats.h>
#include <exst/ExstTypes.h>

namespace exst {

    class GraphStatsCalculator {
    public:

        /*
         * returns the single instance of the GraphStatsCalculator
         */
        static GraphStatsCalculator &getInstance() {
            static GraphStatsCalculator calc;
            return calc;
        }

        /*
         * prints the dependency graph as edge list
         */
        std::string getDIMACS(MyGraph &graph, uint32 edgecount);

        /*
         * adds a dependency to the dependency graph
         */
        void parseRule(std::vector<uint32> dependencies, Clasp::PodVector<Clasp::Var>::type heads, uint32 negative);

        /*
         * adds a literal set during solving
         */
        void addAtomReduct(const Clasp::Literal lit);

        /*
         * prints the dependency graph
         */
        void printDepGraph();

        /*
         *
         */
        void printExtendedStats();

        /*
         * prints the incidence graph of the program
         */
        void printIncidenceGraph();

        /*
         * prints the incidence graoh of the reduct
         */
        void printIGraphReduct();

        /*
         * maps the new id to the old id
         */
        void addId(uint32 before, uint32 after);

        /*
         * resets the incidence graph
         */
        void resetAssignment();

        void reduceGraph(uint32 lit, bool neg);

    private:
        //calculator for statistics of incidence graph
        IncidenceGraphStats incidenceGraphStats;
        //calculation for statistics of dependency graph
        DependencyGraphStats dependencyGraphStats;
        //atoms of current assignment
        std::unordered_map<uint32, bool> selectedAtoms;
        //used for matching literal ids before and after pre processing
        std::unordered_map<int32, uint32> atomIds;
        uint32 numNonHornClauses = 0;
        uint32 numNonDualHornClauses = 0;

        //private constructors for singleton
        GraphStatsCalculator() {
        };

        /*
         * adds a rule to the dependency graph
         */
        void addRuleDependencyGraph(std::vector<uint32> bodies, Clasp::PodVector<Clasp::Var>::type head);

        /*
         * adds a rule to the incidence graph
         */
        void addRuleIncidenceGraph(std::vector<uint32> bodies, Clasp::PodVector<Clasp::Var>::type head,
                                   uint32 negative);

        const Clasp::SymbolTable *sTable;
        int numClauses = 0;
    };
}

#endif //CLASP_GRAPH_STATS_CALCULATOR_H
