#include <exst/exst_types.h>
#include "test.h"

namespace exst
{
    class ExstTypesTests : public AbstractExstTest
    {
    CPPUNIT_TEST_SUITE(ExstTypesTests);
            CPPUNIT_TEST(testCopyGraph);
            CPPUNIT_TEST(testEdgeCount1);
            CPPUNIT_TEST(testEdgeCount2);
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

        void testCopyGraph()
        {
            MyGraph graph, graphCopy;
            std::unordered_map<uint32_t, EdgeType> node1;
            std::unordered_map<uint32_t, EdgeType> node2;
            node1[2] = exst::HEAD;
            node2[1] = exst::BODY;
            graph[1] = node1;
            graph[2] = node2;
            graphCopy = copyMyGraph(graph);
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Assert size of copied graph is equal to original", graph.size(),
                                         graphCopy.size());
        }

        void testEdgeCount1()
        {
            MyGraph graph;
            std::unordered_map<uint32_t, EdgeType> node1;
            std::unordered_map<uint32_t, EdgeType> node2;
            node1[2] = exst::HEAD;
            node2[1] = exst::BODY;
            graph[1] = node1;
            graph[2] = node2;
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Assert edge count",
                                         edgeCount(graph), (uint32_t) 1);
        }

        void testEdgeCount2()
        {
            MyGraph graph;
            std::unordered_map<uint32_t, EdgeType> node1;
            std::unordered_map<uint32_t, EdgeType> node2;
            std::unordered_map<uint32_t, EdgeType> node3;
            std::unordered_map<uint32_t, EdgeType> node4;
            std::unordered_map<uint32_t, EdgeType> node5;
            std::unordered_map<uint32_t, EdgeType> node6;

            node1[2] = exst::HEAD;
            node1[3] = exst::HEAD;

            node2[1] = exst::HEAD;
            node2[3] = exst::HEAD;

            node3[2] = exst::HEAD;
            node3[4] = exst::HEAD;

            node4[3] = exst::HEAD;

            node5[6] = exst::HEAD;

            node6[4] = exst::HEAD;

            graph[1] = node1;
            graph[2] = node2;
            graph[3] = node3;
            graph[4] = node4;
            graph[5] = node5;
            graph[6] = node6;
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Assert edge count", (uint32_t) 6,
                                         edgeCount(graph));
        }
    };

    CPPUNIT_TEST_SUITE_REGISTRATION(ExstTypesTests);
};