#include "test.h"

namespace exst
{
    class SystemTests : public AbstractExstTest
    {
    CPPUNIT_TEST_SUITE(SystemTests);
            CPPUNIT_TEST(testMultipleConstraintsAndNonConstraintsWithHelpersSystem);
        CPPUNIT_TEST_SUITE_END();
    private:
    public:
        void setUp()
        {
            AbstractExstTest::setUp();
        }

        void tearDown()
        {
            AbstractExstTest::tearDown();
        }

        void testMultipleConstraintsAndNonConstraintsWithHelpersSystem()
        {

            std::list<lit_type> body;
            std::list<lit_type> head;
            body.push_back(*new lit_type(2, NEGATIVE));
            body.push_back(*new lit_type(3, NEGATIVE));
            statsCalculator->parseRule(body, head);

            head.clear();
            body.clear();
            body.push_back(*new lit_type(4, NEGATIVE));
            body.push_back(*new lit_type(5, POSITIVE));
            statsCalculator->parseRule(body, head);

            head.clear();
            body.clear();
            body.push_back(*new lit_type(5, POSITIVE));
            body.push_back(*new lit_type(6, POSITIVE));
            head.push_back(*new lit_type(7, POSITIVE));
            statsCalculator->parseRule(body, head);

            head.clear();
            body.clear();
            head.push_back(*new lit_type(8, POSITIVE));
            statsCalculator->parseRule(body, head);

            head.clear();
            body.clear();
            head.push_back(*new lit_type(9, NEGATIVE));
            statsCalculator->parseRule(body, head);

            statsCalculator->addId(2, 2);
            statsCalculator->addId(3, 3);
            statsCalculator->addId(4, 4);
            statsCalculator->addId(5, 5);
            statsCalculator->addId(6, 6);
            statsCalculator->addId(7, 7);

            std::unordered_map<uint32_t, const char *> table;
            table[3] = "";
            table[5] = "";
            table[6] = "";
            table[7] = "";
            table[8] = "";
            table[9] = "";

            statsCalculator->setSymbolTable(table);

            std::streambuf *oldCoutStreamBuf = std::cout.rdbuf();
            std::ostringstream strCout;
            std::cout.rdbuf(strCout.rdbuf());
            statsCalculator->printExtendedStats();
            std::cout.rdbuf(oldCoutStreamBuf);

            std::stringstream res(strCout.str());
            std::string line;

            std::getline(res, line, '\n');
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of Non Horn Clauses", line, std::string(""));

            std::getline(res, line, '\n');
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of Non Horn Clauses", line, std::string(","));

            std::getline(res, line, '\n');
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of Non Horn Clauses", line, std::string(""));

            std::getline(res, line, '\n');
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of Non Horn Clauses", line, std::string(" \"Extended Stats\" : ["));

            std::getline(res, line, '\n');
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of Non Horn Clauses", line,
                                         std::string("  [\"Dependency Graph Nodes\" , 8],"));

            std::getline(res, line, '\n');
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of Non Horn Clauses", line,
                                         std::string("  [\"Dependency Graph Edges\" , 2],"));

            std::getline(res, line, '\n');
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of Non Horn Clauses", line,
                                         std::string("  [\"Incidence Graph Nodes\" , 9],"));

            std::getline(res, line, '\n');
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of Non Horn Clauses", line,
                                         std::string("  [\"Incidence Graph Edges\" , 7],"));

            std::getline(res, line, '\n');
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of Non Horn Clauses", line,
                                         std::string("  [\"Non Horn Clauses\" , 1],"));

            std::getline(res, line, '\n');
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of Non Horn Clauses", line,
                                         std::string("  [\"Non Dual Horn Clauses\" , 1],"));

            std::getline(res, line, '\n');
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of Non Horn Clauses", line,
                                         std::string("  [\"max clause size\" , 3],"));

            std::getline(res, line, '\n');
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of Non Horn Clauses", line,
                                         std::string("  [\"max positive clause size\" , 3],"));

            std::getline(res, line, '\n');
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of Non Horn Clauses", line,
                                         std::string("  [\"max negative clause size\" , 2],"));

            std::getline(res, line, '\n');
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of Non Horn Clauses", line, std::string(
                    "  [\"number of variables that occur as positive literals with helpers\" , 3],"));

            std::getline(res, line, '\n');
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of Non Horn Clauses", line, std::string(
                    "  [\"number of variables that occur as positive literals without helpers\" , 3],"));

            std::getline(res, line, '\n');
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of Non Horn Clauses", line, std::string(
                    "  [\"number of variables that occur as negative literals with helpers\" , 3],"));

            std::getline(res, line, '\n');
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of Non Horn Clauses", line, std::string(
                    "  [\"number of variables that occur as negative literals without helpers\" , 1],"));

            std::getline(res, line, '\n');
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of Non Horn Clauses", line,
                                         std::string("  [\"maximum positive rule size constraint\" , 3],"));

            std::getline(res, line, '\n');
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of Non Horn Clauses", line,
                                         std::string("  [\"maximum positive rule size non-constraint\" , 1],"));

            std::getline(res, line, '\n');
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of Non Horn Clauses", line,
                                         std::string("  [\"total number of atom occurrences constraint\" , 7],"));

            std::getline(res, line, '\n');
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of Non Horn Clauses", line,
                                         std::string("  [\"total number of atom occurrences non-constraint\" , 2],"));

            std::getline(res, line, '\n');
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of Non Horn Clauses", line,
                                         std::string("  [\"maximum number of occurrences of an atom\" , 2],"));

            std::getline(res, line, '\n');
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of Non Horn Clauses", line, std::string(
                    "  [\"maximum number of positive occurrences of an atom\" , 2],"));

            std::getline(res, line, '\n');
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of Non Horn Clauses", line, std::string(
                    "  [\"maximum number of negative occurrences of an atom\" , 1]"));

            std::getline(res, line, '\n');
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of Non Horn Clauses", line, std::string(" ]"));

        }
    };

    CPPUNIT_TEST_SUITE_REGISTRATION(SystemTests);
};