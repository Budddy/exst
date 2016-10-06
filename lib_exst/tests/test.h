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
            dependencyGraphStats = new DependencyGraphStatsCalculator();
            statsCalculator = new StatsCalculator();
            atomIds = new std::unordered_map<uint32_t, uint32_t>();
            incidenceGraphStats = new IncidenceGraphStatsCalculator(atomIds);
        }

        void tearDown()
        {
            delete dependencyGraphStats;
            delete incidenceGraphStats;
            delete atomIds;
            delete statsCalculator;
        }

    protected:
        DependencyGraphStatsCalculator *dependencyGraphStats;
        StatsCalculator *statsCalculator;
        IncidenceGraphStatsCalculator *incidenceGraphStats;
        std::unordered_map<uint32_t, uint32_t> *atomIds;
    };
};

#endif //CLASP_TEST_H
