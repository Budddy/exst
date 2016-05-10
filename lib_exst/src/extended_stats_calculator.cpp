#include <exst/ExstTypes.h>
#include <exst/graph_stats_calculator.h>
#include <iostream>

namespace exst
{

    std::__cxx11::string GraphStatsCalculator::getDIMACS(MyGraph &graph, uint32 edgecount)
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

    void GraphStatsCalculator::parseRule(std::vector<uint32> dependencies, Clasp::PodVector<uint32>::type heads,
                                         uint32 negative)
    {

        //rules
        numRules++;
        // num clauses
        if (dependencies.size() > 0)
            numClauses++;

        // num facts
        if (dependencies.size() == 0)
            numFacts++;

        //non horn clause
        if (negative != 0)
            ++GraphStatsCalculator::numNonHornClauses;
        //non dual horn clause
        if (negative < dependencies.size() - 1)
            ++GraphStatsCalculator::numNonDualHornClauses;
        //clause size
        clauseSize = (unsigned long) (clauseSize < dependencies.size() ? dependencies.size() : clauseSize);
        //negative clause size
        clauseSizePositive = (unsigned long) (clauseSizePositive < dependencies.size() - negative ? dependencies.size() - negative
                                                                                                  : clauseSizePositive);
        //positive clause size
        clauseSizeNegative = clauseSizeNegative < negative ? negative : clauseSizeNegative;

        //atom occurences
        countAtomOccurencesTotal(dependencies, heads);

        //atom occurences
        countAtomOccurences(dependencies, heads);

        //atom occurences Positive
        parseAtomOccurencesPositive(dependencies, heads, negative);

        //atom occurences Negative
        parseAtomOccurencesNegative(negative, dependencies);

        dependencyGraphStats.addRuleDependencyGraph(dependencies, heads);
        addRuleIncidenceGraph(dependencies, heads, negative);
    }

    void GraphStatsCalculator::countAtomOccurences(std::vector<uint32> &dependencies,
                                                   Clasp::PodVector<unsigned int>::type &heads)
    {
        std::vector<uint32>::iterator it1;
        for (it1 = dependencies.begin(); it1 != dependencies.end(); it1++)
        {
            atomOccurences[(*it1)]++;
        }
        uint32 *it2;
        for (it2 = heads.begin(); it2 != heads.end(); it2++)
        {
            atomOccurences[(*it2)]++;
        }
    }

    void GraphStatsCalculator::parseAtomOccurencesNegative(uint32 &negative, std::vector<uint32> &dependencies)
    {
        std::vector<uint32>::iterator it5 = dependencies.begin();
        uint32 i;
        for (i = 0; i < negative; i++)
        {
            atomOccurencesNegative[(*(it5 + i))]++;
        }
    }

    void GraphStatsCalculator::parseAtomOccurencesPositive(std::vector<uint32> &dependencies,
                                                           Clasp::PodVector<unsigned int>::type &heads,
                                                           uint32 &negative)
    {
        std::vector<uint32>::iterator it3 = dependencies.begin();
        uint32 a;
        for (a = negative; a < dependencies.size(); a++)
        {
            atomOccurencesPositive[(*(it3 + a))]++;
        }
        uint32 *it4;
        for (it4 = heads.begin(); it4 != heads.end(); it4++)
        {
            atomOccurencesPositive[(*it4)]++;
        }
    }

    void GraphStatsCalculator::addId(uint32 before, uint32 after)
    {
        atomIds[after] = before;
    }

    void GraphStatsCalculator::printExtendedStats()
    {
        dependencyGraphStats.printDepGraph();
        printIncidenceGraph();
        std::cout << "Non Horn Clauses: " << this->numNonHornClauses << "\n";
        std::cout << "Non Dual Horn Clauses: " << this->numNonDualHornClauses << "\n";
        std::cout << "Clauses: " << this->numClauses << "\n";
        std::cout << "Facts: " << this->numFacts << "\n";
        std::cout << "Rules: " << this->numRules << "\n";
    }

    void GraphStatsCalculator::countAtomOccurencesTotal(std::vector<uint32> &dependencies,
                                                        Clasp::PodVector<unsigned int>::type &heads)
    {
        std::vector<uint32>::iterator it1;
        for (it1 = dependencies.begin(); it1 != dependencies.end(); it1++)
        {
            atomOccurencesTotal[(*it1)]++;
        }
        uint32 *it2;
        for (it2 = heads.begin(); it2 != heads.end(); it2++)
        {
            atomOccurencesTotal[(*it2)]++;
        }
    }
}