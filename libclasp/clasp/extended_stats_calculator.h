#ifndef CLASP_ABSTRACTEXTENDEDSTATSCALCULATOR_H
#define CLASP_ABSTRACTEXTENDEDSTATSCALCULATOR_H

#include <clasp/dependency_graph.h>
#include <clasp/literal.h>
#include <clasp/shared_context.h>
#include <clasp/logic_program.h>
#include <htd/LabeledGraph.hpp>
#include <bits/stl_list.h>
#include <bits/stl_pair.h>
#include <htd/Label.hpp>

namespace exst {
    class GraphStatsCalculator {

    public:
        static GraphStatsCalculator &getInstance() {
            static GraphStatsCalculator calc;
            return calc;
        }

        void buildDependencyGraph(const Clasp::Asp::LogicProgram *program);

        void printGraph(const Clasp::SymbolTable &sym, bool printAll);

        void lableGraph(const Clasp::SymbolTable &sym);

    private:
        htd::LabeledGraph graph;
        std::pair<uint32, htd::vertex_t> *vertexPairs;
        unsigned long maxLableLength;

        GraphStatsCalculator() { };

        GraphStatsCalculator(GraphStatsCalculator const &);

        void operator=(GraphStatsCalculator const &);
    };
}

#endif //CLASP_EXTENDEDSTATSCALCULATOR_H
