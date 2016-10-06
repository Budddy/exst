#ifndef CLASP_INCIDENCE_GRAPH_STATS_H
#define CLASP_INCIDENCE_GRAPH_STATS_H

#include <unordered_map>
#include <list>
#include <htd/MultiGraphFactory.hpp>
#include <exst/exst_types.h>
#include <clasp/literal.h>

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
        unsigned long long widthCalcInterval = 1000;
        ///
        bool calculateTreeWidth = false;
        ///
        std::string iGraphPath = "";
        ///
        std::string rGraphPath = "";
        ///
        GraphFormat iGraphFormat = NONE;
        ///
        GraphFormat rGraphFormat = NONE;

        ///
        htd::LibraryInstance *libraryInstance;

        ///complete incidence graph
        MyGraph incidenceGraph;
        htd::IMutableMultiGraph *iGraph;

        ///incidence graph of reduct
        MyGraph incidenceGraphReduct;
        htd::IMutableMultiGraph *iGraphReduct;

        ///mapping from atoms to vertices in the incidence graph
        std::unordered_map<uint32_t, uint32_t> atomVertexMap;

        ///maps the rules to the bodies
        MyGraph ruleBodyMapReduct;

        ///maps the rules to the bodies
        MyGraph ruleBodyMap;

        ///maps atoms and ids
        std::unordered_map<uint32_t, uint32_t> *atomIds;

        ///list of the treewidth of the incidence graph reductions
        std::list<float> widths;

        ///the literals that are currently assigned to the solution
        Clasp::LitVec current_assignment;

        ///the literals that are currently assigned to the solution
        int numReducts = 0;
        std::list<std::string> rGraphs;
    };

    class IncidenceGraphStatsCalculator
    {
    public:

        /**
         * Constructor of the IncidenceGraphStats class.
         * @param atomIds used to math atom ids before and after preprocessing
         * @return a new instance of the incidence graph stat calculator
         */
        IncidenceGraphStatsCalculator(std::unordered_map<uint32_t, uint32_t> *atomIds)
        {
            iGraphStats.atomIds = atomIds;
            iGraphStats.libraryInstance = htd::createManagementInstance(htd::Id::FIRST);
            htd::MultiGraphFactory &factory = iGraphStats.libraryInstance->multiGraphFactory();
            iGraphStats.iGraph = factory.getMultiGraph();
        }

        /**
         * Adds a rule to the incidence graph.
         * @param body body of the new rule
         * @param head head of the new rule
         */
        void addRuleIncidenceGraph(std::list<lit_type> body, std::list<lit_type> head);

        /**
         * Tesets the incidence graph of the reduct.
         */
        void resetAssignment();

        /**
         * Reduces the incidence graph.
         * @param lit the literal used to reduce the graph
         */
        void reduceGraph(lit_type lit);

        /**
         * Updates the current assignment.
         * @param new_assignment list of literals in the new assignment
         */
        void updateAssignment(Clasp::LitVec new_assignment);

        /**
         * Getter for the HTD incidence graph.
         * @return the incidence graph in HTD format
         */
        htd::IMutableMultiGraph *getHTDIncidenceGraph()
        {
            return iGraphStats.iGraph;
        }

        /**
         * Getter for the HTD incidence graph of the reduct.
         * @return the incidence graph of the reduct in HTD format
         */
        htd::IMutableMultiGraph *getHTDIncidenceGraphReduct()
        {
            return iGraphStats.iGraphReduct;
        }

        /**
         * Getter for the incidence graph.
         * @return the incidence graph
         */
        MyGraph &getIncidenceGraph()
        {
            return iGraphStats.incidenceGraph;
        }

        /**
         * Getter for the incidence graph of the reduct.
         * @return the incidence graph of the reduct
         */
        MyGraph &getIncidenceGraphReduct()
        {
            return iGraphStats.incidenceGraphReduct;
        }

        /**
         * Getter for the list of Treewidths.
         * @return a list conting the Treewidths
         */
        std::list<float> *getWidths()
        {
            return &iGraphStats.widths;
        }

        std::list<std::string> *getRGraphs()
        {
            return &iGraphStats.rGraphs;
        }

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
