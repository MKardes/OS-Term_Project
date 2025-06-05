Begin Data Section
0 19    # pc
1 800  # stack pointer
2 0
3 0
4 0
5 0
6 0
7 0
8 0
9 0
10 0
11 0
12 0
13 0
14 0
15 0
16 0
17 0
18 0
19 0
20 0
21 0
22 0
23 0
24 0
25 0
26 0
27 0
28 0
29 0
30 0
31 0
32 0
33 0
34 0
35 0
36 0
37 0
38 0
39 0
40 0
41 0
42 0
43 0
44 0
45 0
46 0
47 0
48 0
49 0
50 0
51 0
52 0
53 0
54 0
55 0
56 0
57 0
58 0
59 0
60 0
61 0
62 0
63 0
64 0
65 0
66 0
67 0
68 0
69 0
70 0
71 0
72 0
73 0
74 0
75 0
76 0
77 0
78 0
79 0
80 0
81 0
82 0
83 0
84 0
85 0
86 0
87 0
88 0
89 0
90 0
91 0
92 0
93 0
94 0
95 0
96 0
97 0
98 100 # head
99 120 # tail
100 0
101 102
102 1
103 104
104 2
105 106
106 3
107 108
108 4
109 110
110 5
111 112
112 6
113 114
114 7
115 116
116 8
117 118
118 9
119 120
120 10
121 0
200 0 # i
201 0
End Data Section
Begin Instruction Section
# ROUN ROBIN FUNCTION BEGIN
JIF 98 18   # Return if the head is 0 which means that each thread is done
CPYI 98 14  # [14] = 0 Copy thread ID (T6) to register 4 (RUN the thread) //TODO: 14 for now it will be 4 to make it execute it's tasks but don't forget it needed to move to 4 at the and of the scheduling
CPY 98 15   # [15] = 100 Copy address of that thread to register 15 TMP
CPY 15 16   # [16] = 100
ADD 16 1    # [16] = 101 which is address of  next of T6
CPYI 16 98  # [98] = 102 which is next of T6
CPY 99 17   # [17] = 120
ADD 17 1    # [17] = 121
SET 15 18   # [18] = 15
CPYI2 18 17 # [121] = 100 which means that tail-> next is T6 now
CPY 15 99   # [99] = 100 which means that tail shows T6 now
SET 0 15    # [15] = 0
SET 15 17   # [17] = 15
CPYI2 17 16 # [101] = 0 which means that tail shows T6 now
SET 0 15
SET 0 16
SET 0 17
SET 0 18
RET
# ROUN ROBIN FUNCTION END
SET -25 200
ADD 200 1
CALL 0
JIF 200 20
HLT
End Instruction Section