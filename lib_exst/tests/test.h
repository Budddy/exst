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
            extendedStatistics->parameterCalc.clear();
            dependencyGraphStats = new DependencyGraphParameterCalculator();
            statsCalculator = new ProgramParameterCalculator();
            incidenceGraphStats = new IncidenceGraphParameterCalculator();
            atomIds = &incidenceGraphStats->iGraphStats.atomIds;
            extendedStatistics->registerParameterCalculator(dependencyGraphStats);
            extendedStatistics->registerParameterCalculator(statsCalculator);
            extendedStatistics->registerParameterCalculator(incidenceGraphStats);
            ExstFlags::getInstance() = *new ExstFlags();
        }

        void tearDown() {
            delete dependencyGraphStats;
            delete incidenceGraphStats;
            delete statsCalculator;

        }

    protected:
        DependencyGraphParameterCalculator *dependencyGraphStats;
        ProgramParameterCalculator *statsCalculator;
        IncidenceGraphParameterCalculator *incidenceGraphStats;
        std::unordered_map<uint32_t, uint32_t> *atomIds;
        ExtendedParameters *extendedStatistics = &ExtendedParameters::getInstance();
    };
};

#endif //CLASP_TEST_H
