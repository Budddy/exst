#ifndef EXST_EXST_STATISTICS_H
#define EXST_EXST_STATISTICS_H

#include <exst/exst_types.h>
#include <exst/interfaces.h>
#include <list>
#include <clasp/literal.h>
#include <exst/dependency_graph_parameters.h>
#include <exst/program_parameters.h>
#include <exst/incidence_graph_parameters.h>
#include <clasp/enumerator.h>

namespace exst {
    class ExtendedParameters {
    public:
        static ExtendedParameters &getInstance() {
            static ExtendedParameters stats;
            return stats;
        }

        void addRule(std::list<lit_type> body, std::list<lit_type> head);

        void printParameters();

        void updateAssignment(Clasp::LitVec new_assignment);

        void setSymbolTable(const Clasp::SymbolTable &symbolTable);

        void addModel(const Clasp::Model &model);

        void addId(uint32_t before, uint32_t after);


        std::list<ParametersCalculator *> parameterCalc;

        void registerParameterCalculator(ParametersCalculator *pStats) {
            parameterCalc.push_back(pStats);
        }

    private:
        ExtendedParameters() {
            registerParameterCalculator(new DependencyGraphParameterCalculator());
            registerParameterCalculator(new ProgramParameterCalculator());
            registerParameterCalculator(new IncidenceGraphParameterCalculator());
        }

    };

}

#endif //EXST_EXST_STATISTICS_H
