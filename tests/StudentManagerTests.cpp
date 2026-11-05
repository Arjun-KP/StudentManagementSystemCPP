#include "StudentManager.h"
#include "IStudentDisplay.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

namespace
{
class MockStudentDisplay : public IStudentDisplay
{
public:
    MOCK_METHOD(void, showStudent, (const Student& student), (override));
    MOCK_METHOD(void, showEmpty, (), (override));
};

class CoutCapture
{
public:
    CoutCapture()
        : originalBuffer_(std::cout.rdbuf(capturedOutput_.rdbuf()))
    {
    }

    ~CoutCapture()
    {
        std::cout.rdbuf(originalBuffer_);
    }

    std::string str() const
    {
        return capturedOutput_.str();
    }

private:
    std::ostringstream capturedOutput_;
    std::streambuf* originalBuffer_;
};
}

TEST(StudentManagerTests, AddsValidStudent)
{
    StudentManager manager;
    const Student student(7, "Grace Hopper", 95.0);

    EXPECT_TRUE(manager.addStudent(student));

    const auto found = manager.findStudentById(7);
    ASSERT_TRUE(found.has_value());
    EXPECT_EQ(found->getName(), "Grace Hopper");
    EXPECT_DOUBLE_EQ(found->getMarks(), 95.0);
}

TEST(StudentManagerTests, RejectsDuplicateStudentWithoutReplacingOriginal)
{
    StudentManager manager;
    const Student original(7, "Original", 80.0);
    const Student duplicate(7, "Duplicate", 90.0);

    ASSERT_TRUE(manager.addStudent(original));
    EXPECT_FALSE(manager.addStudent(duplicate));

    const auto found = manager.findStudentById(7);
    ASSERT_TRUE(found.has_value());
    EXPECT_EQ(found->getName(), "Original");
    EXPECT_DOUBLE_EQ(found->getMarks(), 80.0);
}

TEST(StudentManagerTests, FindsExistingStudentById)
{
    StudentManager manager;
    ASSERT_TRUE(manager.addStudent(Student(12, "Katherine Johnson", 98.0)));

    const auto found = manager.findStudentById(12);

    ASSERT_TRUE(found.has_value());
    EXPECT_EQ(found->getId(), 12);
    EXPECT_EQ(found->getName(), "Katherine Johnson");
}

TEST(StudentManagerTests, ReturnsNoResultForMissingStudentId)
{
    StudentManager manager;
    ASSERT_TRUE(manager.addStudent(Student(12, "Katherine Johnson", 98.0)));

    EXPECT_FALSE(manager.findStudentById(99).has_value());
}

TEST(StudentManagerTests, ReturnsNoResultForNonPositiveSearchIds)
{
    const StudentManager manager;

    EXPECT_FALSE(manager.findStudentById(0).has_value());
    EXPECT_FALSE(manager.findStudentById(-1).has_value());
}

TEST(StudentManagerTests, RejectsInvalidStudentBeforeManagerInsertion)
{
    StudentManager manager;

    EXPECT_THROW(
        {
            const Student invalidStudent(0, "Invalid", 50.0);
            manager.addStudent(invalidStudent);
        },
        std::invalid_argument);
    EXPECT_FALSE(manager.findStudentById(1).has_value());
}

TEST(StudentManagerTests, DisplaysMessageForEmptyCollection)
{
    const StudentManager manager;
    CoutCapture output;

    manager.displayStudents();

    EXPECT_EQ(output.str(), "No students exist.\n");
}

TEST(StudentManagerTests, DisplaysAddedStudent)
{
    StudentManager manager;
    ASSERT_TRUE(manager.addStudent(Student(7, "Grace Hopper", 95.0)));
    CoutCapture output;

    manager.displayStudents();

    EXPECT_EQ(output.str(), "ID: 7 | Name: Grace Hopper | Marks: 95\n");
}

TEST(StudentManagerTests, NotifiesInjectedDisplayWhenEmpty)
{
    const StudentManager manager;
    testing::StrictMock<MockStudentDisplay> display;

    EXPECT_CALL(display, showEmpty()).Times(1);

    manager.displayStudents(display);
}

TEST(StudentManagerTests, SendsStudentsToInjectedDisplayInInsertionOrder)
{
    StudentManager manager;
    ASSERT_TRUE(manager.addStudent(Student(7, "Grace Hopper", 95.0)));
    ASSERT_TRUE(manager.addStudent(Student(12, "Katherine Johnson", 98.0)));
    testing::StrictMock<MockStudentDisplay> display;
    testing::InSequence sequence;

    EXPECT_CALL(display, showStudent(testing::AllOf(
        testing::Property(&Student::getId, 7),
        testing::Property(&Student::getName, "Grace Hopper"),
        testing::Property(&Student::getMarks, 95.0)))).Times(1);
    EXPECT_CALL(display, showStudent(testing::AllOf(
        testing::Property(&Student::getId, 12),
        testing::Property(&Student::getName, "Katherine Johnson"),
        testing::Property(&Student::getMarks, 98.0)))).Times(1);

    manager.displayStudents(display);
}

TEST(StudentManagerTests, ReturnsNoAverageForEmptyCollection)
{
    const StudentManager manager;

    EXPECT_FALSE(manager.calculateAverageMarks().has_value());
}

TEST(StudentManagerTests, CalculatesAverageMarks)
{
    StudentManager manager;
    ASSERT_TRUE(manager.addStudent(Student(1, "First", 60.0)));
    ASSERT_TRUE(manager.addStudent(Student(2, "Second", 80.0)));
    ASSERT_TRUE(manager.addStudent(Student(3, "Third", 100.0)));

    const auto average = manager.calculateAverageMarks();

    ASSERT_TRUE(average.has_value());
    EXPECT_DOUBLE_EQ(*average, 80.0);
}

TEST(StudentManagerTests, ReturnsNoTopStudentForEmptyCollection)
{
    const StudentManager manager;

    EXPECT_FALSE(manager.findTopStudent().has_value());
}

TEST(StudentManagerTests, FindsTopStudentAndKeepsFirstOnTie)
{
    StudentManager manager;
    ASSERT_TRUE(manager.addStudent(Student(1, "First Top", 95.0)));
    ASSERT_TRUE(manager.addStudent(Student(2, "Second Top", 95.0)));
    ASSERT_TRUE(manager.addStudent(Student(3, "Lower", 80.0)));

    const auto topStudent = manager.findTopStudent();

    ASSERT_TRUE(topStudent.has_value());
    EXPECT_EQ(topStudent->getId(), 1);
    EXPECT_EQ(topStudent->getName(), "First Top");
}
