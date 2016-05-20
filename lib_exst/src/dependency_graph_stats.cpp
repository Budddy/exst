#include <exst/dependency_graph_stats.h>
#include <iostream>

namespace exst
{
    void DependencyGraphStats::printDepGraph()
    {
        std::cout << "\n_Dependency Graph_ \nNodes: ";
        std::cout << nodecount;
        std::cout << "\nEdges: ";
        std::cout << edgecount;
        std::cout << "\n";
    }

    void DependencyGraphStats::addRuleDependencyGraph(Clasp::WeightLitVec bodies, Clasp::PodVector<uint32>::type heads)
    {
        std::unordered_map<uint32, uint32> &vertexNodeMap = atomVertexMap;
        std::unordered_map<uint32, std::unordered_map<uint32, EdgeType>> &graph = dependencyGraph;

        // add body atoms to graph if they are not in it
        for (uint32 i = 0; i < bodies.size(); ++i)
        {
            uint32 id = bodies[i].first.index();
            if (vertexNodeMap.count(id) == 0)
            {
                vertexNodeMap[id] = nodecount;
                nodecount++;
                graph[vertexNodeMap[id]];
            }
        }

        // add head atoms to graph if they are not in it
        for (uint32 i = 0; i < heads.size(); ++i)
        {
            if (vertexNodeMap.count(heads[i]) == 0)
            {
                vertexNodeMap[heads[i]] = nodecount;
                nodecount++;
                vertexNodeMap[heads[i]];
            }
        }

        for (uint32 a = 0; a < heads.size(); ++a)
        {
            for (uint32 b = 0; b < bodies.size(); ++b)
            {
                uint32 id = bodies[b].first.index();
                if ((heads[a]) > 1 && edgeMap[heads[a]].count(id) == 0)
                {
                    graph[vertexNodeMap[heads[a]]][vertexNodeMap[id]] = head;
                    edgecount++;
                };
            }
        }

        if (heads.size() == 0 || heads.front() == 1)
        {
            for (uint32 a = 0; a < bodies.size(); ++a)
            {
                uint32 ida = bodies[a].first.index();
                for (uint32 b = 0; b < bodies.size(); ++b)
                {
                    uint32 idb = bodies[b].first.index();
                    if ((ida) != (idb) && edgeMap[ida].count(idb) == 0)
                    {
                        graph[vertexNodeMap[ida]][vertexNodeMap[idb]] = body;
                    };
                }
            }
        }
    }
}