#include "IStudentDisplay.h"
#include "StudentManager.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <stdexcept>

namespace
{
class MockStudentDisplay : public IStudentDisplay
{
public:
    MOCK_METHOD(void, showStudent, (const Student& student), (override));
    MOCK_METHOD(void, showEmpty, (), (override));
};
}

TEST(StudentManagerMockTests, DisplaysSuccessfullyAddedStudentExactlyOnce)
{
    StudentManager manager;
    ASSERT_TRUE(manager.addStudent(Student(7, "Grace Hopper", 95.0)));
    testing::StrictMock<MockStudentDisplay> display;

    EXPECT_CALL(display, showEmpty()).Times(0);
    EXPECT_CALL(display, showStudent(testing::AllOf(
        testing::Property(&Student::getId, 7),
        testing::Property(&Student::getName, "Grace Hopper"),
        testing::Property(&Student::getMarks, 95.0))))
        .Times(1)
        .WillOnce(testing::Return());

    manager.displayStudents(display);
}

TEST(StudentManagerMockTests, PropagatesDisplayExceptionWithoutDisplayingLaterStudents)
{
    StudentManager manager;
    ASSERT_TRUE(manager.addStudent(Student(7, "Grace Hopper", 95.0)));
    ASSERT_TRUE(manager.addStudent(Student(12, "Katherine Johnson", 98.0)));
    testing::StrictMock<MockStudentDisplay> display;

    EXPECT_CALL(display, showEmpty()).Times(0);
    EXPECT_CALL(display, showStudent(testing::Property(&Student::getId, 12))).Times(0);
    EXPECT_CALL(display, showStudent(testing::Property(&Student::getId, 7)))
        .Times(1)
        .WillOnce(testing::Throw(std::runtime_error("Display unavailable")));

    EXPECT_THROW(manager.displayStudents(display), std::runtime_error);

    const auto original = manager.findStudentById(7);
    const auto later = manager.findStudentById(12);
    ASSERT_TRUE(original.has_value());
    ASSERT_TRUE(later.has_value());
    EXPECT_EQ(original->getName(), "Grace Hopper");
    EXPECT_EQ(later->getName(), "Katherine Johnson");
}

TEST(StudentManagerMockTests, RejectsDuplicateIdAndDisplaysOnlyOriginalRecord)
{
    StudentManager manager;
    ASSERT_TRUE(manager.addStudent(Student(7, "Original", 80.0)));
    EXPECT_FALSE(manager.addStudent(Student(7, "Duplicate", 90.0)));
    testing::StrictMock<MockStudentDisplay> display;

    EXPECT_CALL(display, showEmpty()).Times(0);
    EXPECT_CALL(display, showStudent(testing::Property(&Student::getName, "Duplicate")))
        .Times(0);
    EXPECT_CALL(display, showStudent(testing::AllOf(
        testing::Property(&Student::getId, 7),
        testing::Property(&Student::getName, "Original"),
        testing::Property(&Student::getMarks, 80.0))))
        .Times(1)
        .WillOnce(testing::Return());

    manager.displayStudents(display);
}

TEST(StudentManagerMockTests, EmptyCollectionDoesNotCallShowStudent)
{
    const StudentManager manager;
    testing::StrictMock<MockStudentDisplay> display;

    EXPECT_CALL(display, showStudent(testing::_)).Times(0);
    EXPECT_CALL(display, showEmpty())
        .Times(1)
        .WillOnce(testing::Return());

    manager.displayStudents(display);
}

TEST(StudentManagerMockTests, CallsDisplayOncePerStudentOnEveryDisplayRequest)
{
    StudentManager manager;
    ASSERT_TRUE(manager.addStudent(Student(7, "Grace Hopper", 95.0)));
    ASSERT_TRUE(manager.addStudent(Student(12, "Katherine Johnson", 98.0)));
    testing::StrictMock<MockStudentDisplay> display;
    testing::InSequence sequence;

    EXPECT_CALL(display, showEmpty()).Times(0);
    EXPECT_CALL(display, showStudent(testing::Property(&Student::getId, 7))).Times(1);
    EXPECT_CALL(display, showStudent(testing::Property(&Student::getId, 12))).Times(1);
    EXPECT_CALL(display, showStudent(testing::Property(&Student::getId, 7))).Times(1);
    EXPECT_CALL(display, showStudent(testing::Property(&Student::getId, 12))).Times(1);

    manager.displayStudents(display);
    manager.displayStudents(display);
}