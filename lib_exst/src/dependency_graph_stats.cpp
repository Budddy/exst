#include <exst/dependency_graph_stats.h>
#include <iostream>
#include <exst/graph_stats_calculator.h>

namespace exst {
    void GraphStatsCalculator::addRuleDependencyGraph(std::vector<uint32> bodies,
                                                      Clasp::PodVector<uint32>::type heads) {
        std::unordered_map<uint32, uint32> &vertexNodeMap = dependencyGraphStats.atomVertexMap;
        std::unordered_map<uint32, std::unordered_map<uint32, EdgeType>> &graph = dependencyGraphStats.dependencyGraph;

        // add body atoms to graph if they are not in it
        for (int i = 0; i < bodies.size(); ++i) {
            if (vertexNodeMap.count(bodies[i]) == 0) {
                vertexNodeMap[bodies[i]] = dependencyGraphStats.nodecount;
                dependencyGraphStats.nodecount++;
                graph[vertexNodeMap[bodies[i]]];
            }
        }

        // add head atoms to graph if they are not in it
        for (int i = 0; i < heads.size(); ++i) {
            if (vertexNodeMap.count(heads[i]) == 0) {
                vertexNodeMap[heads[i]] = dependencyGraphStats.nodecount;
                dependencyGraphStats.nodecount++;
                vertexNodeMap[heads[i]];
            }
        }

        for (int a = 0; a < heads.size(); ++a) {
            for (int b = 0; b < bodies.size(); ++b) {
                if ((heads[a]) > 1 && dependencyGraphStats.edgeMap[heads[a]].count(bodies[b]) == 0) {
                    graph[vertexNodeMap[heads[a]]][vertexNodeMap[bodies[b]]] = head;
                    dependencyGraphStats.edgecount++;
                };
            }
        }

        if (heads.size() == 0 || heads.front() == 1) {
            for (int a = 0; a < bodies.size(); ++a) {
                for (int b = 0; b < bodies.size(); ++b) {
                    if ((bodies[a]) != (bodies[b]) && dependencyGraphStats.edgeMap[bodies[a]].count(bodies[b]) == 0) {
                        graph[vertexNodeMap[bodies[a]]][vertexNodeMap[bodies[b]]] = body;
                    };
                }
            }
        }
    }

    void GraphStatsCalculator::printDepGraph() {
        std::cout << "\n_Dependency Graph_ \nNodes: ";
        std::cout << dependencyGraphStats.nodecount;
        std::cout << "\nEdges: ";
        std::cout << dependencyGraphStats.edgecount;
        std::cout << "\n";
    }
}