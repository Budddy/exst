#ifndef EXST_PARAMETERS_CALCULATOR_H
#define EXST_PARAMETERS_CALCULATOR_H

#include <exst/exst_types.h>
#include <list>
#include <clasp/literal.h>
#include <clasp/enumerator.h>

namespace exst {
    class ParametersCalculator {
    public:

        /**
         * supplies the calculator with the program rules
         * @param body contains the body literals
         * @param head contains the head literals
         */
        virtual void addRule(std::list<lit_type> body, std::list<lit_type> head)= 0;

        /**
         * supplies the calculator with the current assignment
         * @param new_assignment the new assignment
         */
        virtual void updateAssignment(Clasp::LitVec new_assignment)= 0;

        /**
         * returns the parameters of the calculator
         * @return the parameters
         */
        virtual std::list<std::pair<std::string, std::string>> getParameters()= 0;

        /**
         * supplies the calculator with the clasp symbol table
         * @param symbolTable the clasp symbol table
         */
        virtual void setSymbolTable(const Clasp::SymbolTable &symbolTable)= 0;

        /**
         * supplies the calculator with the atom ids before and after preprocessing
         * @param before the id before preprocessing
         * @param after the id after preprocessing
         */
        virtual void addId(uint32_t before, uint32_t after)= 0;

        /**
         * returns the additional parameters of a calculator
         * @return the additional paramters
         */
        virtual std::list<std::string> getAdditionalParameters()= 0;

        /**
         * supplies the calculator with the answer set
         * @param answerSet the answer set
         */
        virtual void addAnswerSet(const Clasp::Model &model)= 0;
    };
}

#endif //EXST_PARAMETERS_CALCULATOR_H
