#include <exst/extended_stats_calculator.h>
#include <fstream>

namespace exst
{
    bool StatsCalculator::isConstraint(std::list<lit_type> head, std::list<lit_type> body)
    {
        std::list<exst::lit_type>::iterator it;
        for (it = head.begin(); it != head.end(); it++)
        {
            if (generalStatistics.atomIdsOldNew.count(it->id))
            {
                return true;
            }
        }
        for (it = body.begin(); it != body.end(); it++)
        {
            if (generalStatistics.atomIdsOldNew.count(it->id))
            {
                return true;
            }
        }
        return false;
    }

    void StatsCalculator::parseRule(std::list<lit_type> body, std::list<lit_type> head)
    {
        dependencyGraphStats.addRuleDependencyGraph(body, head);
        incidenceGraphStats.addRuleIncidenceGraph(body, head);
        std::pair<std::list<lit_type>, std::list<lit_type>> rule;
        rule.first = head;
        rule.second = body;
        generalStatistics.rules.push_back(rule);
    }

    void StatsCalculator::calculateStats()
    {
        std::list<std::pair<std::list<exst::lit_type>, std::list<exst::lit_type>>>::iterator it;
        for (it = generalStatistics.rules.begin(); it != generalStatistics.rules.end(); it++)
        {
            std::list<lit_type> &head = it->first;
            std::list<lit_type> &body = it->second;
            //rules
            generalStatistics.numRules++;

            // num facts
            if (body.size() == 0)
                generalStatistics.numFacts++;

            uint32_t negative = 0, positive = 0;
            std::list<exst::lit_type>::iterator i;
            for (i = body.begin(); i != body.end(); i++)
            {
                if ((*i).s == -1)
                {
                    negative++;
                } else
                {
                    positive++;
                }
            };
            for (i = head.begin(); i != head.end(); i++)
            {
                if ((*i).s == -1)
                {
                    negative++;
                } else
                {
                    positive++;
                }
            };

            if (isConstraint(head, body))
            {
                generalStatistics.maxPositiveRuleSizeConstraint = (uint32_t) (
                        generalStatistics.maxPositiveRuleSizeConstraint < positive ? positive
                                                                                   : generalStatistics.maxPositiveRuleSizeConstraint);
                generalStatistics.numConstraints++;
                generalStatistics.atomOccurencesConstraint += head.size();
                generalStatistics.atomOccurencesConstraint += body.size();
            } else
            {
                generalStatistics.maxPositiveRuleSizeNonConstraint = (uint32_t) (
                        generalStatistics.maxPositiveRuleSizeNonConstraint < positive ? positive
                                                                                      : generalStatistics.maxPositiveRuleSizeNonConstraint);
                generalStatistics.atomOccurencesNonConstraint += head.size();
                generalStatistics.atomOccurencesNonConstraint += body.size();
            }
            //non horn clause
            if (!((head.size() == 0 && negative <= 1) || (head.size() == 1 && negative <= 0)) && (body.size() > 0))
                ++generalStatistics.numNonHornClauses;

            //non dual horn clause
            if ((body.size() - negative > 1) && (body.size() > 0))
                ++generalStatistics.numNonDualHornClauses;

            //clause size
            generalStatistics.maxClauseSize = (generalStatistics.maxClauseSize < body.size() + head.size() ?
                                               body.size() + head.size() : generalStatistics.maxClauseSize);
            //positive clause size
            generalStatistics.maxClauseSizePositive = (generalStatistics.maxClauseSizePositive <
                                                       body.size() - negative + head.size() ? body.size() -
                                                                                              negative +
                                                                                              head.size()
                                                                                            : generalStatistics.maxClauseSizePositive);
            //negative clause size
            generalStatistics.maxClauseSizeNegative = generalStatistics.maxClauseSizeNegative < negative ? negative
                                                                                                         : generalStatistics.maxClauseSizeNegative;

            //atom occurrences
            countAtomOccurences(body, head);

            //number of variables that occur in positive/negative literals
            parseVariableLiteral(body, head);
        }
        calculateVariables();
    }

    void StatsCalculator::countAtomOccurences(std::list<lit_type> body, std::list<lit_type> head)
    {
        std::list<exst::lit_type>::iterator it;
        for (it = body.begin(); it != body.end(); it++)
        {
            uint32_t id = it->id;
            if (it->s == -1)
            {
                generalStatistics.atomOccurencesNegative[id]++;
            } else
            {
                generalStatistics.atomOccurencesPositive[id]++;
            }
            generalStatistics.atomOccurences[id]++;
        }
        for (it = head.begin(); it != head.end(); it++)
        {
            uint32_t id = it->id;
            if (it->s == -1)
            {
                generalStatistics.atomOccurencesNegative[id]++;
            } else
            {
                generalStatistics.atomOccurencesPositive[id]++;
            }
            generalStatistics.atomOccurences[id]++;
        }
    }

    void StatsCalculator::addId(uint32_t before, uint32_t after)
    {
        generalStatistics.atomIdsNewOld[after] = before;
        generalStatistics.atomIdsOldNew[before] = after;
    }

    void StatsCalculator::printExtendedStats()
    {
        calculateStats();
        std::cout << "\n,\n\n \"Extended Stats\" : [\n";

        //dependency graph
        std::cout << "  [\"Dependency Graph Nodes\" , "
                  << dependencyGraphStats.getDependencyGraph().size() << "],\n";
        std::cout << "  [\"Dependency Graph Edges\" , "
                  << edgeCount(dependencyGraphStats.getDependencyGraph()) << "],\n";

        //incidence graph
        std::cout << "  [\"Incidence Graph Nodes\" , "
                  << incidenceGraphStats.getIncidenceGraph().size() << "],\n";
        std::cout << "  [\"Incidence Graph Edges\" , "
                  << edgeCount(incidenceGraphStats.getIncidenceGraph()) << "],\n";

        //number of non horn clauses
        std::cout << "  [\"Non Horn Clauses\" , " << generalStatistics.numNonHornClauses << "],\n";

        //number of non dual horn clauses
        std::cout << "  [\"Non Dual Horn Clauses\" , " << generalStatistics.numNonDualHornClauses << "],\n";

        //maximum clause size
        std::cout << "  [\"max clause size\" , " << generalStatistics.maxClauseSize << "],\n";

        //maximum positive/negative clause size, i.e., only positive/negative literals are counted
        std::cout << "  [\"max positive clause size\" , " << generalStatistics.maxClauseSizePositive << "],\n";
        std::cout << "  [\"max negative clause size\" , " << generalStatistics.maxClauseSizeNegative << "],\n";

        //number of variables that occur as positive/negative literals
        std::cout << "  [\"number of variables that occur as positive literals with helpers\" , "
                  << generalStatistics.variablePositive.size() << "],\n";
        std::cout << "  [\"number of variables that occur as positive literals without helpers\" , "
                  << generalStatistics.variablePositiveWithoutHelper.size() << "],\n";
        std::cout << "  [\"number of variables that occur as negative literals with helpers\" , "
                  << generalStatistics.variableNegative.size() << "],\n";
        std::cout << "  [\"number of variables that occur as negative literals without helpers\" , "
                  << generalStatistics.variableNegativeWithoutHelper.size() << "],\n";

        //maximum positive rule size (constraint/non-constraint)
        std::cout << "  [\"maximum positive rule size constraint\" , "
                  << generalStatistics.maxPositiveRuleSizeConstraint << "],\n";
        std::cout << "  [\"maximum positive rule size non-constraint\" , "
                  << generalStatistics.maxPositiveRuleSizeNonConstraint
                  << "],\n";

        //total number of atom occurrences in the program (constraints/non-constraint)
        std::cout << "  [\"total number of atom occurrences constraint\" , "
                  << generalStatistics.atomOccurencesConstraint << "],\n";
        std::cout << "  [\"total number of atom occurrences non-constraint\" , "
                  << generalStatistics.atomOccurencesNonConstraint
                  << "],\n";

        //maximum number of occurrences of an atom
        std::cout << "  [\"maximum number of occurrences of an atom\" , " << maxValue(generalStatistics.atomOccurences)
                  << "],\n";

        //maximum number of positive occurrences of an atom
        std::cout << "  [\"maximum number of positive occurrences of an atom\" , "
                  << maxValue(generalStatistics.atomOccurencesPositive)
                  << "],\n";

        //maximum number of negative occurrences of an atom
        std::cout << "  [\"maximum number of negative occurrences of an atom\" , "
                  << maxValue(generalStatistics.atomOccurencesNegative)
                  << "]\n";

        std::list<float>::iterator it;
        for (it = incidenceGraphStats.getWidths()->begin();
             it != incidenceGraphStats.getWidths()->end(); it++)
        {
            std::cout << "  ,[\"reduct width\" , " << (*it) << "]\n";
        }

        std::cout << " ]\n";

        if (incidenceGraphStats.iGraphStats.rGraphFormat != NONE)
        {
            if (incidenceGraphStats.iGraphStats.rGraphPath.length() == 0)
            {
                std::cout << "\n,\n\n \"Reduct Graph\" : [\n  ";
                std::list<std::string>::iterator it_;
                for (it_ = incidenceGraphStats.getRGraphs()->begin();
                     it_ != incidenceGraphStats.getRGraphs()->end(); it_++)
                {
                    if (it_ != incidenceGraphStats.getRGraphs()->begin())
                    {
                        std::cout << "  ,";
                    }
                    std::cout << "[" << (*it_) << "]\n";
                }
                std::cout << "]\n";
            }
        }

        if (incidenceGraphStats.iGraphStats.iGraphFormat != NONE)
        {

            if (incidenceGraphStats.iGraphStats.iGraphPath.length() != 0)
            {
                std::ofstream fileStream;
                fileStream.open(incidenceGraphStats.iGraphStats.iGraphPath, std::ofstream::out);
                fileStream << getFormatedGraph(incidenceGraphStats.iGraphStats.iGraphFormat,
                                               incidenceGraphStats.getIncidenceGraph());
                fileStream.close();
            } else
            {
                std::cout << "\n,\n\n \"Incidence Graph\" : \n["
                          << getFormatedGraph(incidenceGraphStats.iGraphStats.iGraphFormat,
                                              incidenceGraphStats.getIncidenceGraph()) << "]";
            }
        }
        if (dependencyGraphStats.dGraphStatistics.graphFormat != NONE)
        {

            if (dependencyGraphStats.dGraphStatistics.dGraphPath.length() != 0)
            {
                std::ofstream fileStream;
                fileStream.open(dependencyGraphStats.dGraphStatistics.dGraphPath, std::ofstream::out);
                fileStream << getFormatedGraph(dependencyGraphStats.dGraphStatistics.graphFormat,
                                               dependencyGraphStats.getDependencyGraph());
                fileStream.close();
            } else
            {
                std::cout << "\n,\n\n \"Dependency Graph\" : \n["
                          << getFormatedGraph(dependencyGraphStats.dGraphStatistics.graphFormat,
                                              dependencyGraphStats.getDependencyGraph()) << "]";
            }
        }
        std::flush(std::cout);
    }

    void StatsCalculator::parseVariableLiteral(std::list<lit_type> body, std::list<lit_type> head)
    {
        std::list<exst::lit_type>::iterator it;
        for (it = body.begin(); it != body.end(); it++)
        {
            uint32_t id = it->id;
            if (generalStatistics.atomIdsOldNew.count(id) > 0)
            {
                if (it->s == -1)
                {
                    generalStatistics.variableNegative[id] = true;
                    generalStatistics.variableNegativeWithoutHelper[id] = true;
                } else
                {
                    generalStatistics.variablePositive[id] = true;
                    generalStatistics.variablePositiveWithoutHelper[id] = true;
                }
            }
        }
        for (it = head.begin(); it != head.end(); it++)
        {
            uint32_t id = it->id;
            if (generalStatistics.atomIdsOldNew.count(id) > 0)
            {
                if (it->s == -1)
                {
                    generalStatistics.variableNegative[id] = true;
                    generalStatistics.variableNegativeWithoutHelper[id] = true;
                } else
                {
                    generalStatistics.variablePositive[id] = true;
                    generalStatistics.variablePositiveWithoutHelper[id] = true;
                }
            }
        }
    }

    void StatsCalculator::setSymbolTable(std::unordered_map<uint32_t, const char *> &table)
    {
        this->generalStatistics.sTable = &table;
    }

    void StatsCalculator::calculateVariables()
    {
        std::list<unsigned int> rem;
        std::unordered_map<unsigned int, bool>::iterator it;
        for (it = generalStatistics.variablePositiveWithoutHelper.begin();
             it != generalStatistics.variablePositiveWithoutHelper.end(); it++)
        {
            if (generalStatistics.sTable->count(it->first) == 0)
            {
                rem.push_back((*it).first);
            }
        }
        while (rem.size() > 0)
        {
            generalStatistics.variablePositiveWithoutHelper.erase(rem.front());
            rem.pop_front();
        }
        for (it = generalStatistics.variableNegativeWithoutHelper.begin();
             it != generalStatistics.variableNegativeWithoutHelper.end(); it++)
        {
            if (generalStatistics.sTable->count((*it).first) == 0)
            {
                rem.push_back((*it).first);
            }
        }
        while (rem.size() > 0)
        {
            generalStatistics.variableNegativeWithoutHelper.erase(rem.front());
            rem.pop_front();
        }
    }

    bool parseParameter(StatsCalculator *inst, const std::string &name, const std::string &value)
    {
        GraphFormat format;
        bool file = false;
        std::string path;

        if (value.length() == 0)
        {
            return false;
        }
        try
        {
            switch (std::stoi(value.substr(0, 1), nullptr))
            {
                case 0 :
                    format = DIMACS;
                    break;
                case 1 :
                    format = GR;
                    break;
                case 2 :
                    format = GML;
                    break;
                default:
                    format = NONE;
            }
            if (value.length() > 1)
            {
                path = value.substr(2);
                file = true;
            }
        } catch (int e)
        {
            return false;
        }

        if (name.compare("printDgraph") == 0)
        {
            inst->dependencyGraphStats.dGraphStatistics.graphFormat = format;
            if (file)
            {
                inst->dependencyGraphStats.dGraphStatistics.dGraphPath = path;
            }
        } else if (name.compare("printIgraph") == 0)
        {
            inst->incidenceGraphStats.iGraphStats.iGraphFormat = format;
            if (file)
            {
                inst->incidenceGraphStats.iGraphStats.iGraphPath = path;
            }
        } else if (name.compare("printRgraph") == 0)
        {
            inst->incidenceGraphStats.iGraphStats.rGraphFormat = format;
            if (file)
            {
                inst->incidenceGraphStats.iGraphStats.rGraphPath = path;
            }
        } else
        {
            return false;
        }
        return true;
    }
}