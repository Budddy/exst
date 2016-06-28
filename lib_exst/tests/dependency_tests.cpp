#include "test.h"

namespace exst
{
    class DependencyTests : public AbstractExstTest
    {
    CPPUNIT_TEST_SUITE(DependencyTests);
            CPPUNIT_TEST(testCreateGraph);
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

        void testCreateGraph(){

            std::list<lit_type> body;
            std::list<lit_type> head;
            body.push_back(*new lit_type(2, NEGATIVE));
            body.push_back(*new lit_type(3, NEGATIVE));
            dependencyGraphStats->addRuleDependencyGraph(body, head);

            CPPUNIT_ASSERT_EQUAL_MESSAGE("Edge Count", (uint32_t) 0, dependencyGraphStats->edgecount);
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Node Count", (uint32_t) 2, dependencyGraphStats->nodecount);

            head.clear();
            body.clear();
            body.push_back(*new lit_type(4, NEGATIVE));
            body.push_back(*new lit_type(5, POSITIVE));
            dependencyGraphStats->addRuleDependencyGraph(body, head);

            CPPUNIT_ASSERT_EQUAL_MESSAGE("Edge Count", (uint32_t) 0, dependencyGraphStats->edgecount);
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Node Count", (uint32_t) 4, dependencyGraphStats->nodecount);

            head.clear();
            body.clear();
            body.push_back(*new lit_type(5, POSITIVE));
            body.push_back(*new lit_type(6, POSITIVE));
            head.push_back(*new lit_type(7, POSITIVE));
            dependencyGraphStats->addRuleDependencyGraph(body, head);

            CPPUNIT_ASSERT_EQUAL_MESSAGE("Edge Count", (uint32_t) 2, dependencyGraphStats->edgecount);
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Node Count", (uint32_t) 6, dependencyGraphStats->nodecount);

            head.clear();
            body.clear();
            head.push_back(*new lit_type(8, POSITIVE));
            dependencyGraphStats->addRuleDependencyGraph(body, head);

            CPPUNIT_ASSERT_EQUAL_MESSAGE("Edge Count", (uint32_t) 2, dependencyGraphStats->edgecount);
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Node Count", (uint32_t) 7, dependencyGraphStats->nodecount);

            head.clear();
            body.clear();
            head.push_back(*new lit_type(9, NEGATIVE));
            dependencyGraphStats->addRuleDependencyGraph(body, head);

            CPPUNIT_ASSERT_EQUAL_MESSAGE("Edge Count", (uint32_t) 2, dependencyGraphStats->edgecount);
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Node Count", (uint32_t) 8, dependencyGraphStats->nodecount);

            head.clear();
            body.clear();
            body.push_back(*new lit_type(5, POSITIVE));
            head.push_back(*new lit_type(7, POSITIVE));
            dependencyGraphStats->addRuleDependencyGraph(body, head);

            CPPUNIT_ASSERT_EQUAL_MESSAGE("Edge Count", (uint32_t) 3, dependencyGraphStats->edgecount);
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Node Count", (uint32_t) 8, dependencyGraphStats->nodecount);

            head.clear();
            body.clear();
            body.push_back(*new lit_type(2, POSITIVE));
            body.push_back(*new lit_type(4, NEGATIVE));
            head.push_back(*new lit_type(3, POSITIVE));
            dependencyGraphStats->addRuleDependencyGraph(body, head);

            CPPUNIT_ASSERT_EQUAL_MESSAGE("Edge Count", (uint32_t) 5, dependencyGraphStats->edgecount);
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Node Count", (uint32_t) 8, dependencyGraphStats->nodecount);
        }
    };

    CPPUNIT_TEST_SUITE_REGISTRATION(DependencyTests);
};