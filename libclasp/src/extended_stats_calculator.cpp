#include <clasp/extended_stats_calculator.h>

namespace exst {

    void GraphStatsCalculator::addDependency(std::list<uint32> deps, Clasp::VarVec heads) {
        std::list<unsigned int>::iterator it;
        for (it = deps.begin(); it != deps.end(); ++it) {
            if (!graph.isVertex(vertexNodeMap[(*it)])) {
                vertexNodeMap[(*it)] = graph.addVertex();
            }
        }
        Clasp::Var *it_heads;
        for (it_heads = heads.begin(); it_heads != heads.end(); ++it_heads) {
            if ((*it_heads) > 1 && !graph.isVertex(vertexNodeMap[(*it_heads)])) {
                vertexNodeMap[(*it_heads)] = graph.addVertex();
            }
        }

        Clasp::Var *it_heads1;
        std::list<unsigned int>::iterator it_bodies;
        for (it_heads1 = heads.begin(); it_heads1 != heads.end(); ++it_heads1) {
            for (it_bodies = deps.begin(); it_bodies != deps.end(); it_bodies++) {
                if ((*it_heads1) > 1 && !graph.isEdge(vertexNodeMap[*it_heads1], vertexNodeMap[*it_bodies]) &&
                    !graph.isEdge(vertexNodeMap[*it_bodies], vertexNodeMap[*it_heads1])) {
                    graph.addEdge(vertexNodeMap[*it_heads1], vertexNodeMap[*it_bodies]);
                };
            }
        }

        std::list<unsigned int>::iterator it1, it2;
        for (it1 = deps.begin(); it1 != deps.end(); ++it1) {
            for (it2 = deps.begin(); it2 != deps.end(); it2++) {
                if (!graph.isEdge(vertexNodeMap[*it1], vertexNodeMap[*it2]) &&
                    !graph.isEdge(vertexNodeMap[*it2], vertexNodeMap[*it1])) {
                    graph.addEdge(vertexNodeMap[*it1], vertexNodeMap[*it2]);
                };
            }
        }
    }

    void GraphStatsCalculator::lableGraph(const Clasp::SymbolTable &symbolTable) {

        for (Clasp::SymbolTable::key_type i = 0; i < vertexNodeMap.size(); i++) {
            if (vertexNodeMap[i] == 0) {
                continue;
            }
            const Clasp::SymbolTable::symbol_type *sym = symbolTable.find(i);
            if (sym != nullptr) {
                maxLableLength =
                        maxLableLength > strlen(sym->name.c_str()) ? maxLableLength : strlen(sym->name.c_str());
            }
        }

        size_t atomcount = graph.vertexCount();
        maxIdLength = std::to_string(atomcount).size();
        maxLableLength = maxLableLength > 8 + maxIdLength ? maxLableLength : maxIdLength + 8;

        for (Clasp::SymbolTable::key_type i = 0; i < vertexNodeMap.size(); i++) {
            if (vertexNodeMap[i] == 0) {
                continue;
            }
            const Clasp::SymbolTable::symbol_type *sym = symbolTable.find(i);
            htd::Label <std::string> *label;

            //atom has no name
            const char *name;
            if (sym == nullptr) {
                name = "unknown";
            } else {
                name = sym->name.c_str();
            }

            //name of the atom
            label = new htd::Label<std::string>(
                    (new std::string(name))->append(std::string(maxLableLength - strlen(name), ' ')));
            graph.setVertexLabel("name", vertexNodeMap[i], label);

            //id of the atom
            std::string id = std::to_string(i);
            label = new htd::Label<std::string>(id.append(std::string(maxIdLength - id.size(), ' ')));
            graph.setVertexLabel("id", vertexNodeMap[i], label);
        }
    }

    void GraphStatsCalculator::printMatrix(bool printAll) {
        const htd::ConstCollection <htd::vertex_t> &vertices = graph.vertices();
        bool connection = false;
        printf("\nDependency Matrix: \n");
        std::list<htd::vertex_t> vertexList;

        //print first line and gather vertices to print
        printf("%s", std::string(maxIdLength + maxLableLength + 3, ' ').c_str());
        for (int a = 0; a < vertexNodeMap.size(); a++) {
            if (vertexNodeMap[a] == 0) {
                continue;
            }
            htd::ConstCollection <htd::edge_t> collection = graph.edges(vertexNodeMap[a]);
            if (printAll) {
                vertexList.push_back(vertexNodeMap[a]);
                graph.vertexLabel("id", vertexNodeMap[a]).print(std::cout);
                printf(" ");
            } else if (collection.size() > 0) {
                vertexList.push_back(vertexNodeMap[a]);
                graph.vertexLabel("id", vertexNodeMap[a]).print(std::cout);
                printf(" ");
            }
        }
        printf("\n");

        for (std::list<unsigned int>::iterator a = vertexList.begin(); a != vertexList.end(); a++) {
            htd::ConstCollection <htd::edge_t> collection = graph.edges(*a);

            //print id of atom
            graph.vertexLabel("id", *a).print(std::cout);
            printf(": ");

            //print name of atom
            graph.vertexLabel("name", *a).print(std::cout);
            printf(" ");

            //print connections of atom
            for (std::list<unsigned int>::iterator b = vertexList.begin(); b != vertexList.end(); b++) {
                for (int i = 0; i < collection.size(); i++) {
                    const htd::edge_t &edge = collection[i];
                    if (a != b && !connection && (edge.second == *b || edge.first == *b)) {
                        printf("X%s", std::string(maxIdLength, ' ').c_str());
                        connection = true;
                    }
                }
                if (!connection) {
                    printf("-%s", std::string(maxIdLength, ' ').c_str());
                }
                connection = false;
            }
            printf("\n");
        }

        printf("\n");
    }

    void GraphStatsCalculator::printEdgeList() {
        const htd::ConstCollection <htd::vertex_t> vertices = graph.vertices();
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
    }
}