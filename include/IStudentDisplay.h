#ifndef I_STUDENT_DISPLAY_H
#define I_STUDENT_DISPLAY_H

class Student;

class IStudentDisplay
{
public:
    virtual ~IStudentDisplay() = default;
    virtual void showStudent(const Student& student) = 0;
    virtual void showEmpty() = 0;
};

#endif