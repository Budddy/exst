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
        POS = 1, NEG = -1, HEAD = 2, BODY = 3
    };

    enum Sign
    {
        POSITIVE = 1, NEGATIVE = -1
    };

    struct lit_type
    {
        lit_type(uint32_t id, Sign s) : id(id), s(s)
        {
        }

        lit_type()
        {
        }

        uint32_t id;
        Sign s;
    };

    //datatype for internal exst graph representation
    typedef std::unordered_map<uint32_t, std::unordered_map<uint32_t, EdgeType>> MyGraph;

    uint32_t edgeCount(MyGraph &graph);

    /*
     * returns a copy of the given graph
     */
    MyGraph copyMyGraph(MyGraph graph);

    /*
     * returns the string representation of the graph in DIMACS format
     */
    std::string getDIMACS(MyGraph &graph);

    /*
     * returns the string representation of the graph in Gr format
     */
    std::string getGrFormat(MyGraph &graph);

    uint32_t maxValue(std::unordered_map<uint32_t, uint32_t> values);
}

#endif //CLASP_EXSTTYPES_H