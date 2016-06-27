#ifndef CLASP_TEST_H
#define CLASP_TEST_H

#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/ui/text/TextTestRunner.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/BriefTestProgressListener.h>
#include <cppunit/CompilerOutputter.h>
#include <cppunit/XmlOutputter.h>
#include <cppunit/extensions/HelperMacros.h>
#include <exst/dependency_graph_stats.h>
#include <exst/extended_stats_calculator.h>

namespace exst
{
    class AbstractExstTest : public CppUnit::TestFixture
    {
    public:
        void setUp()
        {
            dependencyGraphStats = new DependencyGraphStats();
            statsCalculator = new StatsCalculator();
            atomIds = new std::unordered_map<uint32_t, uint32_t>();
            selectedAtoms = new std::unordered_map<uint32_t, bool>();
            graphStatsCalculator = new GraphStatsCalculator(*atomIds, *selectedAtoms);
            incidenceGraphStats = new IncidenceGraphStats(*atomIds, *selectedAtoms);
        }

        void tearDown()
        {
            delete dependencyGraphStats;
            delete statsCalculator;
            delete graphStatsCalculator;
            delete incidenceGraphStats;
            delete atomIds;
            delete selectedAtoms;
        }

    protected:
        DependencyGraphStats *dependencyGraphStats;
        StatsCalculator *statsCalculator;
        GraphStatsCalculator *graphStatsCalculator;
        IncidenceGraphStats *incidenceGraphStats;
        std::unordered_map<uint32_t, uint32_t> *atomIds;
        std::unordered_map<uint32_t, bool> *selectedAtoms;
    };
};

#endif //CLASP_TEST_H
