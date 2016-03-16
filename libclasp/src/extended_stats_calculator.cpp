#include <clasp/extended_stats_calculator.h>

namespace exst
{

    void GraphStatsCalculator::addDep(std::vector<uint32> dependencies, Clasp::VarVec heads, uint32 negative)
    {
        //addRuleDependencyGraph(dependencies, heads, negative);
        //addRuleIncidenceGraph(dependencies, heads, negative);
    }

    void GraphStatsCalculator::addRuleIncidenceGraph(std::vector<uint32> deps, Clasp::VarVec heads, uint32 negative)
    {
        std::unordered_map<int32, htd::vertex_t> &vertexNodeMap = incidenceGraphStats.atomVertexMap;
        std::unordered_map<int32, htd::vertex_t> &ruleVertexMap = incidenceGraphStats.ruleVertexMap;

        htd::LabeledHypergraph &iGraph = incidenceGraphStats.incidenceGraph;
        htd::LabeledHypergraph &minIGraph = incidenceGraphStats.minIncidenceGraph;

        std::unordered_map<id_t, IncidenceGraphStats::GraphInfo> &iEdgeInfo = incidenceGraphStats.edgeInfo;

        std::unordered_map<uint32, std::list<uint32>> &ruleBodyMap = incidenceGraphStats.ruleBodyMap;
        std::unordered_map<uint32, std::list<uint32>> &bodyRuleMap = incidenceGraphStats.bodyRuleMap;
        
        htd::vertex_t vertex = iGraph.addVertex();
        minIGraph.addVertex();

        ruleVertexMap[incidenceGraphStats.rules] = vertex;
        uint32 ruleNumber = incidenceGraphStats.rules;
        incidenceGraphStats.rules++;

        // add body atoms
        for (int i = 0; i < deps.size(); ++i)
        {
            if (vertexNodeMap.count(deps[i])==0)
            {
                vertexNodeMap[deps[i]] = iGraph.addVertex();
                minIGraph.addVertex();
            }
            htd::id_t edge_id = iGraph.addEdge(vertex, vertexNodeMap[deps[i]]);
            ruleBodyMap[ruleNumber].push_back(deps[i]);
            bodyRuleMap[deps[i] * (i<negative ?-1:1)].push_back(ruleNumber);
            iEdgeInfo[edge_id].head = false;
            iEdgeInfo[edge_id].negative = i<negative;
        }

        // add head atoms
        for (int i = 0; i < heads.size(); ++i)
        {
            if (vertexNodeMap.count(heads[i])==0)
            {
                vertexNodeMap[heads[i]]= iGraph.addVertex();
                minIGraph.addVertex();
            }
            htd::id_t edge_id = iGraph.addEdge(vertex, vertexNodeMap[heads[i]]);
            minIGraph.addEdge(iGraph.hyperedge(edge_id));
            iEdgeInfo[edge_id].head = true;
            iEdgeInfo[edge_id].negative = false;
        }
    }

    void GraphStatsCalculator::addRuleDependencyGraph(std::vector<uint32> dependencies, Clasp::VarVec heads,
                                                      uint32 negative)
    {
        std::unordered_map<int32, htd::vertex_t> &vertexNodeMap = dependencyGraphStats.atomVertexMap;
        htd::LabeledHypergraph &graph = dependencyGraphStats.dependencyGraph;

        // add body atoms to graph if they are not in it
        for (int i = 0; i < dependencies.size(); ++i)
        {
            if (vertexNodeMap.count(dependencies[i])==0)
            {
                vertexNodeMap[dependencies[i]] = graph.addVertex();
            }
        }

        // add head atoms to graph if they are not in it
        for (int i = 0; i < heads.size(); ++i)
        {
            if (vertexNodeMap.count(heads[i])==0)
            {
                vertexNodeMap[heads[i]] = graph.addVertex();
            }
        }

        for (int a = 0; a < heads.size(); ++a)
        {
            for (int b = 0; b < dependencies.size(); ++b)
            {
                if ((heads[a]) > 1 && dependencyGraphStats.edgeMap[heads[a]].count(dependencies[b])==0)
                                              {
                    graph.addEdge(vertexNodeMap[heads[a]], vertexNodeMap[dependencies[b]]);
                    dependencyGraphStats.edgeMap[heads[a]][dependencies[b]]= heads[a];
                    dependencyGraphStats.edgeMap[dependencies[b]][heads[a]]= dependencies[b];

                };
            }
        }

        if (heads.size() == 0 || heads.front() == 1)
        {
            for (int a = 0; a < dependencies.size(); ++a)
            {
                for (int b = 0; b < dependencies.size(); ++b)
                {
                    if ((dependencies[a]) != (dependencies[b]) && dependencyGraphStats.edgeMap[dependencies[a]].count(dependencies[b])==0)
                    {
                        graph.addEdge(vertexNodeMap[dependencies[a]], vertexNodeMap[dependencies[b]]);
                        dependencyGraphStats.edgeMap[dependencies[a]][dependencies[b]]= dependencies[a];
                        dependencyGraphStats.edgeMap[dependencies[b]][dependencies[a]]= dependencies[b];
                    };
                }
            }
        }
    }

    void GraphStatsCalculator::addAtomReduct(const Clasp::Literal lit)
    {
        bool neg = lit.sign();
        uint32 atomId = atomIds[lit.var()];
        selectedAtoms[atomId] = neg;
    }

    void GraphStatsCalculator::resetAssignment()
    {
        incidenceGraphStats.incidenceGraphReduct = *(incidenceGraphStats.minIncidenceGraph.clone());
        generateReductGraph();
        incidenceGraphStats.reductBodyRuleMap = incidenceGraphStats.bodyRuleMap;
    }
    
    void GraphStatsCalculator::generateReductGraph() {
        for(int i = 0; i<incidenceGraphStats.ruleBodyMap.size();i++){
            bool reduce = false;
            std::list<unsigned int>::iterator body;
            for(body = incidenceGraphStats.ruleBodyMap[i].begin(); body != incidenceGraphStats.ruleBodyMap[i].end(); body++){
                if(selectedAtoms.count(-*body)>0){
                    reduce = true;
                    break;
                }
            }
            if(!reduce){

            }
        }
    }

    void GraphStatsCalculator::printIGraphReduct()
    {
        printEdgeList(incidenceGraphStats.incidenceGraphReduct);
    }

    void GraphStatsCalculator::addId(uint32 before, uint32 after)
    {
        atomIds[after] = before;
    }

    void GraphStatsCalculator::labelGraph(const Clasp::SymbolTable &symbolTable)
    {
        labelDepGraph(symbolTable);
        labelInzGraph(symbolTable);
    }

    void GraphStatsCalculator::labelInzGraph(const Clasp::SymbolTable &symbolTable)
    {
        std::unordered_map<int32, htd::vertex_t> &litVertexMap = incidenceGraphStats.atomVertexMap;
        std::unordered_map<int32, htd::vertex_t> &ruleVertexMap = incidenceGraphStats.ruleVertexMap;
        htd::LabeledHypergraph &graph = incidenceGraphStats.incidenceGraph;

        for (Clasp::SymbolTable::key_type i = 0; i < litVertexMap.size(); i++)
        {
            if (litVertexMap[i] == 0)
            {
                continue;
            }
            const Clasp::SymbolTable::symbol_type *sym = symbolTable.find(i);

            //atom has no name
            const char *name;
            if (sym == nullptr)
            {
                name = "unknown";
            } else
            {
                name = sym->name.c_str();
            }

            //name of the atom
            graph.setVertexLabel("name", litVertexMap[i], new htd::Label<std::string>(name));

            //id of the atom
            std::string id = std::to_string(i);
            graph.setVertexLabel("id", litVertexMap[i], new htd::Label<std::string>(id.c_str()));
        }

        for (Clasp::SymbolTable::key_type i = 0; i < ruleVertexMap.size(); i++)
        {
            if (ruleVertexMap[i] == 0)
            {
                continue;
            }

            //name of the atom
            std::string name = "rule:";
            name.append(std::to_string(i));
            graph.setVertexLabel("name", ruleVertexMap[i], new htd::Label<std::string>(name.c_str()));

            //id of the atom
            std::string id = "r_";
            id.append(std::to_string(i));
            graph.setVertexLabel("id", ruleVertexMap[i], new htd::Label<std::string>(id.c_str()));
        }
    }

    void GraphStatsCalculator::labelDepGraph(const Clasp::SymbolTable &symbolTable)
    {
        std::unordered_map<int32, htd::vertex_t> &litVertexMap = dependencyGraphStats.atomVertexMap;
        htd::LabeledHypergraph &dependencyGraph = dependencyGraphStats.dependencyGraph;

        for (Clasp::SymbolTable::key_type i = 0; i < litVertexMap.size(); i++)
        {
            if (litVertexMap[i] == 0)
            {
                continue;
            }
            const Clasp::SymbolTable::symbol_type *sym = symbolTable.find(i);

            //atom has no name
            const char *name;
            if (sym == nullptr)
            {
                name = "unknown";
            } else
            {
                name = sym->name.c_str();
            }

            //name of the atom
            dependencyGraph.setVertexLabel("name", litVertexMap[i], new htd::Label<std::string>(name));

            //id of the atom
            std::string id = std::to_string(i);
            dependencyGraph.setVertexLabel("id", litVertexMap[i], new htd::Label<std::string>(id.c_str()));
        }
    }

    void GraphStatsCalculator::printDepGraph()
    {
        printEdgeList(dependencyGraphStats.dependencyGraph);
    }

    void GraphStatsCalculator::printIncidenceGraph()
    {
        printEdgeList(incidenceGraphStats.incidenceGraph);
    }

    void GraphStatsCalculator::printEdgeList(htd::LabeledHypergraph &graph)
    {
        htd::ConstCollection<htd::vertex_t> vertices = graph.vertices();
        printf("\nEdge List: \n");

        for (int a = 0; a < vertices.size(); a++)
        {
            const htd::vertex_t &vertex = vertices[a];
            const htd::ILabel &nameLable = graph.vertexLabel("name", vertex);
            htd::ConstCollection<htd::Hyperedge> edges = graph.hyperedges(vertex);
            graph.vertexLabel("id", vertex).print(std::cout);
            printf(": ");
            nameLable.print(std::cout);
            printf("\n");
            for (int b = 0; b < edges.size(); b++)
            {
                htd::vertex_t v = edges[b][0] == vertex ? edges[b][1] : edges[b][0];
                printf(" -> ");
                graph.vertexLabel("id", v).print(std::cout);
                printf(": ");
                graph.vertexLabel("name", v).print(std::cout);
                printf("\n");
            }
        }

        std::flush(std::cout);
    }

}