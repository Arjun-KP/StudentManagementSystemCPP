# Student Management System

A C++17 console application for adding, listing, and finding student records,
calculating average marks, and finding the highest-scoring student.

## Build and Run

From this project folder in PowerShell, with GCC available on `PATH`:

```powershell
g++ -std=c++17 -Wall -Wextra -Wpedantic -I include src/main.cpp src/Student.cpp src/StudentManager.cpp -o StudentManagementSystem.exe
.\StudentManagementSystem.exe
```

With MSVC, use a Developer PowerShell terminal:

```powershell
cl /nologo /std:c++17 /EHsc /W4 /I include src/main.cpp src/Student.cpp src/StudentManager.cpp /Fe:StudentManagementSystem.exe
.\StudentManagementSystem.exe
```

## Behavior

- Student IDs must be positive integers and unique.
- Names must contain non-whitespace characters; names with spaces are supported.
- Marks must be finite numbers from 0 to 100 inclusive.
- Invalid input is rejected; end-of-file exits cleanly.
- Empty collections and missing student IDs produce messages rather than invalid accesses.
- If top marks are tied, the first added student with those marks is returned.
- Records are stored in memory only and are not saved after exit.

## Verification

After changes, compile and run the complete application. Check empty collections,
adding and finding records, duplicate IDs, averages, tied top marks, invalid input,
boundary marks (0 and 100), and end-of-file handling.
