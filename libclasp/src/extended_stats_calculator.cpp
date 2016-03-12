#include <clasp/extended_stats_calculator.h>

namespace exst
{

    void GraphStatsCalculator::addDep(std::vector <uint32> dependencies, Clasp::VarVec heads, uint32 negative)
    {
        addRuleDependencyGraph(dependencies, heads, negative);
        addRuleIncidenceGraph(dependencies, heads, negative);
    }

    void GraphStatsCalculator::addRuleIncidenceGraph(std::vector <uint32> deps, Clasp::VarVec heads, uint32 negative)
    {
        std::map <int32, htd::vertex_t> &vertexNodeMap = incidenceGraphStats.getAtomVertexMap();
        std::map <int32, htd::vertex_t> &ruleVertexMap = incidenceGraphStats.getRuleVertexMap();

        htd::LabeledHypergraph &graph = incidenceGraphStats.getIncidenceGraph();
        htd::vertex_t vertex = graph.addVertex();

        ruleVertexMap[incidenceGraphStats.getRuleCount()] = vertex;
        incidenceGraphStats.getRuleCount()++;

        // add body atoms
        for (int i = 0; i < deps.size(); ++i)
        {
            if (!graph.isVertex(vertexNodeMap[deps[i]]))
            {
                vertexNodeMap[deps[i]] = graph.addVertex();
            }
            if (!graph.isEdge(vertex, vertexNodeMap[deps[i]]))
            {
                htd::id_t edge_id = graph.addEdge(vertex, vertexNodeMap[deps[i]]);
                graph.setEdgeLabel("negative", edge_id, new htd::Label<bool>(i <= negative));
                graph.setEdgeLabel("head", edge_id, new htd::Label<bool>(false));
            }
        }

        // add head atoms
        for (int i = 0; i < heads.size(); ++i)
        {
            if (!graph.isVertex(vertexNodeMap[heads[i]]))
            {
                vertexNodeMap[heads[i]] = graph.addVertex();
            }
            if (!graph.isEdge(vertex, vertexNodeMap[heads[i]]))
            {
                htd::id_t edge_id = graph.addEdge(vertex, vertexNodeMap[heads[i]]);
                graph.setEdgeLabel("negative", edge_id, new htd::Label<bool>(false));
                graph.setEdgeLabel("head", edge_id, new htd::Label<bool>(true));
            }
        }
    }

    void GraphStatsCalculator::addRuleDependencyGraph(std::vector <uint32> dependencies, Clasp::VarVec heads,
                                                      uint32 negative)
    {
        std::map <int32, htd::vertex_t> &vertexNodeMap = dependencyGraphStats.getAtomVertexMap();
        htd::LabeledHypergraph &graph = dependencyGraphStats.getGraph();

        // add body atoms to graph if they are not in it
        for (int i = 0; i < dependencies.size(); ++i)
        {
            if (!graph.isVertex(vertexNodeMap[dependencies[i]]))
            {
                vertexNodeMap[dependencies[i]] = graph.addVertex();
            }
        }

        // add head atoms to graph if they are not in it
        for (int i = 0; i < heads.size(); ++i)
        {
            if (heads[i] > 1 && !graph.isVertex(vertexNodeMap[heads[i]]))
            {
                vertexNodeMap[heads[i]] = graph.addVertex();
            }
        }

        for (int a = 0; a < heads.size(); ++a)
        {
            for (int b = 0; b < dependencies.size(); ++b)
            {
                if ((heads[a]) > 1 && !graph.isEdge(vertexNodeMap[heads[a]], vertexNodeMap[dependencies[b]]) &&
                    !graph.isEdge(vertexNodeMap[dependencies[b]], vertexNodeMap[heads[a]]))
                {
                    graph.addEdge(vertexNodeMap[heads[a]], vertexNodeMap[dependencies[b]]);
                };
            }
        }

        if (heads.size() == 0 || heads.front() == 1)
        {
            for (int a = 0; a < dependencies.size(); ++a)
            {
                for (int b = 0; b < dependencies.size(); ++b)
                {
                    if ((dependencies[a]) != (dependencies[b]) &&
                        !graph.isEdge(vertexNodeMap[dependencies[a]], vertexNodeMap[dependencies[b]]) &&
                        !graph.isEdge(vertexNodeMap[dependencies[b]], vertexNodeMap[dependencies[a]]))
                    {
                        graph.addEdge(vertexNodeMap[dependencies[a]], vertexNodeMap[dependencies[b]]);
                    };
                }
            }
        }
    }

    void GraphStatsCalculator::labelGraph(const Clasp::SymbolTable &symbolTable)
    {
        labelDepGraph(symbolTable);
        labelInzGraph(symbolTable);
    }

    void GraphStatsCalculator::labelInzGraph(const Clasp::SymbolTable &symbolTable)
    {
        std::map <int32, htd::vertex_t> &litVertexMap = incidenceGraphStats.getAtomVertexMap();
        std::map <int32, htd::vertex_t> &ruleVertexMap = incidenceGraphStats.getRuleVertexMap();
        htd::LabeledHypergraph &graph = incidenceGraphStats.getIncidenceGraph();

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
        std::map <int32, htd::vertex_t> &litVertexMap = dependencyGraphStats.getAtomVertexMap();
        htd::LabeledHypergraph &dependencyGraph = dependencyGraphStats.getGraph();

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
        printEdgeList(dependencyGraphStats.getGraph());
    }

    void GraphStatsCalculator::printIncidenceGraph()
    {
        printEdgeList(incidenceGraphStats.getIncidenceGraph());
    }

    void GraphStatsCalculator::printEdgeList(htd::LabeledHypergraph graph)
    {
        htd::ConstCollection <htd::vertex_t> vertices = graph.vertices();
        printf("\nDependency List: \n");

        for (int a = 0; a < vertices.size(); a++)
        {
            const htd::vertex_t &vertex = vertices[a];
            const htd::ILabel &nameLable = graph.vertexLabel("name", vertex);
            htd::ConstCollection <htd::Hyperedge> edges = graph.hyperedges(vertex);
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

    void deleteBodyEdges(htd::LabeledHypergraph &graph, htd::vertex_t vertex)
    {
        htd::ConstCollection <htd::Hyperedge> edges = graph.hyperedges(vertex);

        for (int i = 0; i < edges.size(); ++i)
        {
            const htd::Hyperedge &edge = edges[i];
            if (!htd::accessLabel<bool>(graph.edgeLabel("head", edge.id())))
            {
                graph.removeEdge(edge.id());
            }
        }
    }

    void GraphStatsCalculator::addAtomReduct(const Clasp::Literal lit)
    {
        bool neg = lit.sign();
        int32 id = lit.var();
        const unsigned int &lid = atomIds[id];
        uint32 vertex = incidenceGraphStats.getAtomVertexMap()[lid];
        htd::LabeledHypergraph &graph = incidenceGraphStats.getIncidenceGraph();
        htd::ConstCollection <htd::Hyperedge> edges = graph.hyperedges(vertex);

        for (int i = 0; i < edges.size(); ++i)
        {
            const htd::Hyperedge &edge = edges[i];
            if (htd::accessLabel<bool>(graph.edgeLabel("negative", edge.id())) != neg &&
                !htd::accessLabel<bool>(graph.edgeLabel("head", edge.id())))
            {
                if (edge[0] != vertex)
                {
                    deleteBodyEdges(graph, edge[0]);
                } else
                {
                    deleteBodyEdges(graph, edge[1]);
                }
            }
        }
    }

    void GraphStatsCalculator::resetAssignment()
    {
        incidenceGraphStats.setMinimalIncidenceGraph(incidenceGraphStats.getIncidenceGraph());
    }

    void GraphStatsCalculator::printIGraphReduct()
    {
        printEdgeList(incidenceGraphStats.getMinimalIncidenceGraph());
    }

    void GraphStatsCalculator::addId(uint32 before, uint32 after)
    {
        atomIds[after] = before;
    }
}