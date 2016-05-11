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

    void DependencyGraphStats::addRuleDependencyGraph(std::vector<uint32> bodies, Clasp::PodVector<uint32>::type heads)
    {
        std::unordered_map<uint32, uint32> &vertexNodeMap = atomVertexMap;
        std::unordered_map<uint32, std::unordered_map<uint32, EdgeType>> &graph = dependencyGraph;

        // add body atoms to graph if they are not in it
        for (uint32 i = 0; i < bodies.size(); ++i)
        {
            if (vertexNodeMap.count(bodies[i]) == 0)
            {
                vertexNodeMap[bodies[i]] = nodecount;
                nodecount++;
                graph[vertexNodeMap[bodies[i]]];
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
                if ((heads[a]) > 1 && edgeMap[heads[a]].count(bodies[b]) == 0)
                {
                    graph[vertexNodeMap[heads[a]]][vertexNodeMap[bodies[b]]] = head;
                    edgecount++;
                };
            }
        }

        if (heads.size() == 0 || heads.front() == 1)
        {
            for (uint32 a = 0; a < bodies.size(); ++a)
            {
                for (uint32 b = 0; b < bodies.size(); ++b)
                {
                    if ((bodies[a]) != (bodies[b]) && edgeMap[bodies[a]].count(bodies[b]) == 0)
                    {
                        graph[vertexNodeMap[bodies[a]]][vertexNodeMap[bodies[b]]] = body;
                    };
                }
            }
        }
    }
}