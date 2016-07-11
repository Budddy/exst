#include <exst/incidence_graph_stats.h>
#include <iostream>

namespace exst
{
    void exst::IncidenceGraphStats::addRuleIncidenceGraph(std::list<lit_type> bodies, std::list<lit_type> heads)
    {

        std::unordered_map<uint32_t, std::unordered_map<uint32_t, EdgeType>> &igraph = incidenceGraph;
        std::unordered_map<uint32_t, uint32_t> &avmap = atomVertexMap;
        std::unordered_map<uint32_t, std::unordered_map<uint32_t, EdgeType>> &rbmap = ruleBodyMap;

        uint32_t rule_Vertex = nodecount;
        nodecount++;
        igraph[rule_Vertex];

        // add body atoms
        std::list<exst::lit_type>::iterator it;
        for (it = bodies.begin(); it != bodies.end(); it++)
        {
            uint32_t dId = it->id;
            if (avmap.count(dId) == 0)
            {
                avmap[dId] = nodecount;
                nodecount++;
                igraph[avmap[dId]];
            }
            bool neg = it->s == NEGATIVE;
            rbmap[rule_Vertex][dId] = neg ? NEG : POS;
            igraph[avmap[dId]][rule_Vertex] = neg ? NEG : POS;
            igraph[rule_Vertex][avmap[dId]] = neg ? NEG : POS;
            edgecount++;
        }

        // add head atoms
        for (it = heads.begin(); it != heads.end(); it++)
        {
            uint32_t hId = it->id;
            if (avmap.count(hId) == 0)
            {
                avmap[hId] = nodecount;
                nodecount++;
                igraph[avmap[hId]];
            }
            igraph[avmap[hId]][rule_Vertex] = HEAD;
            igraph[rule_Vertex][avmap[hId]] = HEAD;
            edgecount++;
        }
    }

    void exst::IncidenceGraphStats::addAtomReduct(lit_type lit)
    {
        bool neg = true;
        uint32_t atomId = atomIds[lit.id];
        selectedAtoms[atomId] = neg;
    }

    void exst::IncidenceGraphStats::resetAssignment()
    {
        selectedAtoms.clear();
        nodecountReduct = nodecount;
        edgecountReduct = edgecount;
        ruleBodyMapReduct.insert(ruleBodyMap.begin(), ruleBodyMap.end());
        incidenceGraphReduct = copyMyGraph(incidenceGraph);
    }

    void exst::IncidenceGraphStats::reduceGraph(lit_type lit)
    {
        uint32_t nodeIdBody = atomVertexMap[atomIds[lit.id]];
        MyGraph &igraph = incidenceGraphReduct;
        std::unordered_map<uint32_t, EdgeType> edges = igraph[nodeIdBody];

        std::unordered_map<unsigned int, exst::EdgeType>::iterator it;
        for (it = edges.begin(); it != edges.end(); it++)
        {
            if ((*it).second == (lit.s == POSITIVE ? NEG : POS))
            {
                std::unordered_map<uint32_t, EdgeType> bodies = ruleBodyMapReduct[(*it).first];
                std::unordered_map<unsigned int, exst::EdgeType>::iterator bodyIt;
                for (bodyIt = bodies.begin(); bodyIt != bodies.end(); bodyIt++)
                {
                    if (bodyIt->second == POS || bodyIt->second == NEG)
                    {
                        igraph[bodyIt->first].erase(it->first);
                        ruleBodyMapReduct[it->first].erase(bodyIt->first);
                        igraph[it->first].erase(bodyIt->first);
                        edgecountReduct--;
                    }
                }
            }
        }
        reds.push_back(1.0F*edgecountReduct/edgecount);
        printIGraphReduct();
    }

    void exst::IncidenceGraphStats::printIGraphReduct()
    {
        std::cout << "{\"_Incidence Graph Reduct_\": [ \n  [\"Nodes\", ";
        std::cout << nodecountReduct;
        std::cout << "],\n  [\"Edges\", ";
        std::cout << edgecountReduct;
        std::cout << "]\n}\n";
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