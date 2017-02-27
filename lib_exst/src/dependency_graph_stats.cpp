#include <exst/dependency_graph_stats.h>
#include <fstream>

namespace exst {
    void DependencyGraphStatsCalculator::addRule(std::list<lit_type> body, std::list<lit_type> heads) {
        std::unordered_map<uint32_t, uint32_t> &vertexNodeMap = dGraphStatistics.atomVertexMap;
        std::unordered_map<uint32_t, std::unordered_map<uint32_t, EdgeType>> &graph = dGraphStatistics.dependencyGraph;

        // add body atoms to graph if they are not in it
        std::list<exst::lit_type>::iterator it;
        for (it = body.begin(); it != body.end(); it++) {
            uint32_t id = it->id;
            if (vertexNodeMap.count(id) == 0) {
                vertexNodeMap[id] = (unsigned int) graph.size();
                graph[vertexNodeMap[id]];
            }
        }

        // add head atoms to graph if they are not in it
        for (it = heads.begin(); it != heads.end(); it++) {
            uint32_t id = it->id;
            if (vertexNodeMap.count(id) == 0) {
                vertexNodeMap[id] = (unsigned int) graph.size();
                graph[vertexNodeMap[id]];
            }
        }

        //add dependency edges
        std::list<exst::lit_type>::iterator a;
        for (a = heads.begin(); a != heads.end(); a++) {
            std::list<exst::lit_type>::iterator b;
            for (b = body.begin(); b != body.end(); b++) {
                uint32_t id = b->id;
                if ((a->id) > 1 && dGraphStatistics.edgeMap[a->id].count(id) == 0) {
                    graph[vertexNodeMap[a->id]][vertexNodeMap[id]] = HEAD;
                };
            }
        }
    }

    std::list<std::string> DependencyGraphStatsCalculator::getAdditionalStatistics() {
        std::list<std::string> slist;
        std::string str = "";
        if (ProgramParameter::getInstance().dGraphFormat != NONE) {

            if (ProgramParameter::getInstance().dGraphPath.length() != 0) {
                std::ofstream fileStream;
                fileStream.open(ProgramParameter::getInstance().dGraphPath, std::ofstream::out);
                fileStream << getFormatedGraph(ProgramParameter::getInstance().dGraphFormat, getDependencyGraph());
                fileStream.close();
            } else {
                str += "\"Dependency Graph\" : \n[";
                str += getFormatedGraph(ProgramParameter::getInstance().dGraphFormat, getDependencyGraph());
                str += "]";
                slist.push_back(str);
            }
        }
        return slist;
    }

    std::list<std::pair<std::string, std::string>> DependencyGraphStatsCalculator::getStatistics() {
        std::list<std::pair<std::string, std::string>> ret;

        ret.push_back(std::pair<std::string, std::string>("Dependency Graph Nodes", std::to_string(
                this->dGraphStatistics.dependencyGraph.size())));
        ret.push_back(std::pair<std::string, std::string>("Dependency Graph Edges", std::to_string(
                edgeCount(this->dGraphStatistics.dependencyGraph))));
        return ret;
    }

}