====================================================
   STUDENT RECORD MANAGEMENT SYSTEM
   (Singly Linked List / Array Implementation)
====================================================

1. PROJECT DESCRIPTION
----------------------------------------------------
This system is developed using C++ to manage student
records using fundamental data structures (Array or
Singly Linked List). It supports insertion, deletion,
searching, displaying, sorting, and performance
analysis using multiple dataset sizes.

The system is designed to evaluate and compare time
complexity (Big-O) using C++ <chrono> in microseconds.

----------------------------------------------------

2. SOURCE CODE FILE
----------------------------------------------------
Main C++ file:

TP085679_DingHaoZhe_CT077_LW1.cpp

----------------------------------------------------

3. HOW TO COMPILE
----------------------------------------------------
Using g++ compiler:

g++ TP085679_DingHaoZhe_CT077_LW1.cpp -o student_system

OR in Visual Studio / CodeBlocks:
- Open the .cpp file or project
- Build the project
- Run the program

----------------------------------------------------

4. HOW TO RUN
----------------------------------------------------
After compilation:

Windows:
student_system.exe

Linux/Mac:
./student_system

----------------------------------------------------

5. FEATURES
----------------------------------------------------
The system supports the following operations:

- Insert student record
- Delete student record by Student ID
- Search student by ID or Name
- Display all student records
- Sort records by CGPA
- Load data from CSV file
- Count total records (Linked List only)
- Performance measurement using chrono (μs)

----------------------------------------------------

6. DATASET FILES REQUIRED
----------------------------------------------------
The program uses the following CSV files:

- students_500.csv
- students_2000.csv
- students_8000.csv
- students_30000.csv
- programmes.csv

All files must be placed in the same directory as the
executable.

----------------------------------------------------

7. IMPLEMENTATION DETAILS
----------------------------------------------------
- Programming Language: C++
- Data Structure: Singly Linked List
- Sorting Algorithm: Bubble Sort
- Searching Algorithm: Linear Search
- No STL containers (vector, list, etc.) are used

----------------------------------------------------

8. PERFORMANCE ANALYSIS
----------------------------------------------------
Execution time for each operation is measured using
C++ <chrono> in microseconds (μs). These results are
used to analyze and compare time complexity growth
across different dataset sizes.

----------------------------------------------------

9. AUTHOR
----------------------------------------------------
Name: Ding Hao Zhe
Student ID: TP085679
Course: CT077 Data Structures

====================================================
END OF README
====================================================