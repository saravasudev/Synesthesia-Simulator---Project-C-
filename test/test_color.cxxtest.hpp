#include <cxxtest/TestSuite.h>
#include "color.hpp"

class TestColor : public CxxTest::TestSuite {
public:
    void testDefaultConstructor() {
        Color c;
        TS_ASSERT_EQUALS(c.getR(), 0);
        TS_ASSERT_EQUALS(c.getG(), 0);
        TS_ASSERT_EQUALS(c.getB(), 0);
        TS_ASSERT_EQUALS(c.getA(), 255);
    }

    void testAddClamps() {
        Color a(200, 200, 200);
        Color b(100, 100, 100);
        Color sum = a + b;
        TS_ASSERT_EQUALS(sum.getR(), 255);
        TS_ASSERT_EQUALS(sum.getG(), 255);
        TS_ASSERT_EQUALS(sum.getB(), 255);
    }

    void testSubtractClamps() {
        Color a(50, 50, 50);
        Color b(100, 100, 100);
        Color diff = a - b;
        TS_ASSERT_EQUALS(diff.getR(), 0);
        TS_ASSERT_EQUALS(diff.getG(), 0);
        TS_ASSERT_EQUALS(diff.getB(), 0);
    }

    void testEquality() {
        Color a(10, 20, 30);
        Color b(10, 20, 30);
        Color c(10, 20, 31);
        TS_ASSERT_EQUALS(a, b);
        TS_ASSERT_DIFFERS(a, c);
    }
};