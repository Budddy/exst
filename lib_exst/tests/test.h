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
#include <exst/dependency_graph_parameters.h>
#include <exst/program_parameters.h>
#include <exst/exst_parameters.h>

namespace exst {
    class AbstractExstTest : public CppUnit::TestFixture {
    public:
        void setUp() {
            extendedStatistics->programStatistics.clear();
            dependencyGraphStats = new DependencyGraphStatsCalculator();
            statsCalculator = new ProgramStatsCalculator();
            incidenceGraphStats = new IncidenceGraphStatsCalculator();
            atomIds = &incidenceGraphStats->iGraphStats.atomIds;
            extendedStatistics->registerProgramStatistics(dependencyGraphStats);
            extendedStatistics->registerProgramStatistics(statsCalculator);
            extendedStatistics->registerProgramStatistics(incidenceGraphStats);
            ProgramParameter::getInstance() = *new ProgramParameter();
        }

        void tearDown() {
            delete dependencyGraphStats;
            delete incidenceGraphStats;
            delete statsCalculator;

        }

    protected:
        DependencyGraphStatsCalculator *dependencyGraphStats;
        ProgramStatsCalculator *statsCalculator;
        IncidenceGraphStatsCalculator *incidenceGraphStats;
        std::unordered_map<uint32_t, uint32_t> *atomIds;
        ExtendedStatistics *extendedStatistics = &ExtendedStatistics::getInstance();
    };
};

#endif //CLASP_TEST_H
