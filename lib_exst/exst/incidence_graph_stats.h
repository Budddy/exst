#ifndef CLASP_INCIDENCE_GRAPH_STATS_H
#define CLASP_INCIDENCE_GRAPH_STATS_H

#include <unordered_map>
#include <list>
#include <htd/MultiGraphFactory.hpp>
#include <exst/exst_types.h>
#include <clasp/literal.h>
#include <exst/interfaces.h>
#include <exst/program_parameter.h>

/**
 * class used for calculating and saving stats of the incidence graph and the incidence graph
 */
namespace exst
{
    struct IncidenceGraphStatistics
    {
        ///
        unsigned long long assignmentCount = 0;

        ///
        htd::LibraryInstance *libraryInstance;

        ///complete incidence graph
        MyGraph incidenceGraph;
        htd::IMutableMultiGraph *iGraph;

        ///incidence graph of reduct
        MyGraph incidenceGraphReduct;
        htd::IMutableMultiGraph *iGraphReduct = nullptr;

        ///mapping from atoms to vertices in the incidence graph
        std::unordered_map<uint32_t, uint32_t> atomVertexMap;

        ///maps the rules to the bodies
        MyGraph ruleBodyMapReduct;

        ///maps the rules to the bodies
        MyGraph ruleBodyMap;

        ///maps atoms and ids
        std::unordered_map<uint32_t, uint32_t> atomIds;

        ///list of the treewidth of the incidence graph reductions
        std::list<float> widths;

        ///the literals that are currently assigned to the solution
        Clasp::LitVec current_assignment;

        ///the literals that are currently assigned to the solution
        int numReducts = 0;
        std::list<std::string> rGraphs;
    };

    class IncidenceGraphStatsCalculator : public StatisticsCalculator
    {
    public:

        /**
         * Adds a rule to the incidence graph.
         * @param body body of the new rule
         * @param head head of the new rule
         */
        virtual void addRule(std::list<lit_type> body, std::list<lit_type> head);

        /**
         * Updates the current assignment.
         * @param new_assignment list of literals in the new assignment
         */
        virtual void updateAssignment(Clasp::LitVec new_assignment);

        virtual std::list<std::pair<std::string, std::string>> getStatistics();

        virtual void addId(uint32_t before, uint32_t after);

        virtual std::list<std::string> getAdditionalStatistics();

        /**
         * Constructor of the IncidenceGraphStats class.
         * @param atomIds used to math atom ids before and after preprocessing
         * @return a new instance of the incidence graph stat calculator
         */
        IncidenceGraphStatsCalculator()
        {
            iGraphStats.libraryInstance = htd::createManagementInstance(htd::Id::FIRST);
            htd::MultiGraphFactory &factory = iGraphStats.libraryInstance->multiGraphFactory();
            iGraphStats.iGraph = factory.getMultiGraph();
        }

        /**
         * Tesets the incidence graph of the reduct.
         */
        void resetAssignment();

        /**
         * Reduces the incidence graph.
         * @param lit the literal used to reduce the graph
         */
        void reduceGraph(lit_type lit);

        IncidenceGraphStatistics iGraphStats;

    };

    /**
     * Calculates the treewidth of the given graph.
     * @param graph the graph to calculate the treewidth
     * @return the treewidth of the given graph
     */
    static size_t getTreewidth(htd::IMutableMultiGraph *graph, htd::LibraryInstance *libraryInstance);

    /*
     *  Width Minimizing Fitness Function Copied from htd: https://github.com/mabseher/htd/blob/master/src/htd_main/main.cpp
     *  Implementation of the htd::ITreeDecompositionFitnessFunction interface which prefers decompositions of minimal width.
     */
    class WidthMinimizingFitnessFunction : public htd::ITreeDecompositionFitnessFunction
    {
    public:
        /*
         *  Constructor of class WidthMinimizingFitnessFunction.
         */
        WidthMinimizingFitnessFunction(void)
        {
        }

        /*
         *  Destructor of class WidthMinimizingFitnessFunction.
         */
        virtual ~WidthMinimizingFitnessFunction()
        {
        }

        htd::FitnessEvaluation *
        fitness(const htd::IMultiHypergraph &graph, const htd::ITreeDecomposition &decomposition) const HTD_OVERRIDE
        {
            HTD_UNUSED(graph)

            return new htd::FitnessEvaluation(1, -(double) (decomposition.maximumBagSize()));
        }

        WidthMinimizingFitnessFunction *clone(void) const HTD_OVERRIDE
        {
            return new WidthMinimizingFitnessFunction();
        }
    };
}

#endif //CLASP_INCIDENCE_GRAPH_STATS_H
