# DSA01-192525150-Assignment2

Name: Yash Sharma  |  Register number: 192525150  |  Department: AIML

Compiler: g++ 13.3.0 (Ubuntu 24.04)
Compile command used for every program: `g++ -std=c++17 -Wall -Wextra file.cpp -o file`
(timing programs also with `-O2`, and Q6 with `-O0` and `-O2`; see the report)

One folder per question. Run a program with `./file` after compiling.

| Folder | Files | Result |
|---|---|---|
| Q1 | q1_predict, q1_complete, q1_three_ways, q1_design, q1_alt, q1_constref_err (compile error expected) | Fee changes only by reference/pointer; returning the new fee chosen |
| Q2 | q2_marks, q2_dangling (warning + crash), q2_vector, q2_reserve, q2_alt, q2_ro (error expected) | Reference to element works; dangling reference crashes; vector growth invalidates references |
| Q3 | q3_ambiguous (error expected), q3_design, q3_conv (error expected), q3_defaults_err (error expected), q3_alt | Six calls resolved as predicted |
| Q4 | q4_account (-DHISTORY=50 for 50 entries), q4_bench (-O2 -DHISTORY=10 or 50), q4_private_err (error expected) | Last 10 transactions kept; circular buffer O(1) |
| Q5 | q5_predict, q5_complete, q5_design, q5_func, q5_alt, q5_bench, q5_nodef / q5_static_err / q5_noinit_err (errors expected) | 101 103 104 4; seats and waiting list verified |
| Q6 | q6_complete, q6_tests, q6_public, q6_bench.cpp + time.h + time_fn.cpp (-DVARIANT=0..3, -O0/-O2) | 205 false; all tests pass; inline timings |
| Q7 | account_v1.h, account_v2.h, q7_test.cpp (-DUSE_V2 for the redesign), q7_bench.cpp (-DHISTORY=K) | Circular buffer: same output, flat time |

Add your own output screenshots to each folder and commit task by task (at least one commit per question).
