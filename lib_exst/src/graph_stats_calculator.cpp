#include <exst/graph_stats_calculator.h>
#include <iostream>

namespace exst {
    void GraphStatsCalculator::parseRule(std::vector<uint32> dependencies, Clasp::PodVector<Clasp::Var>::type heads,
                                         uint32 negative) {
        //exclude facts (where id is 0)
        if (dependencies.size() <= 0) return;

        //non horn clause
        if (negative != 0) ++numNonHornClauses;
        //non dual horn clause
        if (negative < dependencies.size() - 1) ++numNonDualHornClauses;

        //number of clauses
        ++numClauses;
        addRuleDependencyGraph(dependencies, heads);
        addRuleIncidenceGraph(dependencies, heads, negative);
    }

    void GraphStatsCalculator::addRuleIncidenceGraph(std::vector<uint32> deps,
                                                     Clasp::PodVector<Clasp::Var>::type heads, uint32 negative) {

        std::unordered_map<uint32, std::unordered_map<uint32, EdgeType>> &igraph = incidenceGraphStats.incidenceGraph;
        std::unordered_map<uint32, uint32> &avmap = incidenceGraphStats.atomVertexMap;
        std::unordered_map<uint32, std::unordered_map<uint32, EdgeType>> &rbmap = incidenceGraphStats.ruleBodyMap;

        uint32 rule_Vertex = incidenceGraphStats.nodecount;
        incidenceGraphStats.nodecount++;
        igraph[rule_Vertex];

        // add body atoms
        for (int i = 0; i < deps.size(); ++i) {
            uint32 dId = deps[i];
            if (avmap.count(dId) == 0) {
                avmap[dId] = incidenceGraphStats.nodecount;
                incidenceGraphStats.nodecount++;
                igraph[avmap[dId]];
            }
            bool neg = i < negative;
            rbmap[rule_Vertex][dId] = neg ? EdgeType::negative : EdgeType::positive;
            incidenceGraphStats.edgecount++;
        }

        // add head atoms
        for (int i = 0; i < heads.size(); ++i) {
            uint32 hId = heads[i];
            if (avmap.count(hId) == 0) {
                avmap[hId] = incidenceGraphStats.nodecount;
                incidenceGraphStats.nodecount++;
                igraph[avmap[hId]];
            }
            igraph[avmap[hId]][rule_Vertex] = EdgeType::head;
            igraph[rule_Vertex][avmap[hId]] = EdgeType::head;
            incidenceGraphStats.edgecount++;
        }
    }

    void GraphStatsCalculator::addRuleDependencyGraph(std::vector<uint32> bodies,
                                                      Clasp::PodVector<Clasp::Var>::type heads) {
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
                    graph[vertexNodeMap[heads[a]]][vertexNodeMap[bodies[b]]] = EdgeType::head;
                    dependencyGraphStats.edgecount++;
                };
            }
        }

        if (heads.size() == 0 || heads.front() == 1) {
            for (int a = 0; a < bodies.size(); ++a) {
                for (int b = 0; b < bodies.size(); ++b) {
                    if ((bodies[a]) != (bodies[b]) && dependencyGraphStats.edgeMap[bodies[a]].count(bodies[b]) == 0) {
                        graph[vertexNodeMap[bodies[a]]][vertexNodeMap[bodies[b]]] = EdgeType::body;
                    };
                }
            }
        }
    }

    void GraphStatsCalculator::addAtomReduct(const Clasp::Literal lit) {
        bool neg = true;
        uint32 atomId = atomIds[lit.var()];
        selectedAtoms[atomId] = neg;
    }

    void GraphStatsCalculator::resetAssignment() {
        std::cout << "reset Incidence graph";
        selectedAtoms.clear();
        incidenceGraphStats.edgecountReduct = incidenceGraphStats.edgecount;
        incidenceGraphStats.incidenceGraphReduct = copyMyGraph(incidenceGraphStats.incidenceGraph);
    }

    void GraphStatsCalculator::reduceGraph(uint32 lit, bool neg) {
        uint32 nodeIdBody = incidenceGraphStats.atomVertexMap[atomIds[lit]];
        MyGraph &igraph = incidenceGraphStats.incidenceGraphReduct;

        std::unordered_map<unsigned int, exst::EdgeType>::iterator it;
        for (it = igraph[nodeIdBody].begin(); it != igraph[nodeIdBody].end(); it++) {
            if ((*it).second == neg ? EdgeType::positive : EdgeType::negative) {
                std::unordered_map<unsigned int, exst::EdgeType>::iterator bodyIt;
                std::unordered_map<uint32, EdgeType> &bodies = incidenceGraphStats.ruleBodyMap[(*it).first];
                for (bodyIt = bodies.begin(); bodyIt != bodies.end(); bodyIt++) {
                    if (bodyIt->second == EdgeType::positive || bodyIt->second == EdgeType::negative) {
                        igraph[bodyIt->first].erase(it->first);
                        incidenceGraphStats.edgecountReduct--;
                    }
                }
            }
        }
        printIGraphReduct();
    }

    void GraphStatsCalculator::addId(uint32 before, uint32 after) {
        atomIds[after] = before;
    }

    void GraphStatsCalculator::printIGraphReduct() {
        std::cout << "\n_Incidence Graph Reduct_ \nNodes: ";
        std::cout << incidenceGraphStats.nodecountReduct;
        std::cout << "\nEdges: ";
        std::cout << incidenceGraphStats.edgecountReduct;
        std::cout << "\n";
    }

    void GraphStatsCalculator::printDepGraph() {
        std::cout << "\n_Dependency Graph_ \nNodes: ";
        std::cout << dependencyGraphStats.nodecount;
        std::cout << "\nEdges: ";
        std::cout << dependencyGraphStats.edgecount;
        std::cout << "\n";
    }

    void GraphStatsCalculator::printIncidenceGraph() {
        std::cout << "_Incidence Graph_ \nNodes: ";
        std::cout << incidenceGraphStats.nodecount;
        std::cout << "\nEdges: ";
        std::cout << incidenceGraphStats.edgecount;
        std::cout << "\n";
    }

    std::string GraphStatsCalculator::getDIMACS(MyGraph &graph, uint32 edgecount) {
        std::string dimacs;
        dimacs += "p edge " + std::to_string(graph.size()) + " " + std::to_string(edgecount);

        MyGraph::iterator it;
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

    void GraphStatsCalculator::printExtendedStats() {
        printDepGraph();
        printIncidenceGraph();
        std::cout << "Non Horn Clauses: " << this->numNonHornClauses << "\n";
        std::cout << "Non Dual Horn Clauses: " << this->numNonDualHornClauses << "\n";
        std::cout << "Clauses: " << this->numClauses << "\n";
    }

    MyGraph copyMyGraph(MyGraph graph) {
        MyGraph newGraph;
        MyGraph::iterator i;
        for (i = graph.begin(); i != graph.end(); i++) {
            newGraph[i->first].insert(i->second.begin(), i->second.end());
        }
        return newGraph;
    }
}