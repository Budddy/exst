#ifndef EXST_EXST_STATISTICS_H
#define EXST_EXST_STATISTICS_H

#include <exst/exst_types.h>
#include <exst/interfaces.h>
#include <list>
#include <clasp/literal.h>
#include <exst/dependency_graph_stats.h>
#include <exst/program_stats.h>
#include <exst/incidence_graph_stats.h>

namespace exst {
    class ExtendedStatistics {
    public:
        static ExtendedStatistics &getInstance() {
            static ExtendedStatistics stats;
            return stats;
        }

        void addRule(std::list<lit_type> body, std::list<lit_type> head);

        void printStatistics();

        void updateAssignment(Clasp::LitVec new_assignment);

        void setSymbolTable(std::unordered_map<uint32_t, const char *> &table);

        void addModel(const Clasp::ValueVec *model);

        void addId(uint32_t before, uint32_t after);


        std::list<StatisticsCalculator *> programStatistics;

        void registerProgramStatistics(StatisticsCalculator *pStats) {
            programStatistics.push_back(pStats);
        }

        void clearProgramStatistics() {
            programStatistics.clear();
        }

    private:
        ExtendedStatistics() {
            registerProgramStatistics(new DependencyGraphStatsCalculator());
            registerProgramStatistics(new ProgramStatsCalculator());
            if (!ProgramParameter::getInstance().stopIGraphGen) {
                registerProgramStatistics(new IncidenceGraphStatsCalculator());
            }
        }

    };

}

#endif //EXST_EXST_STATISTICS_H
