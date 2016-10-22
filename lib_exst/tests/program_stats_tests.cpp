#include "test.h"

namespace exst
{
    class ExstTests : public AbstractExstTest
    {
    CPPUNIT_TEST_SUITE(ExstTests);
            CPPUNIT_TEST(testOneConstraint);
            CPPUNIT_TEST(testOneConstraintAndOneFact);
            CPPUNIT_TEST(testMultipleConstraints);
            CPPUNIT_TEST(testMultipleFacts);
            CPPUNIT_TEST(testMultipleConstraintsAndNonConstraints);
            CPPUNIT_TEST(testMultipleConstraintsAndNonConstraintsWithHelpers);
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

        void testOneConstraint()
        {
            std::list<lit_type> bodies;
            bodies.push_back(*new lit_type(2, NEGATIVE));
            bodies.push_back(*new lit_type(3, POSITIVE));
            std::list<lit_type> heads;
            heads.push_back(*new lit_type(4, POSITIVE));
            statsCalculator->addRule(bodies, heads);

            statsCalculator->addId(2, 2);
            statsCalculator->addId(3, 3);
            statsCalculator->addId(4, 4);

            std::unordered_map<uint32_t, const char *> table;
            table[1] = "";
            table[2] = "";
            table[3] = "";
            table[4] = "";

            statsCalculator->setSymbolTable(table);

            statsCalculator->calculateStats();

            CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of Non Horn Clauses", (uint32_t) 1,
                                         statsCalculator->generalStatistics.numNonHornClauses);
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of Non Dual Horn Clauses", (uint32_t) 0,
                                         statsCalculator->generalStatistics.numNonDualHornClauses);
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Max Clause Size", (uint32_t) 3,
                                         statsCalculator->generalStatistics.maxClauseSize);
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Max Clause Size Positive", (uint32_t) 2,
                                         statsCalculator->generalStatistics.maxClauseSizePositive);
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Max Clause Size Negative", (uint32_t) 1,
                                         statsCalculator->generalStatistics.maxClauseSizeNegative);
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Atom Occurences", (std::size_t) 3,
                                         statsCalculator->generalStatistics.atomOccurences.size());
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Atom Occurences Positive", (std::size_t) 2,
                                         statsCalculator->generalStatistics.atomOccurencesPositive.size());
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Atom Occurences Negative", (std::size_t) 1,
                                         statsCalculator->generalStatistics.atomOccurencesNegative.size());
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of Negative Variables", (std::size_t) 1,
                                         statsCalculator->generalStatistics.variableNegative.size());
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of Positive Variables", (std::size_t) 2,
                                         statsCalculator->generalStatistics.variablePositive.size());
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Negative Variables Without Helpers", (std::size_t) 1,
                                         statsCalculator->generalStatistics.variableNegativeWithoutHelper.size());
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Positive Variables Without Helpers", (std::size_t) 2,
                                         statsCalculator->generalStatistics.variablePositiveWithoutHelper.size());
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Max Positive Rule Size Constraint", (uint32_t) 2,
                                         statsCalculator->generalStatistics.maxPositiveRuleSizeConstraint);
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Max Positive Rule Size Non Constraint", (uint32_t) 0,
                                         statsCalculator->generalStatistics.maxPositiveRuleSizeNonConstraint);
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Atom Occurences Constraint", (uint32_t) 3,
                                         statsCalculator->generalStatistics.atomOccurencesConstraint);
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Atom Occurences Non Constraint", (uint32_t) 0,
                                         statsCalculator->generalStatistics.atomOccurencesNonConstraint);
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of Constraint", (uint32_t) 1,
                                         statsCalculator->generalStatistics.numConstraints);
        }

        void testOneConstraintAndOneFact()
        {
            std::list<lit_type> body;
            std::list<lit_type> head;
            body.push_back(*new lit_type(2, NEGATIVE));
            body.push_back(*new lit_type(3, NEGATIVE));
            statsCalculator->addRule(body, head);

            head.clear();
            body.clear();
            head.push_back(*new lit_type(4, POSITIVE));
            statsCalculator->addRule(body, head);

            statsCalculator->addId(2, 2);
            statsCalculator->addId(3, 3);

            std::unordered_map<uint32_t, const char *> table;
            table[1] = "";
            table[2] = "";
            table[3] = "";
            table[4] = "";

            statsCalculator->setSymbolTable(table);

            statsCalculator->calculateStats();

            CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of Non Horn Clauses", (uint32_t) 1,
                                         statsCalculator->generalStatistics.numNonHornClauses);
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of Non Dual Horn Clauses", (uint32_t) 0,
                                         statsCalculator->generalStatistics.numNonDualHornClauses);
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Max Clause Size", (uint32_t) 2,
                                         statsCalculator->generalStatistics.maxClauseSize);
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Max Clause Size Positive", (uint32_t) 1,
                                         statsCalculator->generalStatistics.maxClauseSizePositive);
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Max Clause Size Negative", (uint32_t) 2,
                                         statsCalculator->generalStatistics.maxClauseSizeNegative);
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Atom Occurences", (std::size_t) 3,
                                         statsCalculator->generalStatistics.atomOccurences.size());
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Atom Occurences Positive", (std::size_t) 1,
                                         statsCalculator->generalStatistics.atomOccurencesPositive.size());
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Atom Occurences Negative", (std::size_t) 2,
                                         statsCalculator->generalStatistics.atomOccurencesNegative.size());
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of Negative Variables", (std::size_t) 2,
                                         statsCalculator->generalStatistics.variableNegative.size());
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of Positive Variables", (std::size_t) 0,
                                         statsCalculator->generalStatistics.variablePositive.size());
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Negative Variables Without Helpers", (std::size_t) 2,
                                         statsCalculator->generalStatistics.variableNegativeWithoutHelper.size());
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Positive Variables Without Helpers", (std::size_t) 0,
                                         statsCalculator->generalStatistics.variablePositiveWithoutHelper.size());
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Max Positive Rule Size Constraint", (uint32_t) 0,
                                         statsCalculator->generalStatistics.maxPositiveRuleSizeConstraint);
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Max Positive Rule Size Non Constraint", (uint32_t) 1,
                                         statsCalculator->generalStatistics.maxPositiveRuleSizeNonConstraint);
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Atom Occurences Constraint", (uint32_t) 2,
                                         statsCalculator->generalStatistics.atomOccurencesConstraint);
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Atom Occurences Non Constraint", (uint32_t) 1,
                                         statsCalculator->generalStatistics.atomOccurencesNonConstraint);
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of Constraint", (uint32_t) 1,
                                         statsCalculator->generalStatistics.numConstraints);
        }

        void testMultipleConstraints()
        {
            std::list<lit_type> body;
            std::list<lit_type> head;
            body.push_back(*new lit_type(2, NEGATIVE));
            body.push_back(*new lit_type(3, NEGATIVE));
            statsCalculator->addRule(body, head);

            head.clear();
            body.clear();
            body.push_back(*new lit_type(4, NEGATIVE));
            body.push_back(*new lit_type(5, POSITIVE));
            statsCalculator->addRule(body, head);

            head.clear();
            body.clear();
            body.push_back(*new lit_type(2, POSITIVE));
            body.push_back(*new lit_type(3, NEGATIVE));
            statsCalculator->addRule(body, head);

            head.clear();
            body.clear();
            body.push_back(*new lit_type(4, POSITIVE));
            body.push_back(*new lit_type(5, POSITIVE));
            statsCalculator->addRule(body, head);

            head.clear();
            body.clear();
            body.push_back(*new lit_type(2, NEGATIVE));
            body.push_back(*new lit_type(3, NEGATIVE));
            head.push_back(*new lit_type(6, POSITIVE));
            statsCalculator->addRule(body, head);

            head.clear();
            body.clear();
            body.push_back(*new lit_type(5, POSITIVE));
            body.push_back(*new lit_type(6, POSITIVE));
            head.push_back(*new lit_type(7, POSITIVE));
            statsCalculator->addRule(body, head);

            statsCalculator->addId(2, 2);
            statsCalculator->addId(3, 3);
            statsCalculator->addId(4, 4);
            statsCalculator->addId(5, 5);
            statsCalculator->addId(6, 6);
            statsCalculator->addId(7, 7);

            std::unordered_map<uint32_t, const char *> table;
            table[1] = "";
            table[2] = "";
            table[3] = "";
            table[4] = "";
            table[5] = "";
            table[6] = "";
            table[7] = "";

            statsCalculator->setSymbolTable(table);

            statsCalculator->calculateStats();

            CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of Non Horn Clauses", (uint32_t) 2,
                                         statsCalculator->generalStatistics.numNonHornClauses);
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of Non Dual Horn Clauses", (uint32_t) 2,
                                         statsCalculator->generalStatistics.numNonDualHornClauses);
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Max Clause Size", (uint32_t) 3,
                                         statsCalculator->generalStatistics.maxClauseSize);
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Max Clause Size Positive", (uint32_t) 3,
                                         statsCalculator->generalStatistics.maxClauseSizePositive);
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Max Clause Size Negative", (uint32_t) 2,
                                         statsCalculator->generalStatistics.maxClauseSizeNegative);
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Max Number Atom Occurences", (uint32_t) 3,
                                         maxValue(statsCalculator->generalStatistics.atomOccurences));
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Max Number Atom Occurences Positive", (uint32_t) 3,
                                         maxValue(statsCalculator->generalStatistics.atomOccurencesPositive));
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Max Number Atom Occurences Negative", (uint32_t) 3,
                                         maxValue(statsCalculator->generalStatistics.atomOccurencesNegative));
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of Negative Variables", (std::size_t) 3,
                                         statsCalculator->generalStatistics.variableNegative.size());
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of Positive Variables", (std::size_t) 5,
                                         statsCalculator->generalStatistics.variablePositive.size());
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Negative Variables Without Helpers", (std::size_t) 3,
                                         statsCalculator->generalStatistics.variableNegativeWithoutHelper.size());
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Positive Variables Without Helpers", (std::size_t) 5,
                                         statsCalculator->generalStatistics.variablePositiveWithoutHelper.size());
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Max Positive Rule Size Constraint", (uint32_t) 3,
                                         statsCalculator->generalStatistics.maxPositiveRuleSizeConstraint);
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Max Positive Rule Size Non Constraint", (uint32_t) 0,
                                         statsCalculator->generalStatistics.maxPositiveRuleSizeNonConstraint);
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Atom Occurences Constraint", (uint32_t) 14,
                                         statsCalculator->generalStatistics.atomOccurencesConstraint);
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Atom Occurences Non Constraint", (uint32_t) 0,
                                         statsCalculator->generalStatistics.atomOccurencesNonConstraint);
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of Constraint", (uint32_t) 6,
                                         statsCalculator->generalStatistics.numConstraints);
        }

        void testMultipleFacts()
        {
            std::list<lit_type> body;
            std::list<lit_type> head;
            head.push_back(*new lit_type(3, NEGATIVE));
            statsCalculator->addRule(body, head);

            head.clear();
            body.clear();
            head.push_back(*new lit_type(5, POSITIVE));
            statsCalculator->addRule(body, head);

            head.clear();
            body.clear();
            head.push_back(*new lit_type(3, NEGATIVE));
            statsCalculator->addRule(body, head);

            std::unordered_map<uint32_t, const char *> table;
            table[1] = "";
            table[2] = "";
            table[3] = "";
            table[4] = "";
            table[5] = "";

            statsCalculator->setSymbolTable(table);

            statsCalculator->calculateStats();

            CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of Non Horn Clauses", (uint32_t) 0,
                                         statsCalculator->generalStatistics.numNonHornClauses);
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of Non Dual Horn Clauses", (uint32_t) 0,
                                         statsCalculator->generalStatistics.numNonDualHornClauses);
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Max Clause Size", (uint32_t) 1,
                                         statsCalculator->generalStatistics.maxClauseSize);
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Max Clause Size Positive", (uint32_t) 1,
                                         statsCalculator->generalStatistics.maxClauseSizePositive);
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Max Clause Size Negative", (uint32_t) 1,
                                         statsCalculator->generalStatistics.maxClauseSizeNegative);
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Max Number Atom Occurences", (uint32_t) 2,
                                         maxValue(statsCalculator->generalStatistics.atomOccurences));
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Max Number Atom Occurences Positive", (uint32_t) 1,
                                         maxValue(statsCalculator->generalStatistics.atomOccurencesPositive));
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Max Number Atom Occurences Negative", (uint32_t) 2,
                                         maxValue(statsCalculator->generalStatistics.atomOccurencesNegative));
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of Negative Variables", (std::size_t) 0,
                                         statsCalculator->generalStatistics.variableNegative.size());
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of Positive Variables", (std::size_t) 0,
                                         statsCalculator->generalStatistics.variablePositive.size());
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Negative Variables Without Helpers", (std::size_t) 0,
                                         statsCalculator->generalStatistics.variableNegativeWithoutHelper.size());
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Positive Variables Without Helpers", (std::size_t) 0,
                                         statsCalculator->generalStatistics.variablePositiveWithoutHelper.size());
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Max Positive Rule Size Constraint", (uint32_t) 0,
                                         statsCalculator->generalStatistics.maxPositiveRuleSizeConstraint);
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Max Positive Rule Size Non Constraint", (uint32_t) 1,
                                         statsCalculator->generalStatistics.maxPositiveRuleSizeNonConstraint);
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Atom Occurences Constraint", (uint32_t) 0,
                                         statsCalculator->generalStatistics.atomOccurencesConstraint);
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Atom Occurences Non Constraint", (uint32_t) 3,
                                         statsCalculator->generalStatistics.atomOccurencesNonConstraint);
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of Constraint", (uint32_t) 0,
                                         statsCalculator->generalStatistics.numConstraints);
        }

        void testMultipleConstraintsAndNonConstraints()
        {

            std::list<lit_type> body;
            std::list<lit_type> head;
            body.push_back(*new lit_type(2, NEGATIVE));
            body.push_back(*new lit_type(3, NEGATIVE));
            statsCalculator->addRule(body, head);

            head.clear();
            body.clear();
            body.push_back(*new lit_type(4, NEGATIVE));
            body.push_back(*new lit_type(5, POSITIVE));
            statsCalculator->addRule(body, head);

            head.clear();
            body.clear();
            body.push_back(*new lit_type(5, POSITIVE));
            body.push_back(*new lit_type(6, POSITIVE));
            head.push_back(*new lit_type(7, POSITIVE));
            statsCalculator->addRule(body, head);

            head.clear();
            body.clear();
            head.push_back(*new lit_type(8, POSITIVE));
            statsCalculator->addRule(body, head);

            head.clear();
            body.clear();
            head.push_back(*new lit_type(9, NEGATIVE));
            statsCalculator->addRule(body, head);

            statsCalculator->addId(2, 2);
            statsCalculator->addId(3, 3);
            statsCalculator->addId(4, 4);
            statsCalculator->addId(5, 5);
            statsCalculator->addId(6, 6);
            statsCalculator->addId(7, 7);

            std::unordered_map<uint32_t, const char *> table;
            table[1] = "";
            table[2] = "";
            table[3] = "";
            table[4] = "";
            table[5] = "";
            table[6] = "";
            table[7] = "";
            table[8] = "";
            table[9] = "";

            statsCalculator->setSymbolTable(table);

            statsCalculator->calculateStats();

            CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of Non Horn Clauses", (uint32_t) 1,
                                         statsCalculator->generalStatistics.numNonHornClauses);
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of Non Dual Horn Clauses", (uint32_t) 1,
                                         statsCalculator->generalStatistics.numNonDualHornClauses);
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Max Clause Size", (uint32_t) 3,
                                         statsCalculator->generalStatistics.maxClauseSize);
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Max Clause Size Positive", (uint32_t) 3,
                                         statsCalculator->generalStatistics.maxClauseSizePositive);
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Max Clause Size Negative", (uint32_t) 2,
                                         statsCalculator->generalStatistics.maxClauseSizeNegative);
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Max Number Atom Occurences", (uint32_t) 2,
                                         maxValue(statsCalculator->generalStatistics.atomOccurences));
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Max Number Atom Occurences Positive", (uint32_t) 2,
                                         maxValue(statsCalculator->generalStatistics.atomOccurencesPositive));
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Max Number Atom Occurences Negative", (uint32_t) 1,
                                         maxValue(statsCalculator->generalStatistics.atomOccurencesNegative));
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of Negative Variables", (std::size_t) 3,
                                         statsCalculator->generalStatistics.variableNegative.size());
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of Positive Variables", (std::size_t) 3,
                                         statsCalculator->generalStatistics.variablePositive.size());
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Negative Variables Without Helpers", (std::size_t) 3,
                                         statsCalculator->generalStatistics.variableNegativeWithoutHelper.size());
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Positive Variables Without Helpers", (std::size_t) 3,
                                         statsCalculator->generalStatistics.variablePositiveWithoutHelper.size());
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Max Positive Rule Size Constraint", (uint32_t) 3,
                                         statsCalculator->generalStatistics.maxPositiveRuleSizeConstraint);
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Max Positive Rule Size Non Constraint", (uint32_t) 1,
                                         statsCalculator->generalStatistics.maxPositiveRuleSizeNonConstraint);
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Atom Occurences Constraint", (uint32_t) 7,
                                         statsCalculator->generalStatistics.atomOccurencesConstraint);
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Atom Occurences Non Constraint", (uint32_t) 2,
                                         statsCalculator->generalStatistics.atomOccurencesNonConstraint);
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of Constraint", (uint32_t) 3,
                                         statsCalculator->generalStatistics.numConstraints);
        }

        void testMultipleConstraintsAndNonConstraintsWithHelpers()
        {

            std::list<lit_type> body;
            std::list<lit_type> head;
            body.push_back(*new lit_type(2, NEGATIVE));
            body.push_back(*new lit_type(3, NEGATIVE));
            statsCalculator->addRule(body, head);

            head.clear();
            body.clear();
            body.push_back(*new lit_type(4, NEGATIVE));
            body.push_back(*new lit_type(5, POSITIVE));
            statsCalculator->addRule(body, head);

            head.clear();
            body.clear();
            body.push_back(*new lit_type(5, POSITIVE));
            body.push_back(*new lit_type(6, POSITIVE));
            head.push_back(*new lit_type(7, POSITIVE));
            statsCalculator->addRule(body, head);

            head.clear();
            body.clear();
            head.push_back(*new lit_type(8, POSITIVE));
            statsCalculator->addRule(body, head);

            head.clear();
            body.clear();
            head.push_back(*new lit_type(9, NEGATIVE));
            statsCalculator->addRule(body, head);

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
            statsCalculator->calculateStats();

            CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of Non Horn Clauses", (uint32_t) 1,
                                         statsCalculator->generalStatistics.numNonHornClauses);
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of Non Dual Horn Clauses", (uint32_t) 1,
                                         statsCalculator->generalStatistics.numNonDualHornClauses);
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Max Clause Size", (uint32_t) 3,
                                         statsCalculator->generalStatistics.maxClauseSize);
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Max Clause Size Positive", (uint32_t) 3,
                                         statsCalculator->generalStatistics.maxClauseSizePositive);
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Max Clause Size Negative", (uint32_t) 2,
                                         statsCalculator->generalStatistics.maxClauseSizeNegative);
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Max Number Atom Occurences", (uint32_t) 2,
                                         maxValue(statsCalculator->generalStatistics.atomOccurences));
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Max Number Atom Occurences Positive", (uint32_t) 2,
                                         maxValue(statsCalculator->generalStatistics.atomOccurencesPositive));
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Max Number Atom Occurences Negative", (uint32_t) 1,
                                         maxValue(statsCalculator->generalStatistics.atomOccurencesNegative));
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of Negative Variables", (std::size_t) 3,
                                         statsCalculator->generalStatistics.variableNegative.size());
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of Positive Variables", (std::size_t) 3,
                                         statsCalculator->generalStatistics.variablePositive.size());
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Negative Variables Without Helpers", (std::size_t) 1,
                                         statsCalculator->generalStatistics.variableNegativeWithoutHelper.size());
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Positive Variables Without Helpers", (std::size_t) 3,
                                         statsCalculator->generalStatistics.variablePositiveWithoutHelper.size());
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Max Positive Rule Size Constraint", (uint32_t) 3,
                                         statsCalculator->generalStatistics.maxPositiveRuleSizeConstraint);
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Max Positive Rule Size Non Constraint", (uint32_t) 1,
                                         statsCalculator->generalStatistics.maxPositiveRuleSizeNonConstraint);
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Atom Occurences Constraint", (uint32_t) 7,
                                         statsCalculator->generalStatistics.atomOccurencesConstraint);
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Atom Occurences Non Constraint", (uint32_t) 2,
                                         statsCalculator->generalStatistics.atomOccurencesNonConstraint);
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of Constraint", (uint32_t) 3,
                                         statsCalculator->generalStatistics.numConstraints);
        }
    };

    CPPUNIT_TEST_SUITE_REGISTRATION(ExstTests);
};