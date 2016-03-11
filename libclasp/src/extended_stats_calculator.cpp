#include <clasp/extended_stats_calculator.h>

namespace exst {

    void GraphStatsCalculator::addDep(std::vector<uint32> deps, Clasp::VarVec heads, uint32 negative) {
        addRuleDependencyGraph(deps, heads, negative);
        addRuleIncidenceGraph(deps, heads, negative);
    }

    void GraphStatsCalculator::addRuleIncidenceGraph(std::vector<uint32> deps, Clasp::VarVec heads, uint32 negative) {
        std::map<int32, htd::vertex_t> &vertexNodeMap = istats.getLitVertexMap();
        std::map<int32, htd::vertex_t> &ruleVertexMap = istats.getRuleVertexMap();
        htd::LabeledGraph &graph = istats.getGraph();
        htd::vertex_t vertex = graph.addVertex();
        ruleVertexMap[istats.getRuleCount()] = vertex;
        istats.getRuleCount()++;

        // add body atoms
        for (int i =0; i< deps.size();++i) {
            if (!graph.isVertex(vertexNodeMap[deps[i]])) {
                vertexNodeMap[deps[i]] = graph.addVertex();
            }
            if(!graph.isEdge(vertex,vertexNodeMap[deps[i]])){
                htd::id_t edge_id = graph.addEdge(vertex,vertexNodeMap[deps[i]]);
                graph.setEdgeLabel("negative",edge_id,new htd::Label<bool>(i<=negative));
                graph.setEdgeLabel("head",edge_id,new htd::Label<bool>(false));
            }
        }

        // add head atoms
        for (int i =0;i< heads.size(); ++i) {
            if (!graph.isVertex(vertexNodeMap[heads[i]])) {
                vertexNodeMap[heads[i]] = graph.addVertex();
            }
            if(!graph.isEdge(vertex,vertexNodeMap[heads[i]])){
                htd::id_t edge_id = graph.addEdge(vertex, vertexNodeMap[heads[i]]);
                graph.setEdgeLabel("negative",edge_id,new htd::Label<bool>(false));
                graph.setEdgeLabel("head",edge_id,new htd::Label<bool>(true));
            }
        }
    }

    void GraphStatsCalculator::addRuleDependencyGraph(std::vector<uint32> deps, Clasp::VarVec heads, uint32 negative) {
        std::map<int32, htd::vertex_t> &vertexNodeMap = dstats.getLitVertexMap();
        htd::LabeledGraph &graph = dstats.getGraph();
        // add body atoms to graph if they are not in it
        for (int i =0; i< deps.size();++i) {
            if (!graph.isVertex(vertexNodeMap[deps[i]])) {
                vertexNodeMap[deps[i]] = graph.addVertex();
            }
        }

        // add head atoms to graph if they are not in it
        for (int i =0;i< heads.size(); ++i) {
            if (heads[i] > 1 && !graph.isVertex(vertexNodeMap[heads[i]])) {
                vertexNodeMap[heads[i]] = graph.addVertex();
            }
        }

        for (int a =0;a< heads.size();++a) {
            for (int b =0;b<deps.size();++b) {
                if ((heads[a]) > 1 && !graph.isEdge(vertexNodeMap[heads[a]], vertexNodeMap[deps[b]]) &&
                    !graph.isEdge(vertexNodeMap[deps[b]], vertexNodeMap[heads[a]])) {
                    graph.addEdge(vertexNodeMap[heads[a]], vertexNodeMap[deps[b]]);
                };
            }
        }

        if (heads.size() == 0 || heads.front() == 1) {
            for (int a =0;a<deps.size();++a) {
                for (int b =0;b<deps.size();++b) {
                    if ((deps[a]) != (deps[b]) && !graph.isEdge(vertexNodeMap[deps[a]], vertexNodeMap[deps[b]]) && !graph.isEdge(vertexNodeMap[deps[b]], vertexNodeMap[deps[a]])) {
                        graph.addEdge(vertexNodeMap[deps[a]], vertexNodeMap[deps[b]]);
                    };
                }
            }
        }
    }

    void GraphStatsCalculator::lableGraph(const Clasp::SymbolTable &symbolTable) {
        lableDepGraph(symbolTable);
        lableInzGraph(symbolTable);
    }
    
    void GraphStatsCalculator::lableInzGraph(const Clasp::SymbolTable &symbolTable) {
        std::map<int32, htd::vertex_t> &litVertexMap = istats.getLitVertexMap();
        std::map<int32, htd::vertex_t> &ruleVertexMap = istats.getRuleVertexMap();
        htd::LabeledGraph &graph = istats.getGraph();

        for (Clasp::SymbolTable::key_type i = 0; i < litVertexMap.size(); i++) {
            if (litVertexMap[i] == 0) {
                continue;
            }
            const Clasp::SymbolTable::symbol_type *sym = symbolTable.find(i);

            //atom has no name
            const char *name;
            if (sym == nullptr) {
                name = "unknown";
            } else {
                name = sym->name.c_str();
            }

            //name of the atom
            graph.setVertexLabel("name", litVertexMap[i], new htd::Label<std::string>(name));

            //id of the atom
            std::string id = std::to_string(i);
            graph.setVertexLabel("id", litVertexMap[i], new htd::Label<std::string>(id.c_str()));
        }

        for (Clasp::SymbolTable::key_type i = 0; i < ruleVertexMap.size(); i++) {
            if (ruleVertexMap[i] == 0) {
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
    
    void GraphStatsCalculator::lableDepGraph(const Clasp::SymbolTable &symbolTable) {
        std::map<int32, htd::vertex_t> &litVertexMap = dstats.getLitVertexMap();
        htd::LabeledGraph &dependencyGraph = dstats.getGraph();

        for (Clasp::SymbolTable::key_type i = 0; i < litVertexMap.size(); i++) {
            if (litVertexMap[i] == 0) {
                continue;
            }
            const Clasp::SymbolTable::symbol_type *sym = symbolTable.find(i);

            //atom has no name
            const char *name;
            if (sym == nullptr) {
                name = "unknown";
            } else {
                name = sym->name.c_str();
            }

            //name of the atom
            dependencyGraph.setVertexLabel("name", litVertexMap[i], new htd::Label<std::string>(name));

            //id of the atom
            std::string id = std::to_string(i);
            dependencyGraph.setVertexLabel("id", litVertexMap[i], new htd::Label<std::string>(id.c_str()));
        }
    }

    void GraphStatsCalculator::printDepGraph() {
        printEdgeList(dstats.getGraph());
    }

    void GraphStatsCalculator::printIncidenceGraph() {
        printEdgeList(istats.getGraph());
    }

    void GraphStatsCalculator::printEdgeList(htd::LabeledGraph graph) {
        htd::ConstCollection <htd::vertex_t> vertices = graph.vertices();
        printf("\nDependency List: \n");
        for (int a = 0; a < vertices.size(); a++) {
            const htd::vertex_t &vertex = vertices[a];
            const htd::ILabel &nameLable = graph.vertexLabel("name", vertex);
            htd::ConstCollection <htd::edge_t> edges = graph.edges(vertex);
            graph.vertexLabel("id", vertex).print(std::cout);
            printf(": ");
            nameLable.print(std::cout);
            printf("\n");
            for (int b = 0; b < edges.size(); b++) {
                htd::vertex_t v = edges[b].first == vertex ? edges[b].second : edges[b].first;
                printf(" -> ");
                graph.vertexLabel("id", v).print(std::cout);
                printf(": ");
                graph.vertexLabel("name", v).print(std::cout);
                printf("\n");
            }
        }
        std::flush(std::cout);
    }

    void deleteBodyEdges(htd::LabeledGraph &graph, htd::vertex_t vertex) {

        const htd::ConstCollection<htd::edge_t> &edges = graph.edges(vertex);
        for(int i = 0; i<edges.size();++i){
            const htd::edge_t &edge = edges[i];
            htd::ConstCollection<htd::id_t> eid = graph.associatedEdgeIds(edge.first, edge.second);
            if(eid.size()==0){
                eid = graph.associatedEdgeIds(edge.second, edge.first);
            }
            if(eid.size()>0 && !htd::accessLabel<bool>(graph.edgeLabel("head",eid[0]))){
                const htd::id_t &id = eid[0];
                graph.removeEdge(id);
            }
        }
    }

    void GraphStatsCalculator::addAtomReduct(const Clasp::Literal lit) {
        bool neg = lit.sign();
        int32 id = lit.var();
        const unsigned int &lid = litIds[id];
        uint32 vertex = istats.getLitVertexMap()[lid];
        htd::LabeledGraph &graph = istats.getGraph();
        const htd::ConstCollection<htd::edge_t> &edges = graph.edges(vertex);
        for(int i = 0; i<edges.size();++i){
            const htd::edge_t &edge = edges[i];
            htd::ConstCollection<htd::id_t> eid = graph.associatedEdgeIds(edge.first, edge.second);
            if(eid.size()==0){
               eid = graph.associatedEdgeIds(edge.second, edge.first);
            }
            if(eid.size()>0 && htd::accessLabel<bool>(graph.edgeLabel("negative",eid[0]))!=neg && !htd::accessLabel<bool>(graph.edgeLabel("head",eid[0]))){
                if(edge.first!=vertex){
                    deleteBodyEdges(graph, edge.first);
                }else{
                    deleteBodyEdges(graph, edge.second);
                }
            }
        }
    }

    void GraphStatsCalculator::resetAssignment() {
        istats.setCopy(istats.getGraph());
    }

    void GraphStatsCalculator::printIGraphReduct() {
        printEdgeList(istats.getCopy());
    }

    void GraphStatsCalculator::addId(uint32 before, uint32 after){
        litIds[after] = before;
    }
}