# Student Management System Debugging Checklist

## Understanding Existing Code

- [ ] What should the affected workflow do, and what actually happens?
- [ ] Which classes and functions handle input, student records, and any persistence?
- [ ] How does data move from the menu through validation to record updates?
- [ ] Can I reproduce the issue with a small, specific input sequence?
- [ ] Can I explain the root cause before proposing a minimal fix?

## Compiler and Linker Errors

- [ ] Am I compiling with C++17 and reading the first diagnostic before later errors?
- [ ] Are required headers included directly and names correctly qualified?
- [ ] Do function declarations and definitions match, including namespaces and qualifiers?
- [ ] Are all required source files and libraries included in the link command?
- [ ] Are missing definitions or duplicate definitions causing linker failures?

## Runtime Crashes

- [ ] Which inputs and menu actions reproduce the crash?
- [ ] What does the debugger's call stack show at the failure point?
- [ ] Are pointers, references, indexes, and iterators valid when accessed?
- [ ] Could invalid input, end-of-file, an empty collection, or a failed file operation trigger the crash?
- [ ] Is an uncaught exception responsible for termination?

## Undefined Behavior

- [ ] Is every value initialized before it is read?
- [ ] Are array, string, and container accesses within valid bounds?
- [ ] Could arithmetic overflow a signed integer or divide by zero?
- [ ] Are casts, object lifetimes, and iterator usage valid under C++17 rules?
- [ ] If threads are used, are shared mutable values accessed with appropriate synchronization?

## Memory Leaks and Dangling Pointers

- [ ] Who owns each resource, and is its lifetime managed through RAII?
- [ ] Can an owning raw pointer be replaced locally with a value or smart pointer?
- [ ] Can a pointer or reference outlive its object or survive a container reallocation or erasure?
- [ ] Do early returns and exceptions release memory, files, and other resources?
- [ ] Could shared ownership cycles prevent cleanup, and do supported memory tools report leaks or use-after-free?

## Multi-File Dependency Problems

- [ ] Do headers have include guards or an equivalent supported mechanism?
- [ ] Is each header self-contained, and are circular includes avoided?
- [ ] Are declarations in headers consistent with definitions in source files?
- [ ] Are non-inline functions and shared global objects defined only once?
- [ ] Does a clean rebuild compile and link every required translation unit without stale objects?

## Static Analysis

- [ ] Have I enabled compiler warnings, such as GCC/Clang's `-Wall -Wextra -Wpedantic` or MSVC's `/W4`?
- [ ] Are tools such as clang-tidy or Cppcheck configured for C++17 and the project's include paths and definitions?
- [ ] Does each warning indicate a real defect, and have I understood it before changing code or suppressing it?
- [ ] Do the findings reveal unchecked input, lifetime errors, or uninitialized values?
- [ ] Have I rerun analysis after the fix and checked for new findings?

## Testing After Applying a Fix

- [ ] Have I compiled and linked the application with C++17 after the fix?
- [ ] Have I executed the original failing scenario and confirmed the expected result?
- [ ] Have I tested valid input, malformed input, boundary values, empty records, and end-of-file?
- [ ] Do supported student operations, such as adding, finding, updating, or deleting records, still work?
- [ ] Where applicable, do saved records survive application restart, and do file failures behave correctly?
- [ ] Have existing tests passed, and has a regression test been added or updated where practical?
- [ ] Have I used supported runtime sanitizers or memory tools where relevant, and documented the root cause, minimal fix, test results, and any unverified behavior?
