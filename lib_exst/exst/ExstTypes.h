#ifndef CLASP_EXSTTYPES_H
#define CLASP_EXSTTYPES_H

#include <unordered_map>
#include <clasp/util/platform.h>

namespace exst
{
    /*
     * possible types of edges in the graph
     */
    enum EdgeType
    {
        positive, negative, head, body
    };

    //datatype for internal exst graph representation
    typedef std::unordered_map<uint32, std::unordered_map<uint32, EdgeType>> MyGraph;

    /*
     * returns a copy of the given graph
     */
    MyGraph copyMyGraph(MyGraph graph);

    /*
     * prints the dependency graph as edge list
     */
    static std::string getDIMACS(MyGraph &graph, uint32 edgecount);
}

#endif //CLASP_EXSTTYPES_H