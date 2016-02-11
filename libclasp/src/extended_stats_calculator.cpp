#include <clasp/extended_stats_calculator.h>
#include <htd/Label.hpp>

namespace exst {
    void GraphStatsCalculator::buildDependencyGraph(const Clasp::Asp::LogicProgram *program) {
        graph = *(new htd::LabeledGraph());
        uint32 atomcount = program->numAtoms();
        uint32 atomNodes[atomcount];
        vertexPairs = (std::pair<uint32, htd::vertex_t> *) malloc(sizeof(std::pair<uint32, htd::vertex_t>) * atomcount);
        std::map<uint32, std::list<uint32>> bodyAtom;
        for (int i = 0; i < atomcount; i++) {
            Clasp::Asp::PrgAtom *node = program->getAtom(i);
            atomNodes[i] = graph.addVertex();
            std::pair<uint32, htd::vertex_t> *vertexpair = new std::pair<uint32, htd::vertex_t>(i, atomNodes[i]);
            vertexPairs[i] = (*vertexpair);

            for (Clasp::Asp::PrgAtom::dep_iterator it = node->deps_begin(); it != node->deps_end(); ++it) {
                uint32 idx = it->index();
                bodyAtom[idx].push_back(i);
            }
        }
        for (std::map<uint32, std::list<uint32>>::iterator mapit = bodyAtom.begin(); mapit != bodyAtom.end(); ++mapit) {
            for (std::list<uint32>::iterator it1 = mapit->second.begin(); it1 != mapit->second.end(); ++it1) {
                for (std::list<uint32>::iterator it2 = mapit->second.begin(); it2 != mapit->second.end(); ++it2) {
                    if (*it1 != *it2) {
                        graph.addEdge(atomNodes[*it1], atomNodes[*it2]);
                    }
                }
            }
        }
    }

    void GraphStatsCalculator::lableGraph(const Clasp::SymbolTable &sym) {
        size_t vertexCount = graph.vertices().size();
        //get longest name
        for (int i = 0; i < vertexCount; i++) {
            const Clasp::SymbolTable::symbol_type *symbol = sym.find(vertexPairs[i].first);
            std::string name;
            if (symbol != nullptr) {
                name = sym.find(vertexPairs[i].first)->name.c_str();
            } else {
                name = "unknown " + std::to_string(i);
            }
            maxLableLength = maxLableLength > name.length() ? maxLableLength : name.length();
        }
        //add labels to graph
        for (int i = 0; i < vertexCount; i++) {
            const Clasp::SymbolTable::symbol_type *symbol = sym.find(vertexPairs[i].first);
            htd::Label <std::string> *label;
            std::string name;
            if (symbol != nullptr) {
                name = sym.find(vertexPairs[i].first)->name.c_str();
            } else {
                name = "unknown " + std::to_string(i);
            }
            label = new htd::Label<std::string>(name + std::string(maxLableLength - name.length(), ' '));
            graph.setVertexLabel("name", vertexPairs[i].second, label);
        }
    }

    void GraphStatsCalculator::printGraph(const Clasp::SymbolTable &sym, bool printAll) {
        this->lableGraph(sym);
        const htd::ConstCollection <htd::vertex_t> &vertices = graph.vertices();
        bool connection = false;
        printf("\nPrinting dependency matrix: \n");
        std::list<htd::vertex_t> vertexList;
        for (int a = 0; a < vertices.size(); a++) {
            htd::ConstCollection <htd::edge_t> collection = graph.edges(vertices[a]);
            if (printAll) {
                vertexList.push_back(vertices[a]);
            } else if (collection.size() > 0) {
                vertexList.push_back(vertices[a]);
            }
        }
        for (std::list<unsigned int>::iterator a = vertexList.begin(); a != vertexList.end(); a++) {
            htd::ConstCollection <htd::edge_t> collection = graph.edges(*a);
            graph.vertexLabel("name", *a).print(std::cout);
            printf("\t");
            for (std::list<unsigned int>::iterator b = vertexList.begin(); b != vertexList.end(); b++) {
                for (int i = 0; i < collection.size(); i++) {
                    const htd::edge_t &edge = collection[i];
                    if (a != b && !connection && (edge.second == *b || edge.first == *b)) {
                        printf("X ");
                        connection = true;
                    }
                }
                if (!connection) {
                    printf("- ");
                }
                connection = false;
            }
            printf("\n");
        }

        printf("\n");
    }
}