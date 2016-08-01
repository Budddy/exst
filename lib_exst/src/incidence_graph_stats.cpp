#include <exst/incidence_graph_stats.h>

namespace exst
{
    void exst::IncidenceGraphStats::addRuleIncidenceGraph(std::list<lit_type> bodies, std::list<lit_type> heads)
    {
        std::unordered_map<uint32_t, std::unordered_map<uint32_t, EdgeType>> &myGraph = incidenceGraph;
        std::unordered_map<uint32_t, uint32_t> &avmap = atomVertexMap;
        std::unordered_map<uint32_t, std::unordered_map<uint32_t, EdgeType>> &rbmap = ruleBodyMap;

        uint32_t rule_Vertex = (uint32_t) myGraph.size();
        myGraph[rule_Vertex];

        // add body atoms
        std::list<exst::lit_type>::iterator it;
        for (it = bodies.begin(); it != bodies.end(); it++)
        {
            uint32_t dId = it->id;
            if (avmap.count(dId) == 0)
            {
                avmap[dId] = (uint32_t) myGraph.size();
                myGraph[avmap[dId]];
            }
            bool neg = it->s == NEGATIVE;
            rbmap[rule_Vertex][avmap[dId]] = neg ? NEG : POS;
            myGraph[avmap[dId]][rule_Vertex] = neg ? NEG : POS;
            myGraph[rule_Vertex][avmap[dId]] = neg ? NEG : POS;
            edgecount++;
        }

        // add head atoms
        std::list<exst::lit_type>::iterator it_;
        for (it_ = heads.begin(); it_ != heads.end(); it_++)
        {
            uint32_t hId = it_->id;
            if (avmap.count(hId) == 0)
            {
                avmap[hId] = (uint32_t) myGraph.size();
                myGraph[avmap[hId]];
            }
            myGraph[avmap[hId]][rule_Vertex] = HEAD;
            myGraph[rule_Vertex][avmap[hId]] = HEAD;
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
                        igraph[it->first].erase(bodyIt->first);
                        ruleBodyMapReduct[it->first].erase(bodyIt->first);
                    }
                }
            }
        }
        std::cout << "get Treewidth: ";
        reds.push_back(getTreewidth(igraph));
        std::cout << reds.back() << "\n";
    }

    void exst::IncidenceGraphStats::printIGraphReduct()
    {
        std::cout << "{\"_Incidence Graph Reduct_\": [ \n  [\"Nodes\", ";
        std::cout << incidenceGraphReduct.size();
        std::cout << "],\n  [\"Edges\", ";
        std::cout << edgeCount(incidenceGraphReduct);
        //std::cout << "],\n  [\"Treewidth\", ";
        //std::cout << getTreewidth(incidenceGraphReduct);
        std::cout << "]\n}\n";
    }


    int exst::IncidenceGraphStats::getTreewidth(MyGraph &graph)
    {
        FILE *file = fopen("test.txt", "w");
        fputs(getGrFormat(graph).c_str(), file);
        fclose(file);
        const char *str = "htd_main --output width --opt width < test.txt";
        //std::cout << str;
        FILE *stream = popen(str, "r");
        char buffer[50];
        fgets(buffer,50,stream);
        //remove("test.txt");
        return atoi(buffer);
    }
}