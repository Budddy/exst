#include <clasp/extended_stats_calculator.h>
#include <htd/Label.hpp>
#include <stdio.h>
#include <clasp/logic_program_types.h>
#include <bits/stl_list.h>

namespace exst {
    void GraphStatsCalculator::buildGraph(const Clasp::Asp::LogicProgram *program) {
        graph = *(new htd::LabeledGraph());
        uint32 atomcount = program->numAtoms();
        uint32 bodycount = program->numBodies();
        uint32 atomNodes[atomcount];
        std::list<uint32> bodyAtom[500];
        for (int i = 0; i < atomcount; i++) {
            Clasp::Asp::PrgAtom *node = program->getAtom(i);
            atomNodes[i] = graph.addVertex();
            htd::ILabel *label;
            label = new htd::Label<std::string>("atom_" + std::to_string(i));
            graph.setVertexLabel("name", atomNodes[i], label);

            for (Clasp::Asp::PrgAtom::dep_iterator it = node->deps_begin(); it != node->deps_end(); ++it) {
                uint32 idx = it->index();
                bodyAtom[idx].push_back(i);
            }
        }
        for (int i = 0; i < bodycount; ++i) {
            for (std::list<uint32>::iterator it1 = bodyAtom[i].begin(); it1 != bodyAtom[i].end(); ++it1) {
                for (std::list<uint32>::iterator it2 = bodyAtom[i].begin(); it2 != bodyAtom[i].end(); ++it2) {
                    if (*it1 != *it2) {
                        graph.addEdge(atomNodes[*it1], atomNodes[*it2]);
                    }
                }
            }
        }
        /*
        for(int i =0; i<bodycount; i++){
            Clasp::Asp::PrgBody *node = program->getBody(i);
            bodyNodes[i] = graph.addVertex();
            htd::ILabel *label;
            label = new htd::Label<std::string>("body_" + std::to_string(i));
            graph.setVertexLabel("name", bodyNodes[i], label);
        }*/

        printGraph();
    }
    void GraphStatsCalculator::printGraph() {
        const htd::ConstCollection <htd::vertex_t> &vertices = graph.vertices();
        bool connection = false;
        printf("\nprinting graph: \n");
        for (int a = 0; a < vertices.size(); a++) {
            htd::ConstCollection <htd::edge_t> collection = graph.edges(vertices[a]);
            graph.vertexLabel("name", vertices[a]).print(std::cout);
            printf("\t");
            for (int b = 0; b < vertices.size(); b++) {
                for (int i = 0; i < collection.size(); i++) {
                    const htd::edge_t &edge = collection[i];
                    if (!connection && edge.second == vertices[b]) {
                        printf("1 ");
                        connection = true;
                    }
                }
                if (!connection) {
                    printf("0 ");
                }
                connection = false;
            }
            printf("\n");
        }
        int i = 0;
    }
}