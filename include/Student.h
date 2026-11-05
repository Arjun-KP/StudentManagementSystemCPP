#ifndef STUDENT_H
#define STUDENT_H

#include <string>

class Student
{
public:
    Student(int studentId, std::string studentName, double marks);

    int getId() const;
    const std::string& getName() const;
    double getMarks() const;

private:
    int studentId_;
    std::string studentName_;
    double marks_;
};

#endif