#ifndef CLASP_DEPENDENCY_GRAPH_PARAMS_H
#define CLASP_DEPENDENCY_GRAPH_PARAMS_H

#include <unordered_map>
#include <list>
#include <iostream>
#include <exst/exst_types.h>
#include <exst/dependency_graph_parameters.h>
#include <exst/interfaces.h>
#include <exst/program_flags.h>

namespace exst {

    /**
     * class used for calculating and saving stats of the dependency graph and the actual dependency graph
     */
    class DependencyGraphParameterCalculator : public ParametersCalculator {
    public:

        /**
         * structure to save the parameters which belong to the Dependency graph
         */
        struct DependencyGraphParameters {
            ///dependency graph
            MyGraph dependencyGraph;
            ///mapping from graph vertices to literals
            std::unordered_map<uint32_t, uint32_t> atomVertexMap;
        };
        DependencyGraphParameters dGraphStatistics;

        /**
         * not used in this calculator
         */
        virtual void updateAssignment(Clasp::LitVec new_assignment) {

        }

        /**
         * not used in this calculator
         */
        virtual void setSymbolTable(const Clasp::SymbolTable &symbolTable) {

        }

        /**
         * not used in this calculator
         */
        virtual void addId(uint32_t before, uint32_t after) {

        }

        /**
         * not used in this calculator
         */
        virtual void addAnswerSet(const Clasp::Model &model) {

        }

        /**
         * adds a rule to the dependency graph
         * @param body body of the rule
         * @param head head of the rule
         */
        virtual void addRule(std::list<lit_type> body, std::list<lit_type> head);

        /**
         * @return a list containing the number of nodes and edges in dependency graph
         */
        virtual std::list<std::pair<std::string, std::string>> getParameters();

        /**
         * @return the dependency graph
         */
        virtual std::list<std::string> getAdditionalParameters();

    };
}

#endif //CLASP_DEPENDENCY_GRAPH_PARAMS_H
