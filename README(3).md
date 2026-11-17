# CodeAlpha_StudentManagement

Task 3 (High) of the CodeAlpha C Programming Internship.

A menu-driven student record manager written in C. Records are stored permanently in a binary file (`students.dat`) using structures and file handling.

## Features
- **Add** a student (ID, name, age, course, GPA) with duplicate-ID protection
- **Display** all records in a formatted table
- **Search** by ID or by name (case-insensitive, partial match)
- **Update** any single field of a record, in place
- **Delete** a record with a confirmation prompt
- Input validation on every field; data persists between runs

## Concepts used
- `struct Student` as the record type
- Binary file I/O: `fopen`, `fread`, `fwrite`, `fseek`, `rewind`, `remove`, `rename`
- Modular functions, `switch` menu, safe input with `fgets` + `strtol`/`strtod`

## Compile and run
```
gcc student_management.c -o student_management
./student_management
```

## Sample output
```
ID       Name                   Age   Course                 GPA
--------------------------------------------------------------------
102      Ama Serwaa             20    Computer Science       3.95
103      Kofi Boateng           18    Mechanical Engineering 3.20

Total students: 2
```

## Note
`students.dat` is created automatically the first time you add a student.
