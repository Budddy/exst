#ifndef CLASP_GRAPH_STATS_CALCULATOR_H
#define CLASP_GRAPH_STATS_CALCULATOR_H

#include <htd/LabeledHypergraph.hpp>
#include <clasp/literal.h>
#include <clasp/exst/incidence_graph_stats.h>
#include <clasp/exst/dependency_graph_stats.h>

namespace exst
{
    class GraphStatsCalculator
    {
    public:

        /*
         * returns the single instance of the GraphStatsCalculator
         */
        static GraphStatsCalculator &getInstance()
        {
            static GraphStatsCalculator calc;
            return calc;
        }

        /*
         * prints the dependency graph as edge list
         */
        void printEdgeList(htd::LabeledHypergraph &graph);

        /*
         * adds a dependency to the dependency graph
         */
        void addDep(std::vector<uint32_t> dependencies, Clasp::PodVector<Clasp::Var>::type heads, uint32_t negative);

        /*
         * adds the atom labels to the graph
         */
        void labelGraph(const Clasp::SymbolTable &symbolTable);

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
        void addId(uint32_t before, uint32_t after);

        /*
         * resets the incidence graph
         */
        void resetAssignment();
        void buildIncidenzeGraph();

    private:
        //calculator for statistics of incidence graph
        IncidenceGraphStats incidenceGraphStats;
        //calculation for statistics of dependency graph
        DependencyGraphStats dependencyGraphStats;
        //atoms of current assignment
        std::unordered_map<uint32_t, bool> selectedAtoms;
        //used for matching literal ids before and after pre processing
        std::unordered_map<int32_t, uint32_t> atomIds;
        uint32 numNonHornClauses = 0;
        uint32 numNonDualHornClauses = 0;
        uint32 normalClause = 0;

        //private constructors for singleton
        GraphStatsCalculator()
        {
        };

        /*
         * adds a rule to the dependency graph
         */
        void addRuleDependencyGraph(std::vector<uint32_t> bodies, Clasp::PodVector<Clasp::Var>::type heads, uint32_t negative);

        /*
         * adds a rule to the incidence graph
         */
        void addRuleIncidenceGraph(std::vector<uint32_t> vector, Clasp::PodVector<Clasp::Var>::type pod_vector, uint32_t negative);

        /*
         * adds labels to the dependency graph
         */
        void labelDepGraph(const Clasp::SymbolTable &symbolTable);

        /*
         * adds labels to the incidence graph
         */
        void labelInzGraph(const Clasp::SymbolTable &symbolTable, htd::LabeledHypergraph &graph);
        void generateReductGraph();
        const Clasp::SymbolTable *sTable;
    };
}

#endif //CLASP_GRAPH_STATS_CALCULATOR_H
