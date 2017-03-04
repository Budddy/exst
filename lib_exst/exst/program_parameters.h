#ifndef CLASP_EXTENDED_STATS_CALCULATOR_H
#define CLASP_EXTENDED_STATS_CALCULATOR_H

#include <unordered_map>
#include <list>
#include <exst/exst_types.h>
#include <exst/incidence_graph_parameters.h>
#include <exst/dependency_graph_parameters.h>
#include <exst/program_flags.h>

namespace exst {
    struct ProgramStatistics {

        ///number of facts in the program
        uint32_t numFacts = 0;

        ///rules
        std::list<std::pair<std::list<lit_type>, std::list<lit_type>>> rules;

        ///number of not horn clauses in the program
        uint32_t numNonHornClauses = 0;

        ///number of dual horn clauses in the program
        uint32_t numNonDualHornClauses = 0;

        ///maximal clause size
        uint32_t maxClauseSize = 0;

        ///maximal positive clause size
        uint32_t maxClauseSizePositive = 0;

        ///maximal negative clause size
        uint32_t maxClauseSizeNegative = 0;

        ///occurences of an atom
        std::unordered_map<uint32_t, uint32_t> atomOccurences;

        ///positive occurences of an atom
        std::unordered_map<uint32_t, uint32_t> atomOccurencesPositive;

        ///negative occurences of an atom
        std::unordered_map<uint32_t, uint32_t> atomOccurencesNegative;

        ///variable occurs negative
        std::unordered_map<uint32_t, bool> variableNegative;

        ///variable occurs positive
        std::unordered_map<uint32_t, bool> variablePositive;

        ///negative variable occurrences without helpers
        std::unordered_map<uint32_t, bool> variableNegativeWithoutHelper;

        ///positive variable occurrences without helpers
        std::unordered_map<uint32_t, bool> variablePositiveWithoutHelper;

        ///number of constraints in the program
        uint32_t numConstraints = 0;

        ///maximum size of the positive rules in constraints
        uint32_t maxPositiveSizeConstraint = 0;

        ///maximum size of the positive rules in non constraints
        uint32_t maxPositiveRuleSizeNonConstraint = 0;

        ///num of atom occurences in constraints
        uint32_t atomOccurencesConstraint = 0;

        ///num of atom occurences in non constraints
        uint32_t atomOccurencesNonConstraint = 0;

        uint32_t maxNegativeRuleSizeBody = 0;
        uint32_t maxPositiveBodySizeConstraint = 0;
        uint32_t maxNegativeBodySizeConstraint = 0;
        unsigned long numAtomsHead = 0;
        uint32_t numAtomsPositiveBody = 0;
        uint32_t numAtomsNegativeBody = 0;
        uint32_t maxHeadSizeRule = 0;
        uint32_t maxBodySizeNonConstraint = 0;
        uint32_t maxSizeHeadNegBodyRule = 0;
        unsigned long maxSizeNonConstraint = 0;
        std::unordered_map<uint32_t, uint32_t> maxNumVarOcc;
        std::unordered_map<uint32_t, uint32_t> varOccurrHeadNegBody;
        unsigned int modelSize;
        const Clasp::SymbolTable *symbolTable;
    };

    /**
     * class used to generate the extended stats
     */
    class ProgramStatsCalculator : public StatisticsCalculator {
    public:

        virtual void addRule(std::list<lit_type> body, std::list<lit_type> head);

        virtual void addModel(const Clasp::Model &model);

        virtual std::list<std::pair<std::string, std::string>> getStatistics();

        /**
         * Used to set the Symbol Table.
         * @param table the symbol table of the program
         */
        virtual void setSymbolTable(const Clasp::SymbolTable &symbolTable);

        /**
         * Starts the calculation of the stats, implicitly called by printExtendedStats.
         */
        void calculateStats();

        ProgramStatistics generalStatistics;

    private:

        /*
         *
         */
        void parseVariableLiteral(std::list<lit_type> body, std::list<lit_type> head);

        /*
         *
         */
        void countAtomOccurences(std::list<lit_type> body, std::list<lit_type> head);

        /*
         *
         */
        void removeAuxiliaryVariables(std::unordered_map<uint32_t, bool> *variables) const;
    };


}
#endif //CLASP_EXTENDED_STATS_CALCULATOR_H