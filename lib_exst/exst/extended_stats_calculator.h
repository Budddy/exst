#ifndef CLASP_EXTENDED_STATS_CALCULATOR_H
#define CLASP_EXTENDED_STATS_CALCULATOR_H

#include <exst/ExstTypes.h>
#include <unordered_map>
#include <exst/graph_stats_calculator.h>

namespace exst
{
    class StatsCalculator
    {
    public:

        /*
         * returns the single instance of the GraphStatsCalculator
         */
        static StatsCalculator &getInstance()
        {
            static StatsCalculator calc;
            return calc;
        }

        /*
         * adds a dependency to the dependency graph
         */
        void parseRule(Clasp::WeightLitVec &body, Clasp::PodVector<uint32>::type &head);

        /*
         * maximum number of occurrences of an atom
         */
        void countAtomOccurences(Clasp::WeightLitVec &body, Clasp::PodVector<uint32>::type &head);

        /*
         * maps the new id to the old id
         */
        void addId(uint32 before, uint32 after);

        void setSymbolTable(Clasp::SymbolTable &table);

        /*
         *
         */
        void printExtendedStats();

        uint32 maxValue(std::unordered_map<uint32, uint32> values)
        {
            uint32 maxValue = 0;
            std::unordered_map<unsigned int, unsigned int>::iterator it;
            for (it = values.begin(); it != values.end(); it++)
            {
                maxValue = it->second > maxValue ? it->second : maxValue;
            }
            return maxValue;
        }

        uint32 minValue(std::unordered_map<uint32, uint32> values)
        {
            uint32 minValue = UINT32_MAX;
            std::unordered_map<unsigned int, unsigned int>::iterator it;
            for (it = values.begin(); it != values.end(); it++)
            {
                minValue = it->second < minValue ? it->second : minValue;
            }
            return minValue;
        }

        //number of facts in the program
        uint32 numFacts = 0;
        //number of rules in the program
        uint32 numRules = 0;
        //TODO number of clauses in the program
        uint32 numClauses = 0;
        //atoms of current assignment
        std::unordered_map<uint32, bool> selectedAtoms;
        //used for matching literal ids before and after pre processing
        std::unordered_map<int32, uint32> atomIds;
        //number of not horn clauses in the program
        uint32 numNonHornClauses = 0;
        //number of dual horn clauses in the program
        uint32 numNonDualHornClauses = 0;
        //symbol table to match the atom ids with their symbols
        Clasp::SymbolTable* sTable;
        //maximal clause size
        uint64 maxClauseSize = 0;
        //maximal positive clause size
        uint64 maxClauseSizePositive = 0;
        //maximal negative clause size
        uint32 maxClauseSizeNegative = 0;
        // occurences of an atom
        std::unordered_map<uint32, uint32> atomOccurences;
        // positive occurences of an atom
        std::unordered_map<uint32, uint32> atomOccurencesPositive;
        // negative occurences of an atom
        std::unordered_map<uint32, uint32> atomOccurencesNegative;
        //
        GraphStatsCalculator graphStatsCalculator;
        //variable occurs negative
        std::unordered_map<uint32, bool> variableNegative;
        //variable occurs positive
        std::unordered_map<uint32, bool> variablePositive;
        //negative variable occurrences without helpers
        std::unordered_map<uint32, bool> variableNegativeWithoutHelper;
        //positive variable occurrences without helpers
        std::unordered_map<uint32, bool> variablePositiveWithoutHelper;
        //TODO
        uint32 constraint = 0;
        //TODO
        uint32 maxWeightMinModel = 0;
        //TODO
        uint32 maxPositiveRuleSizeConstraint = 0;
        //TODO
        uint32 maxPositiveRuleSizeNonConstraint = 0;
        //TODO
        uint32 atomOccurencesConstraint = 0;
        //TODO
        uint32 atomOccurencesNonConstraint = 0;

    private:

        //private constructors for singleton
        StatsCalculator() : graphStatsCalculator(atomIds, selectedAtoms)
        {
        };

        void parseVariableLiteral(Clasp::WeightLitVec &body, Clasp::PodVector<unsigned int>::type &head);
        void variableOccurrences(Clasp::WeightLitVec &body, Clasp::PodVector<unsigned int>::type &head);
    };
}
#endif //CLASP_EXTENDED_STATS_CALCULATOR_H
