#include <exst/incidence_graph_stats.h>
#include <iostream>

namespace exst
{
    void exst::IncidenceGraphStats::addRuleIncidenceGraph(Clasp::WeightLitVec deps,
                                                          Clasp::PodVector<uint32>::type heads, uint32 negative)
    {

        std::unordered_map<uint32, std::unordered_map<uint32, EdgeType>> &igraph = incidenceGraph;
        std::unordered_map<uint32, uint32> &avmap = atomVertexMap;
        std::unordered_map<uint32, std::unordered_map<uint32, EdgeType>> &rbmap = ruleBodyMap;

        uint32 rule_Vertex = nodecount;
        nodecount++;
        igraph[rule_Vertex];

        // add body atoms
        for (uint32 i = 0; i < deps.size(); ++i)
        {
            uint32 dId = deps[i].first.index();
            if (avmap.count(dId) == 0)
            {
                avmap[dId] = nodecount;
                nodecount++;
                igraph[avmap[dId]];
            }
            bool neg = i < negative;
            rbmap[rule_Vertex][dId] = neg ? exst::negative : positive;
            edgecount++;
        }

        // add head atoms
        for (uint32 i = 0; i < heads.size(); ++i)
        {
            uint32 hId = heads[i];
            if (avmap.count(hId) == 0)
            {
                avmap[hId] = nodecount;
                nodecount++;
                igraph[avmap[hId]];
            }
            igraph[avmap[hId]][rule_Vertex] = head;
            igraph[rule_Vertex][avmap[hId]] = head;
            edgecount++;
        }
    }

    void exst::IncidenceGraphStats::addAtomReduct(const Clasp::Literal lit)
    {
        bool neg = true;
        uint32 atomId = atomIds[lit.var()];
        selectedAtoms[atomId] = neg;
    }

    void exst::IncidenceGraphStats::resetAssignment()
    {
        std::cout << "reset Incidence graph";
        selectedAtoms.clear();
        nodecountReduct = nodecount;
        edgecountReduct = edgecount;
        incidenceGraphReduct = copyMyGraph(incidenceGraph);
    }

    void exst::IncidenceGraphStats::reduceGraph(uint32 lit, bool neg)
    {
        uint32 nodeIdBody = atomVertexMap[atomIds[lit]];
        MyGraph &igraph = incidenceGraphReduct;

        std::unordered_map<unsigned int, exst::EdgeType>::iterator it;
        for (it = igraph[nodeIdBody].begin(); it != igraph[nodeIdBody].end(); it++)
        {
            if ((*it).second == neg ? positive : negative)
            {
                std::unordered_map<uint32, EdgeType> &bodies = ruleBodyMap[(*it).first];
                std::unordered_map<unsigned int, exst::EdgeType>::iterator bodyIt;
                for (bodyIt = bodies.begin(); bodyIt != bodies.end(); bodyIt++)
                {
                    if (bodyIt->second == positive || bodyIt->second == negative)
                    {
                        igraph[bodyIt->first].erase(it->first);
                        edgecountReduct--;
                    }
                }
            }
        }
        printIGraphReduct();
    }

    void exst::IncidenceGraphStats::printIGraphReduct()
    {
        std::cout << "\n_Incidence Graph Reduct_ \nNodes: ";
        std::cout << nodecountReduct;
        std::cout << "\nEdges: ";
        std::cout << edgecountReduct;
        std::cout << "\n";
    }

    void exst::IncidenceGraphStats::printIncidenceGraph()
    {
        std::cout << "_Incidence Graph_ \nNodes: ";
        std::cout << nodecount;
        std::cout << "\nEdges: ";
        std::cout << edgecount;
        std::cout << "\n";
    }
}