#ifndef CLASP_ABSTRACTEXTENDEDSTATSCALCULATOR_H
#define CLASP_ABSTRACTEXTENDEDSTATSCALCULATOR_H

#include <stdio.h>
#include <clasp/literal.h>
#include <clasp/shared_context.h>
#include <htd/LabeledHypergraph.hpp>
#include <htd/Label.hpp>
#include <list>
#include <unordered_map>

namespace exst
{

    /*
     * class used for calculating and saving stats of the incidence graph and the incidence graph
     */
    class IncidenceGraphStats
    {
    public:
        struct GraphInfo{
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
        uint32 rules = 1;
        //mapping from atoms to vertices in the incidence graph
        std::unordered_map<int32, htd::vertex_t> atomVertexMap;
        //mapping from rules to vertices in the incidence graph
        std::unordered_map<int32, htd::vertex_t> ruleVertexMap;
        std::unordered_map<uint32, std::map<uint32,bool>> bodyRuleMap;
        std::unordered_map<uint32, std::map<uint32,bool>> ruleBodyMap;
    };

    /*
     * class used for calculating and saving stats of the dependency graph and the actual dependency graph
     */
    class DependencyGraphStats
    {
    public:
        //dependency graph
        htd::LabeledHypergraph dependencyGraph;
        //mapping from graph vertices to literals
        std::unordered_map<int32, htd::vertex_t> atomVertexMap;
        std::unordered_map<uint32,std::unordered_map<uint32,uint32>> edgeMap;
    };

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
        void addDep(std::vector<uint32> dependencies, Clasp::VarVec heads, uint32 negative);

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

    private:
        //calculator for statistics of incidence graph
        IncidenceGraphStats incidenceGraphStats;
        //calculation for statistics of dependency graph
        DependencyGraphStats dependencyGraphStats;
        //atoms of current assignment
        std::unordered_map<uint32,bool> selectedAtoms;
        //used for matching literal ids before and after pre processing
        std::unordered_map<int32, uint32> atomIds;

        //private constructors for singleton
        GraphStatsCalculator()
        {
        };

        /*
         * adds a rule to the dependency graph
         */
        void addRuleDependencyGraph(std::vector<uint32> bodies, Clasp::VarVec heads, uint32 negative);

        /*
         * adds a rule to the incidence graph
         */
        void addRuleIncidenceGraph(std::vector<uint32> vector, Clasp::VarVec pod_vector, uint32 negative);

        /*
         * adds labels to the dependency graph
         */
        void labelDepGraph(const Clasp::SymbolTable &symbolTable);

        /*
         * adds labels to the incidence graph
         */
        void labelInzGraph(const Clasp::SymbolTable &symbolTable,htd::LabeledHypergraph &graph);
        void generateReductGraph();
        const Clasp::SymbolTable *sTable;
    };
}

#endif //CLASP_EXTENDEDSTATSCALCULATOR_H
