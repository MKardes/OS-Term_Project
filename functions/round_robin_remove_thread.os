Begin Data Section
0 43    # pc
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
# ROUND ROBIN REMOVE FUNCTION BEGIN
POP 500    # for return address
POP 14     # [14] = tn thread that will be removed from round robin
PUSH 500   # repush the return address
SET 0 20   # [20] = 0 will be used to jump
CPY 98 16  # [16] = [98] assign to 16 the heading thread number
CPYI 16 17 # [17] = [[16]] 
CPYI 16 19 # [19] = [[16]] 
CPY 14 18  # [18] = [14]
SUBI 14 17 # [17] = [14] - [17] # 4 - 5 = -1  =>   <= 0
SUBI 19 18 # [18] = [19] - [18] # 5 - 4 =  1  => ! <= 0
CPY 15 13  # [13] = [15] (98)
CPY 16 15  # [15] = [16] (100)
ADD 16 1   # [16] = [16] + 1 (101)
CPYI 16 16 # [16] = [[16]]
JIF 17 16
JIF 20 5
JIF 18 18
JIF 20 5
# 102 It is equal FOUND
JIF 13 21
CPY 13 200
ADD 200 1
CPY 15 201
ADD 201 1
JIF 13 34 # it is head TODO: 34 will be moved to place that changes head (FOr now it just do nothing)
#
#
# check convertions for cases that are not head
CPYI2 201 200 # [[200]] = [[201]] = 104
SET 20 202
CPYI 201 203  # [203] = [[201]] = 104
CPYI2 202 201 # [[201]] = [[202]] = 0 # 8->next = 0
JIF 203 30 # if it is tail
JIF 20 37 # if it is not tail
#
#
# change tail to [200] - 1 (101 - 1)
CPY 200 204 # [204] = [200] = 101
ADD 204 -1    # [204] = 100
CPY 204 99
JIF 20 37
#
# change head to head -> next if the removed one is first one
CPYI 201 98 # [head] = [101]
SET 20 202
CPYI2 202 201 # [[201]] = [[202]]
#
SET 0 14
SET 0 15
SET 0 16
SET 0 17
SET 0 18
RET
# ROUN ROBIN REMOVE FUNCTION END
SET 0 300
PUSH 300
CALL 0
SET 1 300
PUSH 300
CALL 0
SET 2 300
PUSH 300
CALL 0
SET 3 300
PUSH 300
CALL 0
SET 4 300
PUSH 300
CALL 0
SET 5 300
PUSH 300
CALL 0
SET 10 300
PUSH 300
CALL 0
SET 9 300
PUSH 300
CALL 0
SET 8 300
PUSH 300
CALL 0
SET 7 300
PUSH 300
CALL 0
SET 6 300
PUSH 300
CALL 0
HLT
End Instruction Section