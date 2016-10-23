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
            CPPUNIT_TEST(testDIMACSFormat);
            CPPUNIT_TEST(testGMLFormat);
            CPPUNIT_TEST(testGrFormat);
            CPPUNIT_TEST(testNONEFormat);
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
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Assert edge count", edgeCount(graph), (uint32_t) 1);
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
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Assert edge count", (uint32_t) 6, edgeCount(graph));
        }

        void testGrFormat()
        {
            MyGraph graph = getTestGraph();
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Assert edge count", std::string("\"p tw 6 9\"\n"
                                                                                  ",\"1 2\"\n"
                                                                                  ",\"1 3\"\n"
                                                                                  ",\"1 6\"\n"
                                                                                  ",\"2 3\"\n"
                                                                                  ",\"2 5\"\n"
                                                                                  ",\"3 4\"\n"
                                                                                  ",\"3 5\"\n"
                                                                                  ",\"4 6\"\n"
                                                                                  ",\"5 6\""), getFormatedGraph(GR,graph));
        }

        void testGMLFormat()
        {
            MyGraph graph = getTestGraph();
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Assert edge count", std::string("graph [\n"
                                                                                  "  node [\n"
                                                                                  "    id 1\n"
                                                                                  "    label \"1\"\n"
                                                                                  "  ]  node [\n"
                                                                                  "    id 2\n"
                                                                                  "    label \"2\"\n"
                                                                                  "  ]  node [\n"
                                                                                  "    id 3\n"
                                                                                  "    label \"3\"\n"
                                                                                  "  ]  node [\n"
                                                                                  "    id 4\n"
                                                                                  "    label \"4\"\n"
                                                                                  "  ]  node [\n"
                                                                                  "    id 5\n"
                                                                                  "    label \"5\"\n"
                                                                                  "  ]  node [\n"
                                                                                  "    id 6\n"
                                                                                  "    label \"6\"\n"
                                                                                  "  ]  edge [\n"
                                                                                  "    source 1\n"
                                                                                  "    target 2\n"
                                                                                  "  ]  edge [\n"
                                                                                  "    source 1\n"
                                                                                  "    target 3\n"
                                                                                  "  ]  edge [\n"
                                                                                  "    source 1\n"
                                                                                  "    target 6\n"
                                                                                  "  ]  edge [\n"
                                                                                  "    source 2\n"
                                                                                  "    target 3\n"
                                                                                  "  ]  edge [\n"
                                                                                  "    source 2\n"
                                                                                  "    target 5\n"
                                                                                  "  ]  edge [\n"
                                                                                  "    source 3\n"
                                                                                  "    target 4\n"
                                                                                  "  ]  edge [\n"
                                                                                  "    source 3\n"
                                                                                  "    target 5\n"
                                                                                  "  ]  edge [\n"
                                                                                  "    source 4\n"
                                                                                  "    target 6\n"
                                                                                  "  ]  edge [\n"
                                                                                  "    source 5\n"
                                                                                  "    target 6\n"
                                                                                  "  ]]"), getFormatedGraph(GML,graph));
        }

        void testDIMACSFormat()
        {
            MyGraph graph = getTestGraph();
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Assert edge count", std::string("\"p edge 6 9\"\n"
                                                                                  ",\"e 1 2\"\n"
                                                                                  ",\"e 1 3\"\n"
                                                                                  ",\"e 1 6\"\n"
                                                                                  ",\"e 2 3\"\n"
                                                                                  ",\"e 2 5\"\n"
                                                                                  ",\"e 3 4\"\n"
                                                                                  ",\"e 3 5\"\n"
                                                                                  ",\"e 4 6\"\n"
                                                                                  ",\"e 5 6\""), getFormatedGraph(DIMACS,graph));
        }

        void testNONEFormat()
        {
            MyGraph graph = getTestGraph();
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Assert edge count", std::string(""), getFormatedGraph(NONE,graph));
        }

        MyGraph getTestGraph(){
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
            node2[5] = exst::HEAD;

            node3[2] = exst::HEAD;
            node3[4] = exst::HEAD;

            node4[3] = exst::HEAD;

            node5[6] = exst::HEAD;
            node5[3] = exst::HEAD;

            node6[4] = exst::HEAD;
            node6[1] = exst::HEAD;

            graph[1] = node1;
            graph[2] = node2;
            graph[3] = node3;
            graph[4] = node4;
            graph[5] = node5;
            graph[6] = node6;
            return graph;
        }
    };

    CPPUNIT_TEST_SUITE_REGISTRATION(ExstTypesTests);
};