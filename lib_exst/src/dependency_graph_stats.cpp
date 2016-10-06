#include <exst/dependency_graph_stats.h>

namespace exst
{
    void DependencyGraphStatsCalculator::addRuleDependencyGraph(std::list<lit_type> body, std::list<lit_type> heads)
    {
        std::unordered_map<uint32_t, uint32_t> &vertexNodeMap = dGraphStatistics.atomVertexMap;
        std::unordered_map<uint32_t, std::unordered_map<uint32_t, EdgeType>> &graph = dGraphStatistics.dependencyGraph;

        // add body atoms to graph if they are not in it
        std::list<exst::lit_type>::iterator it;
        for (it = body.begin(); it != body.end(); it++)
        {
            uint32_t id = it->id;
            if (vertexNodeMap.count(id) == 0)
            {
                vertexNodeMap[id] = graph.size();
                graph[vertexNodeMap[id]];
            }
        }

        // add head atoms to graph if they are not in it
        for (it = heads.begin(); it != heads.end(); it++)
        {
            uint32_t id = it->id;
            if (vertexNodeMap.count(id) == 0)
            {
                vertexNodeMap[id] = graph.size();
                graph[vertexNodeMap[id]];
            }
        }

        //add dependency edges
        std::list<exst::lit_type>::iterator a;
        for (a = heads.begin(); a != heads.end(); a++)
        {
            std::list<exst::lit_type>::iterator b;
            for (b = body.begin(); b != body.end(); b++)
            {
                uint32_t id = b->id;
                if ((a->id) > 1 && dGraphStatistics.edgeMap[a->id].count(id) == 0)
                {
                    graph[vertexNodeMap[a->id]][vertexNodeMap[id]] = HEAD;
                };
            }
        }
    }
}