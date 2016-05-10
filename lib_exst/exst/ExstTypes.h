#ifndef CLASP_EXSTTYPES_H
#define CLASP_EXSTTYPES_H

#include <unordered_map>
#include <clasp/util/platform.h>

namespace exst
{

    enum EdgeType
    {
        positive, negative, head, body
    };

    typedef std::unordered_map<uint32, std::unordered_map<uint32, EdgeType>> MyGraph;

    MyGraph copyMyGraph(MyGraph graph);
}

#endif //CLASP_EXSTTYPES_H