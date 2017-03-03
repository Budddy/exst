#include "test.h"

namespace exst {
    class ParameterTests : public AbstractExstTest {
    CPPUNIT_TEST_SUITE(ParameterTests);
            CPPUNIT_TEST(testParseParameter);
            CPPUNIT_TEST(testParseParameterEmptyName);
            CPPUNIT_TEST(testParseParameterEmptyValue);
            CPPUNIT_TEST(testParseParameterWrongName);
            CPPUNIT_TEST(testParseParameterWrongValue);
        CPPUNIT_TEST_SUITE_END();
    private:
    public:

        void testParseParameter() {
            int i = 0;
            CPPUNIT_ASSERT_MESSAGE("", ProgramParameter::parseParameter(nullptr, std::string("printDgraph"),
                                                                        std::string("0,./dGraph")));
            CPPUNIT_ASSERT_EQUAL_MESSAGE("", GraphFormat::DIMACS, ProgramParameter::getInstance().dGraphFormat);
            CPPUNIT_ASSERT_EQUAL_MESSAGE("", std::string("./dGraph"), ProgramParameter::getInstance().dGraphPath);

            CPPUNIT_ASSERT_MESSAGE("", ProgramParameter::parseParameter(nullptr, std::string("printIgraph"),
                                                                        std::string("1,./iGraph")));
            CPPUNIT_ASSERT_EQUAL_MESSAGE("", GraphFormat::GR, ProgramParameter::getInstance().iGraphFormat);
            CPPUNIT_ASSERT_EQUAL_MESSAGE("", std::string("./iGraph"), ProgramParameter::getInstance().iGraphPath);

            CPPUNIT_ASSERT_MESSAGE("", ProgramParameter::parseParameter(nullptr, std::string("printRgraph"),
                                                                        std::string("2,./rGraph")));
            CPPUNIT_ASSERT_EQUAL_MESSAGE("", GraphFormat::GML, ProgramParameter::getInstance().rGraphFormat);
            CPPUNIT_ASSERT_EQUAL_MESSAGE("", std::string("./rGraph"), ProgramParameter::getInstance().rGraphPath);
        }

        void testParseParameterEmptyName() {
            CPPUNIT_ASSERT_MESSAGE("", not ProgramParameter::parseParameter(nullptr, std::string(""),
                                                                            std::string("0,./dGraph")));
        }

        void testParseParameterEmptyValue() {
            CPPUNIT_ASSERT_MESSAGE("", not ProgramParameter::parseParameter(nullptr, std::string("printDgraph"),
                                                                            std::string("")));
        }

        void testParseParameterWrongName() {
            CPPUNIT_ASSERT_MESSAGE("", not ProgramParameter::parseParameter(nullptr, std::string("wrong"),
                                                                            std::string("0,./dGraph")));
        }

        void testParseParameterWrongValue() {
            CPPUNIT_ASSERT_MESSAGE("", not ProgramParameter::parseParameter(nullptr, std::string("printDgraph"),
                                                                            std::string("9h")));
        }
    };
};