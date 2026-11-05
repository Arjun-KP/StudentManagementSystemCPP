#include "Student.h"

#include <cmath>
#include <stdexcept>
#include <utility>

Student::Student(int studentId, std::string studentName, double marks)
    : studentId_(studentId), studentName_(std::move(studentName)), marks_(marks)
{
    if (studentId_ <= 0 || studentName_.find_first_not_of(" \t\r\n\f\v") == std::string::npos
        || !std::isfinite(marks_) || marks_ < 0.0 || marks_ > 100.0)
    {
        throw std::invalid_argument("A student requires a positive ID, a name, and marks from 0 to 100.");
    }
}

int Student::getId() const
{
    return studentId_;
}

const std::string& Student::getName() const
{
    return studentName_;
}

double Student::getMarks() const
{
    return marks_;
}