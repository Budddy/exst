#include <exst/incidence_graph_stats.h>
#include <exst/graph_stats_calculator.h>
#include <iostream>

namespace exst
{
    void GraphStatsCalculator::addRuleIncidenceGraph(std::vector<uint32> deps, Clasp::PodVector<uint32>::type heads,
                                                     uint32 negative)
    {

        std::unordered_map<uint32, std::unordered_map<uint32, EdgeType>> &igraph = incidenceGraphStats.incidenceGraph;
        std::unordered_map<uint32, uint32> &avmap = incidenceGraphStats.atomVertexMap;
        std::unordered_map<uint32, std::unordered_map<uint32, EdgeType>> &rbmap = incidenceGraphStats.ruleBodyMap;

        uint32 rule_Vertex = incidenceGraphStats.nodecount;
        incidenceGraphStats.nodecount++;
        igraph[rule_Vertex];

        // add body atoms
        for (uint32 i = 0; i < deps.size(); ++i)
        {
            uint32 dId = deps[i];
            if (avmap.count(dId) == 0)
            {
                avmap[dId] = incidenceGraphStats.nodecount;
                incidenceGraphStats.nodecount++;
                igraph[avmap[dId]];
            }
            bool neg = i < negative;
            rbmap[rule_Vertex][dId] = neg ? EdgeType::negative : EdgeType::positive;
            incidenceGraphStats.edgecount++;
        }

        // add head atoms
        for (uint32 i = 0; i < heads.size(); ++i)
        {
            uint32 hId = heads[i];
            if (avmap.count(hId) == 0)
            {
                avmap[hId] = incidenceGraphStats.nodecount;
                incidenceGraphStats.nodecount++;
                igraph[avmap[hId]];
            }
            igraph[avmap[hId]][rule_Vertex] = head;
            igraph[rule_Vertex][avmap[hId]] = head;
            incidenceGraphStats.edgecount++;
        }
    }

    void GraphStatsCalculator::addAtomReduct(const Clasp::Literal lit)
    {
        bool neg = true;
        uint32 atomId = atomIds[lit.var()];
        selectedAtoms[atomId] = neg;
    }

    void GraphStatsCalculator::resetAssignment()
    {
        std::cout << "reset Incidence graph";
        selectedAtoms.clear();
        incidenceGraphStats.edgecountReduct = incidenceGraphStats.edgecount;
        incidenceGraphStats.incidenceGraphReduct = copyMyGraph(incidenceGraphStats.incidenceGraph);
    }

    void GraphStatsCalculator::reduceGraph(uint32 lit, bool neg)
    {
        uint32 nodeIdBody = incidenceGraphStats.atomVertexMap[atomIds[lit]];
        MyGraph &igraph = incidenceGraphStats.incidenceGraphReduct;

        std::unordered_map<unsigned int, EdgeType>::iterator it;
        for (it = igraph[nodeIdBody].begin(); it != igraph[nodeIdBody].end(); it++)
        {
            if ((*it).second == neg ? positive : negative)
            {
                std::unordered_map<uint32, EdgeType> &bodies = incidenceGraphStats.ruleBodyMap[(*it).first];
                std::unordered_map<unsigned int, EdgeType>::iterator bodyIt;
                for (bodyIt = bodies.begin(); bodyIt != bodies.end(); bodyIt++)
                {
                    if (bodyIt->second == positive || bodyIt->second == negative)
                    {
                        igraph[bodyIt->first].erase(it->first);
                        incidenceGraphStats.edgecountReduct--;
                    }
                }
            }
        }
        printIGraphReduct();
    }

    void GraphStatsCalculator::printIGraphReduct()
    {
        std::cout << "\n_Incidence Graph Reduct_ \nNodes: ";
        std::cout << incidenceGraphStats.nodecountReduct;
        std::cout << "\nEdges: ";
        std::cout << incidenceGraphStats.edgecountReduct;
        std::cout << "\n";
    }

    void GraphStatsCalculator::printIncidenceGraph()
    {
        std::cout << "_Incidence Graph_ \nNodes: ";
        std::cout << incidenceGraphStats.nodecount;
        std::cout << "\nEdges: ";
        std::cout << incidenceGraphStats.edgecount;
        std::cout << "\n";
    }
}