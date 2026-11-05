#include "StudentManager.h"
#include "IStudentDisplay.h"

#include <algorithm>
#include <iostream>

namespace
{
class ConsoleStudentDisplay : public IStudentDisplay
{
public:
    void showStudent(const Student& student) override
    {
        std::cout << "ID: " << student.getId() << " | Name: " << student.getName()
                  << " | Marks: " << student.getMarks() << '\n';
    }

    void showEmpty() override
    {
        std::cout << "No students exist.\n";
    }
};
}

bool StudentManager::addStudent(const Student& student)
{
    const auto existing = std::find_if(students_.begin(), students_.end(),
        [&student](const Student& current) { return current.getId() == student.getId(); });
    if (existing != students_.end())
    {
        return false;
    }

    students_.push_back(student);
    return true;
}

void StudentManager::displayStudents() const
{
    ConsoleStudentDisplay display;
    displayStudents(display);
}

void StudentManager::displayStudents(IStudentDisplay& display) const
{
    if (students_.empty())
    {
        display.showEmpty();
        return;
    }

    for (const Student& student : students_)
    {
        display.showStudent(student);
    }
}

std::optional<Student> StudentManager::findStudentById(int studentId) const
{
    const auto found = std::find_if(students_.begin(), students_.end(),
        [studentId](const Student& student) { return student.getId() == studentId; });
    if (found == students_.end())
    {
        return std::nullopt;
    }
    return *found;
}

std::optional<double> StudentManager::calculateAverageMarks() const
{
    if (students_.empty())
    {
        return std::nullopt;
    }

    double totalMarks = 0.0;
    for (const Student& student : students_)
    {
        totalMarks += student.getMarks();
    }
    return totalMarks / static_cast<double>(students_.size());
}

std::optional<Student> StudentManager::findTopStudent() const
{
    if (students_.empty())
    {
        return std::nullopt;
    }

    const auto topStudent = std::max_element(students_.begin(), students_.end(),
        [](const Student& first, const Student& second) {
            return first.getMarks() < second.getMarks();
        });
    return *topStudent;
}