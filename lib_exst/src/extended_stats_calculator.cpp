#include <exst/ExstTypes.h>
#include <exst/graph_stats_calculator.h>
#include <iostream>

namespace exst {

    std::__cxx11::string GraphStatsCalculator::getDIMACS(MyGraph &graph, uint32 edgecount) {
        std::__cxx11::string dimacs;
        dimacs += "p edge " + std::__cxx11::to_string(graph.size()) + " " + std::__cxx11::to_string(edgecount);

        std::unordered_map<unsigned int, std::unordered_map<unsigned int, exst::EdgeType>>::iterator it;
        for (it = graph.begin(); it != graph.end(); it++) {
            std::unordered_map<unsigned int, exst::EdgeType>::iterator it2;
            for (it2 = it->second.begin(); it2 != it->second.end(); it2++) {
                if (it->first < it2->first) {
                    dimacs += "\ne " + std::to_string(it->first);
                    dimacs += " " + std::to_string(it2->first);
                }
            }
        }
        return dimacs;
    }

    void GraphStatsCalculator::parseRule(std::vector<uint32> dependencies, Clasp::PodVector<uint32>::type heads,
                                         uint32 negative) {
        //exclude facts (where id is 0)
        if (dependencies.size() <= 0) return;

        //non horn clause
        if (negative != 0) ++GraphStatsCalculator::numNonHornClauses;
        //non dual horn clause
        if (negative < dependencies.size() - 1) ++GraphStatsCalculator::numNonDualHornClauses;

        //number of clauses
        ++GraphStatsCalculator::numClauses;
        GraphStatsCalculator::addRuleDependencyGraph(dependencies, heads);
        GraphStatsCalculator::addRuleIncidenceGraph(dependencies, heads, negative);
    }

    void GraphStatsCalculator::addId(uint32 before, uint32 after) {
        atomIds[after] = before;
    }

    void GraphStatsCalculator::printExtendedStats() {
        printDepGraph();
        printIncidenceGraph();
        std::cout << "Non Horn Clauses: " << this->numNonHornClauses << "\n";
        std::cout << "Non Dual Horn Clauses: " << this->numNonDualHornClauses << "\n";
        std::cout << "Clauses: " << this->numClauses << "\n";
    }
}