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

    static std::string getDIMACS(MyGraph &graph, uint32 edgecount)
    {
        std::__cxx11::string dimacs;
        dimacs += "p edge " + std::__cxx11::to_string(graph.size()) + " " + std::__cxx11::to_string(edgecount);

        std::unordered_map<unsigned int, std::unordered_map<unsigned int, exst::EdgeType>>::iterator it;
        for (it = graph.begin(); it != graph.end(); it++)
        {
            std::unordered_map<unsigned int, exst::EdgeType>::iterator it2;
            for (it2 = it->second.begin(); it2 != it->second.end(); it2++)
            {
                if (it->first < it2->first)
                {
                    dimacs += "\ne " + std::to_string(it->first);
                    dimacs += " " + std::to_string(it2->first);
                }
            }
        }
        return dimacs;
    }
}