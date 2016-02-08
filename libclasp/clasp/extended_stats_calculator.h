#ifndef CLASP_ABSTRACTEXTENDEDSTATSCALCULATOR_H
#define CLASP_ABSTRACTEXTENDEDSTATSCALCULATOR_H

#include <clasp/dependency_graph.h>
#include <clasp/literal.h>
#include <clasp/shared_context.h>
#include <clasp/logic_program.h>
#include <htd/LabeledGraph.hpp>


namespace exst {
    class ExtendedStatsCalculator {
    };

    class GraphStatsCalculator {
    public:
        void buildGraph(const Clasp::Asp::LogicProgram *program);
        void printGraph();
    private:
        htd::LabeledGraph graph;
    };
}

#endif //CLASP_EXTENDEDSTATSCALCULATOR_H
