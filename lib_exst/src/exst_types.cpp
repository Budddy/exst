#include <exst/exst_types.h>

namespace exst {
    std::set<std::pair<uint32_t, uint32_t>> genUniqueEdgeSet(MyGraph &graph) {
        std::set<std::pair<uint32_t, uint32_t>> test;
        std::unordered_map<unsigned int, std::unordered_map<unsigned int, exst::EdgeType>>::iterator a;
        for (a = graph.begin(); a != graph.end(); a++) {
            std::unordered_map<unsigned int, exst::EdgeType>::iterator b;
            for (b = a->second.begin(); b != a->second.end(); b++) {
                uint32_t first = (a->first > b->first) ? b->first : a->first;
                uint32_t second = (a->first > b->first) ? a->first : b->first;
                test.insert(std::pair<uint32_t, uint32_t>(first, second));
            }
        }
        return test;
    };

    uint32_t edgeCount(MyGraph &graph) {
        return (uint32_t) genUniqueEdgeSet(graph).size();
    }

    MyGraph copyMyGraph(MyGraph graph) {
        MyGraph newGraph;
        std::unordered_map<unsigned int, std::unordered_map<unsigned int, exst::EdgeType>>::iterator i;
        for (i = graph.begin(); i != graph.end(); i++) {
            newGraph[i->first].insert(i->second.begin(), i->second.end());
        }
        return newGraph;
    }

    std::string getDIMACSFormat(MyGraph &graph) {
        std::string dimacsString;
        std::set<std::pair<uint32_t, uint32_t>> edges = genUniqueEdgeSet(graph);
        int correction = 0;
        if (graph.find(0) != graph.end()) {
            correction = 1;
        }
        dimacsString += "\"p edge " + std::to_string(graph.size()) + " " + std::to_string(edgeCount(graph)) + "\"";
        std::set<std::pair<uint32_t, uint32_t>>::iterator it;
        for (it = edges.begin(); it != edges.end(); it++) {
            dimacsString +=
                    "\n,\"e " + std::to_string(it->first + correction) + " " + std::to_string(it->second + correction) +
                    "\"";
        }
        return dimacsString;
    }

    std::string getGrFormat(MyGraph &graph) {
        std::string grString;
        std::set<std::pair<uint32_t, uint32_t>> edges = genUniqueEdgeSet(graph);
        int correction = 0;
        if (graph.find(0) != graph.end()) {
            correction = 1;
        }
        grString += "\"p tw " + std::to_string(graph.size()) + " " + std::to_string(edgeCount(graph)) + "\"";

        std::set<std::pair<uint32_t, uint32_t>>::iterator it;
        for (it = edges.begin(); it != edges.end(); it++) {
            grString +=
                    "\n,\"" + std::to_string(it->first + correction) + " " + std::to_string(it->second + correction) +
                    "\"";
        }
        return grString;
    }

    std::string getGMLFormat(MyGraph &graph) {
        std::string gmlString;
        std::set<std::pair<uint32_t, uint32_t>> edges = genUniqueEdgeSet(graph);
        int correction = 0;
        if (graph.find(0) != graph.end()) {
            correction = 1;
        }
        gmlString += "graph [\n";

        for (int i = 1; i <= graph.size(); i++) {
            gmlString += "  node [\n    id " + std::to_string(i) + "\n    label \"" + std::to_string(i) + "\"\n  ]";
        }

        std::set<std::pair<uint32_t, uint32_t>>::iterator it_;
        for (it_ = edges.begin(); it_ != edges.end(); it_++) {
            gmlString += "  edge [\n    source " + std::to_string(it_->first + correction);
            gmlString += "\n    target " + std::to_string(it_->second + correction) + "\n  ]";
        }
        gmlString += "]";
        return gmlString;
    }

    uint32_t maxValue(std::unordered_map<uint32_t, uint32_t> values) {
        uint32_t maxValue = 0;
        std::unordered_map<unsigned int, unsigned int>::iterator it;
        for (it = values.begin(); it != values.end(); it++) {
            maxValue = it->second > maxValue ? it->second : maxValue;
        }
        return maxValue;
    }

    std::string getFormatedGraph(GraphFormat f, MyGraph &graph) {
        switch (f) {
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