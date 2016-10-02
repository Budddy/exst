#include <exst/ExstTypes.h>

namespace exst
{
    uint32_t edgeCount(MyGraph &graph)
    {
        uint32_t edges = 0;
        std::unordered_map<unsigned int, std::unordered_map<unsigned int, exst::EdgeType>>::iterator a;
        for (a = graph.begin(); a != graph.end(); a++)
        {
            std::unordered_map<unsigned int, exst::EdgeType>::iterator b;
            for (b = a->second.begin(); b != a->second.end(); b++)
            {
                edges++;
            }
        }
        return edges / 2;
    }

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

    std::string getDIMACSFormat(MyGraph &graph)
    {
        std::string dimacsString;
        dimacsString += "\"p edge " + std::to_string(graph.size()) + " " + std::to_string(edgeCount(graph)) + "\"";

        std::unordered_map<unsigned int, std::unordered_map<unsigned int, exst::EdgeType>>::iterator it;
        for (it = graph.begin(); it != graph.end(); it++)
        {
            std::unordered_map<unsigned int, exst::EdgeType>::iterator it2;
            for (it2 = it->second.begin(); it2 != it->second.end(); it2++)
            {
                if (it->first < it2->first)
                {
                    dimacsString += "\n,\"e " + std::to_string(it->first) + " " + std::to_string(it2->first) + "\"";
                }
            }
        }
        return dimacsString;
    }

    std::string getGrFormat(MyGraph &graph)
    {
        std::string grString;
        grString += "\"p tw " + std::to_string(graph.size()) + " " + std::to_string(edgeCount(graph)) + "\"";

        std::unordered_map<unsigned int, std::unordered_map<unsigned int, exst::EdgeType>>::iterator it;
        for (it = graph.begin(); it != graph.end(); it++)
        {
            std::unordered_map<unsigned int, exst::EdgeType>::iterator it2;
            for (it2 = it->second.begin(); it2 != it->second.end(); it2++)
            {
                if (it->first < it2->first)
                {
                    grString += "\n,\"" + std::to_string(it->first + 1) + " " + std::to_string(it2->first + 1) + "\"";
                }
            }
        }
        return grString;
    }

    std::string getGMLFormat(MyGraph &graph)
    {
        std::string gmlString;
        gmlString += "graph [\n";

        std::unordered_map<unsigned int, std::unordered_map<unsigned int, exst::EdgeType>>::iterator it;
        for (it = graph.begin(); it != graph.end(); it++)
        {
            gmlString += "  node [\n    id " + std::to_string(it->first + 1) + "\n    label \"" +
                         std::to_string(it->first + 1) + "\"\n  ]";
        }

        for (it = graph.begin(); it != graph.end(); it++)
        {
            std::unordered_map<unsigned int, exst::EdgeType>::iterator it2;
            for (it2 = it->second.begin(); it2 != it->second.end(); it2++)
            {
                if (it->first < it2->first)
                {
                    gmlString += "  edge [\n    source " + std::to_string(it->first + 1);
                    gmlString += "\n    target " + std::to_string(it2->first + 1) + "\n  ]";
                }
            }
        }
        gmlString += "]";
        return gmlString;
    }

    uint32_t maxValue(std::unordered_map<uint32_t, uint32_t> values)
    {
        uint32_t maxValue = 0;
        std::unordered_map<unsigned int, unsigned int>::iterator it;
        for (it = values.begin(); it != values.end(); it++)
        {
            maxValue = it->second > maxValue ? it->second : maxValue;
        }
        return maxValue;
    }

    std::string getFormatedGraph(GraphFormat f, MyGraph &graph)
    {
        switch (f)
        {
            case DIMACS :
                return getDIMACSFormat(graph);
            case GR :
                return getGrFormat(graph);
            case GML :
                return getGMLFormat(graph);
            default:
                return "";
        }
    }
}