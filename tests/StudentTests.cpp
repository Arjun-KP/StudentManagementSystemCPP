#include "Student.h"

#include <gtest/gtest.h>

#include <cmath>
#include <limits>
#include <stdexcept>
#include <string>

// Valid input is preserved by all public getters.
TEST(StudentTests, StoresValidValuesForPublicGetters)
{
    const Student student(42, "Ada Lovelace", 91.5);

    ASSERT_EQ(student.getId(), 42);
    EXPECT_EQ(student.getName(), "Ada Lovelace");
    EXPECT_DOUBLE_EQ(student.getMarks(), 91.5);
    EXPECT_TRUE(std::isfinite(student.getMarks()));
    EXPECT_FALSE(student.getName().empty());
}

// The smallest positive ID and inclusive marks limits are valid.
TEST(StudentTests, AcceptsBoundaryIdsAndMarks)
{
    const Student minimumStudent(1, "Minimum", 0.0);
    const Student maximumStudent(std::numeric_limits<int>::max(), "Maximum", 100.0);

    EXPECT_EQ(minimumStudent.getId(), 1);
    EXPECT_DOUBLE_EQ(minimumStudent.getMarks(), 0.0);
    EXPECT_EQ(maximumStudent.getId(), std::numeric_limits<int>::max());
    EXPECT_DOUBLE_EQ(maximumStudent.getMarks(), 100.0);
}

// Zero and negative IDs violate constructor validation.
TEST(StudentTests, RejectsNonPositiveIds)
{
    EXPECT_THROW(Student(0, "Student", 50.0), std::invalid_argument);
    EXPECT_THROW(Student(-1, "Student", 50.0), std::invalid_argument);
}

// Names without a non-whitespace character are rejected.
TEST(StudentTests, RejectsEmptyOrWhitespaceOnlyNames)
{
    EXPECT_THROW(Student(1, "", 50.0), std::invalid_argument);
    EXPECT_THROW(Student(1, " \t\r\n\f\v", 50.0), std::invalid_argument);
}

// Out-of-range and non-finite marks violate constructor validation.
TEST(StudentTests, RejectsInvalidMarks)
{
    EXPECT_THROW(Student(1, "Student", -0.01), std::invalid_argument);
    EXPECT_THROW(Student(1, "Student", 100.01), std::invalid_argument);
    EXPECT_THROW(Student(1, "Student", std::numeric_limits<double>::infinity()), std::invalid_argument);
    EXPECT_THROW(Student(1, "Student", -std::numeric_limits<double>::infinity()), std::invalid_argument);
    EXPECT_THROW(Student(1, "Student", std::numeric_limits<double>::quiet_NaN()), std::invalid_argument);
}

// A nonblank name is stored without trimming its surrounding whitespace.
TEST(StudentTests, PreservesWhitespaceAroundNonBlankName)
{
    const Student student(5, "  Grace Hopper  ", 100.0);

    EXPECT_EQ(student.getName(), "  Grace Hopper  ");
}