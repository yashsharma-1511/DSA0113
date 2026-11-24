# DSA01-192525150-Assignment4

Name: Yash Sharma | Register number: 192525150 | Compiler: g++ 13.3.0 (Ubuntu)

Compile every program with: g++ -std=c++17 -Wall -Wextra file.cpp -o file
Sanitizer runs (Tasks 3, 4, 5): g++ -std=c++17 -g -fsanitize=address file.cpp -o file_asan
(For the leak report of t4_orig.cpp also run with ASAN_OPTIONS=new_delete_type_mismatch=0.)
Programs that show errors or warnings on purpose: t1_orig_loop, t2_amb, t3_cc_swap, t3_shallow, t4_orig, t4_uptr (run with an argument), t5_vec (run with an argument).
Task 6: vehicle.h holds the existing classes; t6.cpp is the demonstration; t6_bus.cpp adds ElectricBus without editing vehicle.h.
Task 2: the acc/ folder has the 18 access tests behind the access table.

Results: Task 1 total payroll 191200; Task 2 prints "Person Student Staff TA"; Task 3 trace and sizeof(TeachingAssistant) 48 (virtual) / 64 (non-virtual);
Task 4 repaired program: senior rate 4, ASan clean; Task 5 prints 324.00 (240.00 if add() returns by value);
Task 6 total revenue Rs 19100 and ElectricBus bill Rs 23940.
Commit each task folder separately (at least one commit per task) as the assignment requires.
