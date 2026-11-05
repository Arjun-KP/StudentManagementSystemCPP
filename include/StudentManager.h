#ifndef STUDENT_MANAGER_H
#define STUDENT_MANAGER_H

#include "Student.h"

#include <optional>
#include <vector>

class IStudentDisplay;

class StudentManager
{
public:
    bool addStudent(const Student& student);
    void displayStudents() const;
    void displayStudents(IStudentDisplay& display) const;
    std::optional<Student> findStudentById(int studentId) const;
    std::optional<double> calculateAverageMarks() const;
    std::optional<Student> findTopStudent() const;

private:
    std::vector<Student> students_;
};

#endif