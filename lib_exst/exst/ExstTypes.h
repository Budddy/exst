#ifndef CLASP_EXSTTYPES_H
#define CLASP_EXSTTYPES_H

#include <unordered_map>

namespace exst
{
    /*
     * possible types of edges in the graph
     */
    enum EdgeType
    {
        POS = 1, NEG = -1, HEAD, BODY
    };

    enum Sign
    {
        POSITIVE = 1, NEGATIVE = -1
    };

    struct lit_type
    {
        uint32_t id;
        Sign s;
    };

    //datatype for internal exst graph representation
    typedef std::unordered_map<uint32_t, std::unordered_map<uint32_t, EdgeType>> MyGraph;

    /*
     * returns a copy of the given graph
     */
    MyGraph copyMyGraph(MyGraph graph);

    /*
     * prints the dependency graph as edge list
     */
    std::string getDIMACS(MyGraph &graph, uint32_t edgecount);
}

#endif //CLASP_EXSTTYPES_H