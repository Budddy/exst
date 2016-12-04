#include <exst/program_stats.h>
#include <fstream>

namespace exst
{

    bool ProgramStatsCalculator::isConstraint(std::list<lit_type> head, std::list<lit_type> body)
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

    void ProgramStatsCalculator::addRule(std::list<lit_type> body, std::list<lit_type> head)
    {
        std::pair<std::list<lit_type>, std::list<lit_type>> rule;
        rule.first = head;
        rule.second = body;
        generalStatistics.rules.push_back(rule);
    }

    void ProgramStatsCalculator::calculateStats()
    {
        std::list<std::pair<std::list<exst::lit_type>, std::list<exst::lit_type>>>::iterator it;
        for (it = generalStatistics.rules.begin(); it != generalStatistics.rules.end(); it++)
        {
            std::list<lit_type> &head = it->first;
            std::list<lit_type> &body = it->second;

            if (head.size() > 0 && head.front().id == 1)
            {
                head.pop_front();
            }
            uint32_t negativeBody = 0, positiveBody = 0;
            uint32_t negativeHead = 0, positiveHead = 0;
            std::list<exst::lit_type>::iterator i;
            for (i = body.begin(); i != body.end(); i++)
            {
                if ((*i).s == -1)
                {
                    negativeBody++;
                } else
                {
                    positiveBody++;
                }
            };
            for (i = head.begin(); i != head.end(); i++)
            {
                if ((*i).s == -1)
                {
                    negativeHead++;
                } else
                {
                    positiveHead++;
                }
            };
            uint32_t positive = positiveBody + positiveHead, negative = negativeBody + negativeHead;

            generalStatistics.numAtomsHead += head.size();
            generalStatistics.numAtomsPositiveBody += positiveBody;
            generalStatistics.numAtomsNegativeBody += negativeBody;

            // facts
            if (body.size() == 0)
            {
                generalStatistics.numFacts++;
            }

            //rules
            {

                generalStatistics.maxSizeHeadNegBodyRule = (uint32_t) (generalStatistics.maxSizeHeadNegBodyRule <
                                                                       head.size() + negativeBody ? head.size() +
                                                                                                    negativeBody
                                                                                                  : generalStatistics.maxSizeHeadNegBodyRule);

                generalStatistics.maxNegativeRuleSizeBody = (uint32_t) (generalStatistics.maxNegativeRuleSizeBody <
                                                                        negativeBody ? negativeBody
                                                                                     : generalStatistics.maxNegativeRuleSizeBody);

                generalStatistics.maxHeadSizeRule = (uint32_t) (generalStatistics.maxHeadSizeRule < head.size()
                                                                ? head.size() : generalStatistics.maxHeadSizeRule);
            }

            //constraints
            if (head.size() == 0)
            {
                generalStatistics.numConstraints++;

                generalStatistics.atomOccurencesConstraint += head.size() + body.size();

                generalStatistics.maxPositiveSizeConstraint = (uint32_t) (generalStatistics.maxPositiveSizeConstraint <
                                                                          positive ? positive
                                                                                   : generalStatistics.maxPositiveSizeConstraint);

                generalStatistics.maxPositiveBodySizeConstraint = (uint32_t) (
                    generalStatistics.maxPositiveBodySizeConstraint < positiveBody ? positiveBody
                                                                                   : generalStatistics.maxPositiveBodySizeConstraint);
                generalStatistics.maxNegativeBodySizeConstraint = (uint32_t) (
                    generalStatistics.maxNegativeBodySizeConstraint < negativeBody ? negativeBody
                                                                                   : generalStatistics.maxNegativeBodySizeConstraint);

            }
                //non constraints
            else
            {

                generalStatistics.maxSizeNonConstraint = (uint32_t) (generalStatistics.maxSizeNonConstraint <
                                                                     head.size() + body.size() ? head.size() +
                                                                                                 body.size()
                                                                                               : generalStatistics.maxSizeNonConstraint);

                generalStatistics.atomOccurencesNonConstraint += head.size() + body.size();
                generalStatistics.maxPositiveRuleSizeNonConstraint = (uint32_t) (
                    generalStatistics.maxPositiveRuleSizeNonConstraint < positive ? positive
                                                                                  : generalStatistics.maxPositiveRuleSizeNonConstraint);

                generalStatistics.maxBodySizeNonConstraint = (uint32_t) (generalStatistics.maxBodySizeNonConstraint <
                                                                         body.size() ? body.size()
                                                                                     : generalStatistics.maxBodySizeNonConstraint);
            }

            //non horn clause
            if (!((head.size() == 0 && negative <= 1) || (head.size() == 1 && negative <= 0)) && (body.size() > 0))
                ++generalStatistics.numNonHornClauses;

            //non dual horn clause
            if ((body.size() - negative > 1) && (body.size() > 0))
                ++generalStatistics.numNonDualHornClauses;

            //clause size
            generalStatistics.maxClauseSize = (uint32_t) (generalStatistics.maxClauseSize < body.size() + head.size() ?
                                                          body.size() + head.size() : generalStatistics.maxClauseSize);
            //positive clause size
            generalStatistics.maxClauseSizePositive = (uint32_t) (generalStatistics.maxClauseSizePositive <
                                                                  body.size() - negative + head.size() ? body.size() -
                                                                                                         negative +
                                                                                                         head.size()
                                                                                                       : generalStatistics.maxClauseSizePositive);
            //negative clause size
            generalStatistics.maxClauseSizeNegative =
                generalStatistics.maxClauseSizeNegative < negative ? negative : generalStatistics.maxClauseSizeNegative;

            //atom occurrences
            countAtomOccurences(body, head);

            //number of variables that occur in positive/negative literals
            parseVariableLiteral(body, head);
        }
        calculateVariables();
    }

    void ProgramStatsCalculator::countAtomOccurences(std::list<lit_type> body, std::list<lit_type> head)
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

    void ProgramStatsCalculator::addId(uint32_t before, uint32_t after)
    {
        generalStatistics.atomIdsNewOld[after] = before;
        generalStatistics.atomIdsOldNew[before] = after;
    }

    std::list<std::pair<std::string, std::string>> ProgramStatsCalculator::getStatistics()
    {
        calculateStats();
        std::list<std::pair<std::string, std::string>> ret;
        ret.push_back(std::pair<std::string, std::string>("Non Horn Clauses",
                                                          std::to_string(generalStatistics.numNonHornClauses)));
        ret.push_back(std::pair<std::string, std::string>("Non Dual Horn Clauses",
                                                          std::to_string(generalStatistics.numNonDualHornClauses)));
        ret.push_back(
            std::pair<std::string, std::string>("max clause size", std::to_string(generalStatistics.maxClauseSize)));
        ret.push_back(std::pair<std::string, std::string>("max positive clause size",
                                                          std::to_string(generalStatistics.maxClauseSizePositive)));
        ret.push_back(std::pair<std::string, std::string>("max negative clause size",
                                                          std::to_string(generalStatistics.maxClauseSizeNegative)));
        ret.push_back(
            std::pair<std::string, std::string>("number of variables that occur as positive literals with helpers",
                                                std::to_string(generalStatistics.variablePositive.size())));
        ret.push_back(
            std::pair<std::string, std::string>("number of variables that occur as positive literals without helpers",
                                                std::to_string(
                                                    generalStatistics.variablePositiveWithoutHelper.size())));
        ret.push_back(
            std::pair<std::string, std::string>("number of variables that occur as negative literals with helpers",
                                                std::to_string(generalStatistics.variableNegative.size())));
        ret.push_back(
            std::pair<std::string, std::string>("number of variables that occur as negative literals without helpers",
                                                std::to_string(
                                                    generalStatistics.variableNegativeWithoutHelper.size())));
        ret.push_back(std::pair<std::string, std::string>("maximum positive rule size constraint",
                                                          std::to_string(generalStatistics.maxPositiveSizeConstraint)));
        ret.push_back(std::pair<std::string, std::string>("maximum positive rule size non-constraint", std::to_string(
            generalStatistics.maxPositiveRuleSizeNonConstraint)));
        ret.push_back(std::pair<std::string, std::string>("total number of atom occurrences constraint",
                                                          std::to_string(generalStatistics.atomOccurencesConstraint)));
        ret.push_back(std::pair<std::string, std::string>("total number of atom occurrences non-constraint",
                                                          std::to_string(
                                                              generalStatistics.atomOccurencesNonConstraint)));
        ret.push_back(std::pair<std::string, std::string>("maximum number of occurrences of an atom",
                                                          std::to_string(maxValue(generalStatistics.atomOccurences))));
        ret.push_back(std::pair<std::string, std::string>("maximum number of positive occurrences of an atom",
                                                          std::to_string(
                                                              maxValue(generalStatistics.atomOccurencesPositive))));
        ret.push_back(std::pair<std::string, std::string>("maximum number of negative occurrences of an atom",
                                                          std::to_string(
                                                              maxValue(generalStatistics.atomOccurencesNegative))));

        ret.push_back(std::pair<std::string, std::string>("maximum size of a non-constraint rule",
                                                          std::to_string(generalStatistics.maxSizeNonConstraint)));
        ret.push_back(std::pair<std::string, std::string>("maximum size of the head and negative body of a rule",
                                                          std::to_string(generalStatistics.maxSizeHeadNegBodyRule)));
        ret.push_back(std::pair<std::string, std::string>("maximum size of the head of a rule",
                                                          std::to_string(generalStatistics.maxHeadSizeRule)));
        ret.push_back(std::pair<std::string, std::string>("maximum size of the positive body of a non-constraint rule",
                                                          std::to_string(generalStatistics.maxBodySizeNonConstraint)));
        ret.push_back(std::pair<std::string, std::string>("maximum size of the negative body of a rule",
                                                          std::to_string(generalStatistics.maxNegativeRuleSizeBody)));
        ret.push_back(std::pair<std::string, std::string>("maximum size of the positive body of a constraint",
                                                          std::to_string(
                                                              generalStatistics.maxPositiveBodySizeConstraint)));
        ret.push_back(std::pair<std::string, std::string>("maximum size of the negative body of a constraint",
                                                          std::to_string(
                                                              generalStatistics.maxNegativeBodySizeConstraint)));
        ret.push_back(std::pair<std::string, std::string>("number of atoms that occur in the head",
                                                          std::to_string(generalStatistics.numAtomsHead)));
        ret.push_back(std::pair<std::string, std::string>("number of atoms that occur in the positive body",
                                                          std::to_string(generalStatistics.numAtomsPositiveBody)));
        ret.push_back(std::pair<std::string, std::string>("number of atoms that occur in the negative body",
                                                          std::to_string(generalStatistics.numAtomsNegativeBody)));
        ret.push_back(std::pair<std::string, std::string>("maximum number of occurences of a variable",
                                                          std::to_string(maxValue(generalStatistics.maxNumVarOcc))));
        ret.push_back(std::pair<std::string, std::string>(
            "maximum number of occurrences of a variable only head and negative body occurences",
            std::to_string(maxValue(generalStatistics.varOccurrHeadNegBody))));
        ret.push_back(std::pair<std::string, std::string>("maximal size of the model",
                                                          std::to_string(generalStatistics.modelSize)));

        return ret;
    }

    void ProgramStatsCalculator::addModel(const Clasp::ValueVec *model)
    {
        generalStatistics.modelSize = model->size();
    }

    void ProgramStatsCalculator::parseVariableLiteral(std::list<lit_type> body, std::list<lit_type> head)
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
                    generalStatistics.varOccurrHeadNegBody[id]++;
                } else
                {
                    generalStatistics.variablePositive[id] = true;
                    generalStatistics.variablePositiveWithoutHelper[id] = true;
                }
            }
            generalStatistics.maxNumVarOcc[id]++;
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
            generalStatistics.maxNumVarOcc[id]++;
            generalStatistics.varOccurrHeadNegBody[id]++;
        }
    }

    void ProgramStatsCalculator::setSymbolTable(std::unordered_map<uint32_t, const char *> &table)
    {
        this->generalStatistics.sTable = &table;
    }

    void ProgramStatsCalculator::calculateVariables()
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

}