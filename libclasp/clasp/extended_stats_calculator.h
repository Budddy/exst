#ifndef CLASP_ABSTRACTEXTENDEDSTATSCALCULATOR_H
#define CLASP_ABSTRACTEXTENDEDSTATSCALCULATOR_H

#include <clasp/literal.h>
#include <clasp/shared_context.h>
#include <htd/LabeledHypergraph.hpp>
#include <htd/Label.hpp>

namespace exst
{

    /*
     * class used for calculating and saving stats of the incidence graph and the incidence graph
     */
    class IncidenceGraphStats
    {
    public:
        /*
         * returns the incidence graph of the program
         */
        htd::LabeledHypergraph &getIncidenceGraph()
        {
            return incidenceGraph;
        };

        /*
         * returns the atom to vertex map
         */
        std::map <int32, htd::vertex_t> &getAtomVertexMap()
        {
            return atomVertexMap;
        };

        /*
         * returns the rule to vertex map
         */
        std::map <int32, htd::vertex_t> &getRuleVertexMap()
        {
            return ruleVertexMap;
        };

        /*
         * returns the current rule count
         */
        int &getRuleCount()
        {
            return rules;
        };

        /*
         * sets the minimal Incidence Graph
         */
        void setMinimalIncidenceGraph(const htd::LabeledHypergraph &graph)
        {
            minIncidenceGraph = graph;
        }

        /*
         * returns the minimal incidence graph
         */
        htd::LabeledHypergraph &getMinimalIncidenceGraph()
        {
            return minIncidenceGraph;
        }

        /*
         * returns the incidence graph of the reduct
         */
        htd::LabeledHypergraph &getIncidenceGraphReduct()
        {
            return incidenceGraphReduct;
        }

    private:
        //complete incidence graph
        htd::LabeledHypergraph incidenceGraph;
        //minimal incidence graph
        htd::LabeledHypergraph minIncidenceGraph;
        //incidence graph of reduct
        htd::LabeledHypergraph incidenceGraphReduct;
        //rule counter
        int rules = 0;
        //mapping from atoms to vertices in the incidence graph
        std::map <int32, htd::vertex_t> atomVertexMap;
        //mapping from rules to vertices in the incidence graph
        std::map <int32, htd::vertex_t> ruleVertexMap;
    };

    /*
     * class used for calculating and saving stats of the dependency graph and the actual dependency graph
     */
    class DependencyGraphStats
    {
    public:

        /*
         * returns the dependency graph
         */
        htd::LabeledHypergraph &getGraph()
        {
            return dependencyGraph;
        };

        /*
         * returns the map between atoms and vertices
         */
        std::map <int32, htd::vertex_t> &getAtomVertexMap()
        {
            return atomVertexMap;
        };

    private:
        //dependency graph
        htd::LabeledHypergraph dependencyGraph;
        //mapping from graph vertices to literals
        std::map <int32, htd::vertex_t> atomVertexMap;
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
        void printEdgeList(htd::LabeledHypergraph graph);

        /*
         * adds a dependency to the dependency graph
         */
        void addDep(std::vector <uint32> dependencies, Clasp::VarVec heads, uint32 negative);

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
        //selected literals of current assignment
        std::vector <int32> selectedAtoms;
        //used for matching literal ids before and after pre processing
        std::map <int32, uint32> atomIds;

        //private constructors for singleton
        GraphStatsCalculator()
        {
        };

        /*
         * adds a rule to the dependency graph
         */
        void addRuleDependencyGraph(std::vector <uint32> dependencies, Clasp::VarVec heads, uint32 negative);

        /*
         * adds a rule to the incidence graph
         */
        void addRuleIncidenceGraph(std::vector <uint32> vector, Clasp::VarVec pod_vector, uint32 negative);

        /*
         * adds labels to the dependency graph
         */
        void labelDepGraph(const Clasp::SymbolTable &symbolTable);

        /*
         * adds labels to the incidence graph
         */
        void labelInzGraph(const Clasp::SymbolTable &symbolTable);
    };
}

#endif //CLASP_EXTENDEDSTATSCALCULATOR_H
