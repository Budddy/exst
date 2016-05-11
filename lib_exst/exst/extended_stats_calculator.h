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
        void parseRule(std::vector<uint32> dependencies, Clasp::PodVector<uint32>::type heads, uint32 negative);
        /*
         *
         */
        void countAtomOccurences(std::vector<uint32> &dependencies, Clasp::PodVector<uint32>::type &heads);
        /*
         *
         */
        void parseAtomOccurencesNegative(uint32 &negative, std::vector<uint32> &dependencies);
        /*
         *
         */
        void parseAtomOccurencesPositive(std::vector<uint32> &dependencies, Clasp::PodVector<uint32>::type &heads,
                                         uint32 &negative);
        /*
         * maps the new id to the old id
         */
        void addId(uint32 before, uint32 after);
        /*
         *
         */
        void printExtendedStats();
        /*
         *
         */
        void countAtomOccurencesTotal(std::vector<uint32> &dependencies, Clasp::PodVector<uint32>::type &heads);

        //
        uint32 numFacts = 0;
        //
        uint32 numRules = 0;
        //atoms of current assignment
        std::unordered_map<uint32, bool> selectedAtoms;
        //used for matching literal ids before and after pre processing
        std::unordered_map<int32, uint32> atomIds;
        //
        uint32 numNonHornClauses = 0;
        //
        uint32 numNonDualHornClauses = 0;
        //
        const Clasp::SymbolTable *sTable;
        //
        uint32 numClauses = 0;
        //
        unsigned long clauseSize = 0;
        //
        unsigned long clauseSizePositive = 0;
        //
        uint32 clauseSizeNegative = 0;
        //
        std::unordered_map<uint32, uint32> atomOccurences;
        //
        std::unordered_map<uint32, uint32> atomOccurencesPositive;
        //
        std::unordered_map<uint32, uint32> atomOccurencesNegative;
        //
        std::unordered_map<uint32, uint32> atomOccurencesTotal;
        //
        GraphStatsCalculator graphStatsCalculator;

    private:

        //private constructors for singleton
        StatsCalculator() : graphStatsCalculator(atomIds, selectedAtoms)
        {
        };
    };
}
#endif //CLASP_EXTENDED_STATS_CALCULATOR_H
