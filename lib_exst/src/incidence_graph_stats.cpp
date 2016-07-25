#include <exst/incidence_graph_stats.h>

namespace exst
{
    void exst::IncidenceGraphStats::addRuleIncidenceGraph(std::list<lit_type> bodies, std::list<lit_type> heads)
    {
        std::unordered_map<uint32_t, std::unordered_map<uint32_t, EdgeType>> &myGraph = incidenceGraph;
        std::unordered_map<uint32_t, uint32_t> &avmap = atomVertexMap;
        std::unordered_map<uint32_t, std::unordered_map<uint32_t, EdgeType>> &rbmap = ruleBodyMap;

        uint32_t rule_Vertex = iGraph.addVertex();
        myGraph[rule_Vertex];

        // add body atoms
        std::list<exst::lit_type>::iterator it;
        for (it = bodies.begin(); it != bodies.end(); it++)
        {
            uint32_t dId = it->id;
            if (avmap.count(dId) == 0)
            {
                avmap[dId] = iGraph.addVertex();
                myGraph[avmap[dId]];
            }
            bool neg = it->s == NEGATIVE;
            rbmap[rule_Vertex][avmap[dId]] = neg ? NEG : POS;
            myGraph[avmap[dId]][rule_Vertex] = neg ? NEG : POS;
            myGraph[rule_Vertex][avmap[dId]] = neg ? NEG : POS;

            iGraph.addEdge(avmap[dId],rule_Vertex);
            edgecount++;
        }

        // add head atoms
        std::list<exst::lit_type>::iterator it_;
        for (it_ = heads.begin(); it_ != heads.end(); it_++)
        {
            uint32_t hId = it_->id;
            if (avmap.count(hId) == 0)
            {
                avmap[hId] = iGraph.addVertex();
                myGraph[avmap[hId]];
            }
            myGraph[avmap[hId]][rule_Vertex] = HEAD;
            myGraph[rule_Vertex][avmap[hId]] = HEAD;
            iGraph.addEdge(avmap[hId],rule_Vertex);
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
        ruleBodyMapReduct.insert(ruleBodyMap.begin(), ruleBodyMap.end());
        edgecountReduct = edgecount;
        incidenceGraphReduct = copyMyGraph(incidenceGraph);
        iGraphReduct = iGraph;
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
                        iGraphReduct.removeEdge(bodyIt->first,it->first);
                        iGraphReduct.removeEdge(it->first,bodyIt->first);
                        edgecountReduct--;
                    }
                }
            }
        }
        reds.push_back(1.0F * iGraphReduct.edgeCount());
        reds.push_back(1.0F * edgecountReduct * 100);
    }

    void exst::IncidenceGraphStats::printIGraphReduct()
    {
        std::cout << "{\"_Incidence Graph Reduct_\": [ \n  [\"Nodes\", ";
        std::cout << iGraphReduct.vertexCount();
        std::cout << "],\n  [\"Edges\", ";
        std::cout << iGraphReduct.edgeCount();
        //std::cout << "],\n  [\"Treewidth\", ";
        //std::cout << getTreewidth(iGraphReduct);
        std::cout << "]\n}\n";
    }


    int exst::IncidenceGraphStats::getTreewidth(htd::Hypergraph &graph){
        return 0;
    }
}