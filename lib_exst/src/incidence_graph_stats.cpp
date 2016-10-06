#include <exst/incidence_graph_stats.h>
#include <exst/extended_stats_calculator.h>
#include <fstream>

namespace exst
{
    void IncidenceGraphStatsCalculator::addRuleIncidenceGraph(std::list<lit_type> body, std::list<lit_type> heads)
    {
        if (body.size() == 0)
        {
            return;
        }
        std::unordered_map<uint32_t, std::unordered_map<uint32_t, EdgeType>> &myGraph = iGraphStats.incidenceGraph;
        std::unordered_map<uint32_t, uint32_t> &avmap = iGraphStats.atomVertexMap;
        std::unordered_map<uint32_t, std::unordered_map<uint32_t, EdgeType>> &rbmap = iGraphStats.ruleBodyMap;
        htd::IMutableMultiGraph *iGraph = iGraphStats.iGraph;

        uint32_t rule_Vertex = iGraph->addVertex();
        myGraph[rule_Vertex];

        // add body atoms
        std::list<lit_type>::iterator it;
        for (it = body.begin(); it != body.end(); it++)
        {
            uint32_t dId = it->id;
            //if atom is new, add vertex to graph
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
        }

        // add head atoms
        std::list<lit_type>::iterator it_;
        for (it_ = heads.begin(); it_ != heads.end(); it_++)
        {
            uint32_t hId = it_->id;
            //if atom is new, add vertex to graph
            if (avmap.count(hId) == 0)
            {
                avmap[hId] = iGraph->addVertex();
                myGraph[avmap[hId]];
            }
            myGraph[avmap[hId]][rule_Vertex] = HEAD;
            myGraph[rule_Vertex][avmap[hId]] = HEAD;
            iGraph->addEdge(avmap[hId], rule_Vertex);
        }
    }

    void IncidenceGraphStatsCalculator::resetAssignment()
    {
        //reset htd reduct graph
        delete iGraphStats.iGraphReduct;
        iGraphStats.iGraphReduct = iGraphStats.iGraph->clone();
        //selectedAtoms.clear();
        iGraphStats.ruleBodyMapReduct.insert(iGraphStats.ruleBodyMap.begin(), iGraphStats.ruleBodyMap.end());
        //reset reduct graph
        iGraphStats.incidenceGraphReduct = copyMyGraph(iGraphStats.incidenceGraph);
    }

    void IncidenceGraphStatsCalculator::reduceGraph(lit_type lit)
    {
        uint32_t nodeIdBody = iGraphStats.atomVertexMap[(*iGraphStats.atomIds)[lit.id]];
        //get all bodies the literal is in
        std::unordered_map<uint32_t, EdgeType> edges = iGraphStats.incidenceGraphReduct[nodeIdBody];

        std::unordered_map<unsigned int, EdgeType>::iterator it;
        //iterate over all bodies the literal is in
        for (it = edges.begin(); it != edges.end(); it++)
        {
            //if sign is different to the sign of the literal in the body, reduce graph
            if ((*it).second == (lit.s == POSITIVE ? NEG : POS))
            {
                std::unordered_map<uint32_t, EdgeType> bodies = iGraphStats.ruleBodyMapReduct[(*it).first];
                std::unordered_map<unsigned int, EdgeType>::iterator bodyIt;
                //remove all edges of wrong body
                for (bodyIt = bodies.begin(); bodyIt != bodies.end(); bodyIt++)
                {
                    //remove only body edges
                    if (bodyIt->second == POS || bodyIt->second == NEG)
                    {
                        iGraphStats.incidenceGraphReduct[bodyIt->first].erase(it->first);
                        iGraphStats.incidenceGraphReduct[it->first].erase(bodyIt->first);
                        iGraphStats.ruleBodyMapReduct[it->first].erase(bodyIt->first);
                        iGraphStats.iGraphReduct->removeEdge(
                                *iGraphStats.iGraphReduct->associatedEdgeIds(bodyIt->first, it->first).begin());
                    }
                }
            }
        }
    }

    void IncidenceGraphStatsCalculator::updateAssignment(const Clasp::LitVec new_assignment)
    {
        if (new_assignment.size() > iGraphStats.current_assignment.size())
        {
            iGraphStats.assignmentCount += new_assignment.size() - iGraphStats.current_assignment.size();
        } else
        {
            uint32_t pos = 0, i = 0;
            for (pos = 0; (pos + 1) < new_assignment.size() && (pos + 1) < iGraphStats.current_assignment.size(); pos++)
            {
                if (new_assignment.at(pos) != iGraphStats.current_assignment.at(pos))
                {
                    break;
                }
            }
            iGraphStats.assignmentCount += new_assignment.size() - (pos + 1);
        }
        if (iGraphStats.assignmentCount >= iGraphStats.widthCalcInterval)
        {
            iGraphStats.numReducts++;
            iGraphStats.assignmentCount -= iGraphStats.widthCalcInterval;
            resetAssignment();
            for (int i = 0; i < new_assignment.size(); i++)
            {
                reduceGraph(exst::lit_type(new_assignment.at(i).var(),
                                           new_assignment.at(i).sign() ? exst::POSITIVE : exst::NEGATIVE));
            }
            //calculate and save the Treewidth of the reduct graph
            if (this->iGraphStats.calculateTreeWidth)
            {
                iGraphStats.widths.push_back(getTreewidth(iGraphStats.iGraphReduct, iGraphStats.libraryInstance));
            }

            if (iGraphStats.rGraphFormat != NONE)
            {

                if (iGraphStats.rGraphPath.length() != 0)
                {
                    std::ofstream fileStream;
                    fileStream.open((iGraphStats.rGraphPath + "_") + std::to_string(iGraphStats.numReducts),
                                    std::ofstream::out);
                    fileStream << getFormatedGraph(iGraphStats.rGraphFormat, iGraphStats.incidenceGraphReduct);
                    fileStream.close();
                } else
                {
                    iGraphStats.rGraphs.push_back(
                            getFormatedGraph(iGraphStats.rGraphFormat, iGraphStats.incidenceGraphReduct));
                }
            }
        }
        iGraphStats.current_assignment = new_assignment;
    }

    size_t getTreewidth(htd::IMutableMultiGraph *graph, htd::LibraryInstance *libraryInstance)
    {
        //initialize algorithm for decomposition
        htd::IterativeImprovementTreeDecompositionAlgorithm algorithm(libraryInstance,
                                                                      libraryInstance->treeDecompositionAlgorithmFactory().getTreeDecompositionAlgorithm(
                                                                              libraryInstance),
                                                                      WidthMinimizingFitnessFunction());
        algorithm.setIterationCount(10);
        // compute decomposition of graph
        htd::ITreeDecomposition *decomposition = algorithm.computeDecomposition(*graph);
        // if there is no decomposition return 0
        size_t size = decomposition != nullptr ? decomposition->maximumBagSize() : 0;
        delete decomposition;
        return size;
    }
}