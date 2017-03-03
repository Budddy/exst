#ifndef EXST_PROGRAM_STATISTICS_H
#define EXST_PROGRAM_STATISTICS_H

#include <exst/exst_types.h>
#include <list>
#include <clasp/literal.h>
#include <clasp/enumerator.h>

namespace exst {
    class StatisticsCalculator {
    public:
        virtual void addRule(std::list<lit_type> body, std::list<lit_type> head) {

        };

        virtual void updateAssignment(Clasp::LitVec new_assignment) {

        };

        virtual std::list<std::pair<std::string, std::string>> getStatistics() {
            return std::list<std::pair<std::string, std::string>>();
        }

        virtual void setSymbolTable(const Clasp::SymbolTable &symbolTable) {

        };

        virtual void addId(uint32_t before, uint32_t after) {

        };

        virtual std::list<std::string> getAdditionalStatistics() {
            return std::list<std::string>();
        };

        virtual void addModel(const Clasp::Model &model) {

        };
    };
}

#endif //EXST_PROGRAM_STATISTICS_H
