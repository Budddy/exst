#include <exst/ExstTypes.h>

namespace exst
{
    MyGraph copyMyGraph(MyGraph graph)
    {
        MyGraph newGraph;
        std::unordered_map<unsigned int, std::unordered_map<unsigned int, exst::EdgeType>>::iterator i;
        for (i = graph.begin(); i != graph.end(); i++)
        {
            newGraph[i->first].insert(i->second.begin(), i->second.end());
        }
        return newGraph;
    }
}