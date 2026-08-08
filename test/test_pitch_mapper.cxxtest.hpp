#include <cxxtest/TestSuite.h>
#include "pitch_mapper.hpp"
#include "synesthesia_exceptions.hpp"

class TestPitchMapper : public CxxTest::TestSuite {
public:
    void testLowFrequencyIsRed() {
        Color c = mapFrequency<double>(0.0, 0.0, 1000.0);
        TS_ASSERT_EQUALS(c.getR(), 255);
        TS_ASSERT_EQUALS(c.getB(), 0);
    }

    void testHighFrequencyIsBlue() {
        Color c = mapFrequency<double>(1000.0, 0.0, 1000.0);
        TS_ASSERT_EQUALS(c.getR(), 0);
        TS_ASSERT_EQUALS(c.getB(), 255);
    }

    void testOutOfRangeThrows() {
        TS_ASSERT_THROWS(mapFrequency<double>(-1.0, 0.0, 1000.0), const InvalidFrequencyException&);
        TS_ASSERT_THROWS(mapFrequency<double>(2000.0, 0.0, 1000.0), const InvalidFrequencyException&);
    }
};