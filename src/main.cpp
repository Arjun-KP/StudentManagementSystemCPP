#include "Student.h"
#include "StudentManager.h"

#include <cmath>
#include <iomanip>
#include <iostream>
#include <limits>
#include <optional>
#include <sstream>
#include <string>

namespace
{
template <typename Number>
std::optional<Number> readNumber(const std::string& prompt, Number minimum, Number maximum)
{
    std::string input;
    while (true)
    {
        std::cout << prompt;
        if (!std::getline(std::cin, input))
        {
            return std::nullopt;
        }

        std::istringstream parser(input);
        Number value{};
        if (parser >> value)
        {
            parser >> std::ws;
            if (parser.eof() && std::isfinite(value) && value >= minimum && value <= maximum)
            {
                return value;
            }
        }
        std::cout << "Invalid input. Enter a number from " << minimum << " to " << maximum << ".\n";
    }
}

void printStudent(const Student& student)
{
    std::cout << "ID: " << student.getId() << " | Name: " << student.getName()
              << " | Marks: " << student.getMarks() << '\n';
}

bool addStudentFromInput(StudentManager& manager)
{
    const auto studentId = readNumber<int>("Student ID: ", 1, std::numeric_limits<int>::max());
    if (!studentId)
    {
        return false;
    }
    if (manager.findStudentById(*studentId))
    {
        std::cout << "A student with that ID already exists.\n";
        return true;
    }

    std::string studentName;
    while (true)
    {
        std::cout << "Student name: ";
        if (!std::getline(std::cin, studentName))
        {
            return false;
        }
        const auto firstCharacter = studentName.find_first_not_of(" \t\r\n\f\v");
        if (firstCharacter != std::string::npos)
        {
            const auto lastCharacter = studentName.find_last_not_of(" \t\r\n\f\v");
            studentName = studentName.substr(firstCharacter, lastCharacter - firstCharacter + 1);
            break;
        }
        std::cout << "Name must not be empty.\n";
    }

    const auto marks = readNumber<double>("Marks (0-100): ", 0.0, 100.0);
    if (!marks)
    {
        return false;
    }

    if (manager.addStudent(Student(*studentId, studentName, *marks)))
    {
        std::cout << "Student added.\n";
    }
    else
    {
        std::cout << "A student with that ID already exists.\n";
    }
    return true;
}
}

int main()
{
    StudentManager manager;
    std::cout << std::fixed << std::setprecision(2);

    while (true)
    {
        std::cout << "\nStudent Management System\n"
                  << "1. Add student\n"
                  << "2. Display students\n"
                  << "3. Find student by ID\n"
                  << "4. Calculate average marks\n"
                  << "5. Find top student\n"
                  << "0. Exit\n";

        const auto choice = readNumber<int>("Choice: ", 0, 5);
        if (!choice || *choice == 0)
        {
            break;
        }

        switch (*choice)
        {
        case 1:
            if (!addStudentFromInput(manager))
            {
                std::cout << "\nInput ended. Goodbye!\n";
                return 0;
            }
            break;
        case 2:
            manager.displayStudents();
            break;
        case 3:
        {
            const auto studentId = readNumber<int>("Student ID: ", 1, std::numeric_limits<int>::max());
            if (!studentId)
            {
                std::cout << "\nInput ended. Goodbye!\n";
                return 0;
            }
            const auto student = manager.findStudentById(*studentId);
            if (student)
            {
                printStudent(*student);
            }
            else
            {
                std::cout << "Student not found.\n";
            }
            break;
        }
        case 4:
        {
            const auto average = manager.calculateAverageMarks();
            if (average)
            {
                std::cout << "Average marks: " << *average << '\n';
            }
            else
            {
                std::cout << "No students exist.\n";
            }
            break;
        }
        case 5:
        {
            const auto topStudent = manager.findTopStudent();
            if (topStudent)
            {
                std::cout << "Top student: ";
                printStudent(*topStudent);
            }
            else
            {
                std::cout << "No students exist.\n";
            }
            break;
        }
        }
    }

    std::cout << "\nGoodbye!\n";
    return 0;
}