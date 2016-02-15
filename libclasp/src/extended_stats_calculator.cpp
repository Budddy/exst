#include <clasp/extended_stats_calculator.h>
#include <clasp/logic_program_types.h>

namespace exst {
    void GraphStatsCalculator::buildDependencyGraph(const Clasp::Asp::LogicProgram *program) {
        graph = *(new htd::LabeledGraph());
        uint32 atomcount = program->numAtoms();
        uint32 atomNodes[atomcount];
        vertexPairs = (std::pair<uint32, htd::vertex_t> *) malloc(sizeof(std::pair<uint32, htd::vertex_t>) * atomcount);
        std::map<uint32, std::list<uint32>> bodyAtom;
        //add vertices to graph
        for (uint32 i = 0; i < atomcount; i++) {
            Clasp::Asp::PrgAtom *node = program->getAtom(i);
            atomNodes[i] = graph.addVertex();
            vertexPairs[i] = (*new std::pair<uint32, htd::vertex_t>(i, atomNodes[i]));

            //get num of chars in longest label for formatting
            const char *name = program->getAtomName(i);
            if(name!=0) {
                size_t len = strlen(name);
                maxLableLength = maxLableLength > len ? maxLableLength : len;
            }

            //get the bodies the atom is in
            for (Clasp::Asp::PrgAtom::dep_iterator it = node->deps_begin(); it != node->deps_end(); ++it) {
                uint32 idx = it->index();
                bodyAtom[idx].push_back(i);
            }
        }

        //add dependency edges
        for (std::map<uint32, std::list<uint32>>::iterator mapit = bodyAtom.begin(); mapit != bodyAtom.end(); ++mapit) {
            for (std::list<uint32>::iterator it1 = mapit->second.begin(); it1 != mapit->second.end(); ++it1) {
                for (std::list<uint32>::iterator it2 = mapit->second.begin(); it2 != mapit->second.end(); ++it2) {
                    if ((*it1) != (*it2)) {
                        graph.addEdge(atomNodes[*it1], atomNodes[*it2]);
                    }
                }
            }
        }

        //add atom labels
        size_t vertexCount = graph.vertices().size();
        maxIdLength = std::to_string(vertexCount).size();
        maxLableLength = maxLableLength > 8+maxIdLength ? maxLableLength : maxIdLength+8;
        
        for (int i = 0; i < vertexCount; i++) {
            const char *name = program->getAtomName(i);
            htd::Label <std::string> *label;

            //atom has no name
            if (name == nullptr) {
                name = "unknown";
            }

            //name of the atom
            label = new htd::Label<std::string>(name + std::string(maxLableLength - strlen(name), ' '));
            graph.setVertexLabel("name", vertexPairs[i].second, label);

            //id of the atom
            std::string id = std::to_string(vertexPairs[i].first);
            label = new htd::Label<std::string>(id + std::string(maxIdLength - id.length(), ' '));
            graph.setVertexLabel("id", vertexPairs[i].second, label);
        }
    }

    void GraphStatsCalculator::printMatrix(bool printAll) {
        const htd::ConstCollection <htd::vertex_t> &vertices = graph.vertices();
        bool connection = false;
        printf("\nDependency Matrix: \n");
        std::list<htd::vertex_t> vertexList;

        //print first line and gather vertices to print
        printf("%s", std::string(maxIdLength + maxLableLength + 3, ' ').c_str());
        for (int a = 0; a < vertices.size(); a++) {
            htd::ConstCollection <htd::edge_t> collection = graph.edges(vertices[a]);
            if (printAll) {
                vertexList.push_back(vertices[a]);
                graph.vertexLabel("id", vertices[a]).print(std::cout);
                printf(" ");
            } else if (collection.size() > 0) {
                vertexList.push_back(vertices[a]);
                graph.vertexLabel("id", vertices[a]).print(std::cout);
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

    void GraphStatsCalculator::printEdgeList(){
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
            for(int b=0; b < edges.size();b++){
                nameLable.print(std::cout);
                printf(" -> ");
                graph.vertexLabel("name",((edges[b].second == vertex) ? edges[b].first : edges[b].second)).print(std::cout);
                printf("\n");
            }
        }
    }
}