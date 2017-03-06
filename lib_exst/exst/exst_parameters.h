#ifndef EXST_EXST_PARAMETERS_H
#define EXST_EXST_PARAMETERS_H

#include <exst/exst_types.h>
#include <exst/interfaces.h>
#include <list>
#include <clasp/literal.h>
#include <exst/dependency_graph_parameters.h>
#include <exst/program_parameters.h>
#include <exst/incidence_graph_parameters.h>
#include <clasp/enumerator.h>

namespace exst {
    /**
     * TODO
     */
    class ExtendedParameters {
    public:
        static ExtendedParameters &getInstance() {
            static ExtendedParameters param;
            return param;
        }

        /**
         * supplies the extension with the program rules
         * @param body contains the body literals
         * @param head contains the head literals
         */
        void addRule(std::list<lit_type> body, std::list<lit_type> head);

        /**
         * prints the parameters of the extension
         */
        void printParameters();

        /**
         * supplies the extension with the current assignment
         * @param new_assignment the new assignment
         */
        void updateAssignment(Clasp::LitVec new_assignment);

        /**
         * supplies the extension with the clasp symbol table
         * @param symbolTable the clasp symbol table
         */
        void setSymbolTable(const Clasp::SymbolTable &symbolTable);

        /**
         * supplies the extension with the answer set
         * @param answerSet the answer set
         */
        void addAnswerSet(const Clasp::Model &answerSet);

        /**
         * supplies the extension with the atom ids before and after preprocessing
         * @param before the id before preprocessing
         * @param after the id after preprocessing
         */
        void addId(uint32_t before, uint32_t after);

        /// the parameter calculators
        std::list<ParametersCalculator *> parameterCalc;

        /**
         * adds a calculator to the currently registered parameter calculators
         * @param paramCalc the new parameter calculator
         */
        void registerParameterCalculator(ParametersCalculator *paramCalc) {
            parameterCalc.push_back(paramCalc);
        }

    private:
        ExtendedParameters() {
            registerParameterCalculator(new DependencyGraphParameterCalculator());
            registerParameterCalculator(new ProgramParameterCalculator());
            registerParameterCalculator(new IncidenceGraphParameterCalculator());
        }
    };
}

#endif //EXST_EXST_PARAMETERS_H
