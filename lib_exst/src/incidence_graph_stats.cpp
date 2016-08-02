#include <exst/incidence_graph_stats.h>

namespace exst
{
    void exst::IncidenceGraphStats::addRuleIncidenceGraph(std::list<lit_type> bodies, std::list<lit_type> heads)
    {
        std::unordered_map<uint32_t, std::unordered_map<uint32_t, EdgeType>> &myGraph = incidenceGraph;
        std::unordered_map<uint32_t, uint32_t> &avmap = atomVertexMap;
        std::unordered_map<uint32_t, std::unordered_map<uint32_t, EdgeType>> &rbmap = ruleBodyMap;

        uint32_t rule_Vertex = iGraph->addVertex();
        myGraph[rule_Vertex];

        // add body atoms
        std::list<exst::lit_type>::iterator it;
        for (it = bodies.begin(); it != bodies.end(); it++)
        {
            uint32_t dId = it->id;
            if (avmap.count(dId) == 0)
            {
                avmap[dId] = iGraph->addVertex();
                myGraph[avmap[dId]];
            }
            bool neg = it->s == NEGATIVE;
            rbmap[rule_Vertex][avmap[dId]] = neg ? NEG : POS;
            myGraph[avmap[dId]][rule_Vertex] = neg ? NEG : POS;
            myGraph[rule_Vertex][avmap[dId]] = neg ? NEG : POS;
            iGraph->addEdge(avmap[dId], rule_Vertex);
            edgecount++;
        }

        // add head atoms
        std::list<exst::lit_type>::iterator it_;
        for (it_ = heads.begin(); it_ != heads.end(); it_++)
        {
            uint32_t hId = it_->id;
            if (avmap.count(hId) == 0)
            {
                avmap[hId] = iGraph->addVertex();
                myGraph[avmap[hId]];
            }
            myGraph[avmap[hId]][rule_Vertex] = HEAD;
            myGraph[rule_Vertex][avmap[hId]] = HEAD;
            iGraph->addEdge(avmap[hId], rule_Vertex);
            edgecount++;
        }
    }

    void exst::IncidenceGraphStats::addAtomReduct(lit_type lit)
    {
        bool neg = true;
        uint32_t atomId = atomIds[lit.id];
        selectedAtoms[atomId] = neg;
    }

    void exst::IncidenceGraphStats::resetAssignment()
    {
        iGraphReduct = iGraph->clone();
        selectedAtoms.clear();
        ruleBodyMapReduct.insert(ruleBodyMap.begin(), ruleBodyMap.end());
        incidenceGraphReduct = copyMyGraph(incidenceGraph);
    }

    void exst::IncidenceGraphStats::reduceGraph(lit_type lit)
    {
        uint32_t nodeIdBody = atomVertexMap[atomIds[lit.id]];
        MyGraph &igraph = incidenceGraphReduct;
        std::unordered_map<uint32_t, EdgeType> edges = igraph[nodeIdBody];

        std::unordered_map<unsigned int, exst::EdgeType>::iterator it;
        for (it = edges.begin(); it != edges.end(); it++)
        {
            if ((*it).second == (lit.s == POSITIVE ? NEG : POS))
            {
                std::unordered_map<uint32_t, EdgeType> bodies = ruleBodyMapReduct[(*it).first];
                std::unordered_map<unsigned int, exst::EdgeType>::iterator bodyIt;
                for (bodyIt = bodies.begin(); bodyIt != bodies.end(); bodyIt++)
                {
                    if (bodyIt->second == POS || bodyIt->second == NEG)
                    {
                        igraph[bodyIt->first].erase(it->first);
                        igraph[it->first].erase(bodyIt->first);
                        ruleBodyMapReduct[it->first].erase(bodyIt->first);
                        iGraphReduct->removeEdge(*iGraphReduct->associatedEdgeIds(bodyIt->first, it->first).begin());
                    }
                }
            }
        }
        std::cout << "get Treewidth: ";
        reds.push_back(getTreewidth(iGraphReduct));
        std::cout << reds.back() << "\n";
    }

    void exst::IncidenceGraphStats::printIGraphReduct()
    {
        std::cout << "{\"_Incidence Graph Reduct_\": [ \n  [\"Nodes\", ";
        std::cout << incidenceGraphReduct.size();
        std::cout << "],\n  [\"Edges\", ";
        std::cout << edgeCount(incidenceGraphReduct);
        //std::cout << "],\n  [\"Treewidth\", ";
        //std::cout << getTreewidth(incidenceGraphReduct);
        std::cout << "]\n}\n";
    }

    /**
 *  Implementation of the htd::ITreeDecompositionFitnessFunction interface which prefers decompositions of minimal width.
 */
    class WidthMinimizingFitnessFunction : public htd::ITreeDecompositionFitnessFunction
    {
    public:
        /**
         *  Constructor of class WidthMinimizingFitnessFunction.
         */
        WidthMinimizingFitnessFunction(void)
        {

        }

        /**
         *  Destructor of class WidthMinimizingFitnessFunction.
         */
        virtual ~WidthMinimizingFitnessFunction()
        {

        }

        htd::FitnessEvaluation *
        fitness(const htd::IMultiHypergraph &graph, const htd::ITreeDecomposition &decomposition) const HTD_OVERRIDE
        {
            HTD_UNUSED(graph)

            return new htd::FitnessEvaluation(1, -(double) (decomposition.maximumBagSize()));
        }

        WidthMinimizingFitnessFunction *clone(void) const HTD_OVERRIDE
        {
            return new WidthMinimizingFitnessFunction();
        }
    };

    size_t exst::IncidenceGraphStats::getTreewidth(htd::IMutableMultiGraph *graph)
    {
        htd::IterativeImprovementTreeDecompositionAlgorithm algorithm(libraryInstance,
                                                                      libraryInstance->treeDecompositionAlgorithmFactory().getTreeDecompositionAlgorithm(
                                                                              libraryInstance),
                                                                      WidthMinimizingFitnessFunction());

        htd::ITreeDecomposition *decomposition = algorithm.computeDecomposition(*graph);
        return decomposition != nullptr ? decomposition->maximumBagSize() : 0;
    }
}