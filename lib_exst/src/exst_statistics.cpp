#include <exst/exst_statistics.h>

namespace exst
{

    void ExtendedStatistics::addRule(std::list<lit_type> body, std::list<lit_type> head)
    {

        std::list<exst::StatisticsCalculator *>::iterator it;
        for (it = programStatistics.begin(); it != programStatistics.end(); it++)
        {
            (*it)->addRule(body, head);
        }

    }

    void ExtendedStatistics::printStatistics()
    {
        std::list<std::pair<std::string, std::string>> stats;
        std::list<exst::StatisticsCalculator *>::iterator it;
        for (it = programStatistics.begin(); it != programStatistics.end(); it++)
        {
            stats.merge((*it)->getStatistics());
        }

        std::cout << ",\"Extended Stats\" : [\n";
        std::list<std::pair<std::string, std::string>>::iterator it2;
        bool begin = true;
        for (it2 = stats.begin(); it2 != stats.end(); it2++)
        {
            if (begin)
            {
                std::cout << "  [\"" << it2->first << "\" , " << it2->second << "]\n";
                begin = false;
            } else
            {
                std::cout << "  ,[\"" << it2->first << "\" , " << it2->second << "]\n";
            }
        }
        std::cout << "]";

        std::list<exst::StatisticsCalculator *>::iterator it3;
        for (it3 = programStatistics.begin(); it3 != programStatistics.end(); it3++)
        {
            std::list<std::string>::iterator it3_;
            std::list<std::string> addstats = (*it3)->getAdditionalStatistics();
            for (it3_ = addstats.begin(); it3_ != addstats.end(); it3_++)
            {
                std::cout << "," << (*it3_);
            }
        }

        std::cout.flush();
    }

    void ExtendedStatistics::updateAssignment(Clasp::LitVec new_assignment)
    {

        std::list<exst::StatisticsCalculator *>::iterator it;
        for (it = programStatistics.begin(); it != programStatistics.end(); it++)
        {
            (*it)->updateAssignment(new_assignment);
        }

    }

    void ExtendedStatistics::setSymbolTable(std::unordered_map<uint32_t, const char *> &table)
    {

        std::list<exst::StatisticsCalculator *>::iterator it;
        for (it = programStatistics.begin(); it != programStatistics.end(); it++)
        {
            (*it)->setSymbolTable(table);
        }

    }

    void ExtendedStatistics::addId(uint32_t before, uint32_t after)
    {

        std::list<exst::StatisticsCalculator *>::iterator it;
        for (it = programStatistics.begin(); it != programStatistics.end(); it++)
        {
            (*it)->addId(before, after);
        }

    }

    void ExtendedStatistics::addModel(const Clasp::ValueVec *model)
    {

        std::list<exst::StatisticsCalculator *>::iterator it;
        for (it = programStatistics.begin(); it != programStatistics.end(); it++)
        {
            (*it)->addModel(model);
        }

    }

}