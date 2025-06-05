Begin Data Section
0 43    # pc

1 800  # stack pointer
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
200 0
201 0
End Data Section
Begin Instruction Section

# ROUND ROBIN REMOVE FUNCTION BEGIN (thread_number)
0 POP 500    # for return address
1 POP 14     # [14] = tn thread that will be removed from round robin
2 PUSH 500   # repush the return address
3 SET 0 17   # [17] = 0 will be used to jump
4 CPY 98 16  # [16] = [98] assign to 16 the heading thread number
5 CPYI 16 17 # [17] = [[16]] 
6 CPYI 16 19 # [19] = [[16]] 
7 CPY 14 18  # [18] = [14]
8 SUBI 14 17 # [17] = [14] - [17] # 4 - 5 = -1  =>   <= 0
9 SUBI 19 18 # [18] = [19] - [18] # 5 - 4 =  1  => ! <= 0
10 CPY 15 13  # [13] = [15] (98)
11 CPY 16 15  # [15] = [16] (100)
12 ADD 16 1   # [16] = [16] + 1 (101)
13 CPYI 16 16 # [16] = [[16]]
14 JIF 17 16
15 JIF 17 5
16 JIF 18 18
17 JIF 17 5

# If the thread was FOUND
18 JIF 13 21
19 CPY 13 200
20 ADD 200 1
21 CPY 15 201
22 ADD 201 1
23 JIF 13 34 # it is head TODO: 34 will be moved to place that changes head (FOr now it just do nothing)


# check convertions for cases that are not head
24 CPYI2 201 200 # [[200]] = [[201]] = 104
25 SET 20 202
26 CPYI 201 203  # [203] = [[201]] = 104
27 CPYI2 202 201 # [[201]] = [[202]] = 0 # 8->next = 0
28 JIF 203 30 # if it is tail
29 JIF 17 37 # if it is not tail


# change tail to [200] - 1 (101 - 1)
30 CPY 200 204   # [204] = [200] = 101
31 ADD 204 -1    # [204] = 100
32 CPY 204 99
33 JIF 17 37

# change head to head -> next if the removed one is first one
34 CPYI 201 98 # [head] = [101]
35 SET 20 202
36 CPYI2 202 201 # [[201]] = [[202]]


37 SET 0 14
38 SET 0 15
39 SET 0 16
40 SET 0 17
41 SET 0 18
42 RET
# ROUN ROBIN REMOVE FUNCTION END


43 SET 0 300
44 PUSH 300
45 CALL 0
46 SET 1 300
47 PUSH 300
48 CALL 0
49 SET 2 300
50 PUSH 300
51 CALL 0
52 SET 3 300
53 PUSH 300
54 CALL 0
55 SET 4 300
56 PUSH 300
57 CALL 0
58 SET 5 300
59 PUSH 300
60 CALL 0
61 SET 10 300
62 PUSH 300
63 CALL 0
64 SET 9 300
65 PUSH 300
66 CALL 0
67 SET 8 300
68 PUSH 300
69 CALL 0
70 SET 7 300
71 PUSH 300
72 CALL 0
73 SET 6 300
74 PUSH 300
75 CALL 0
76 HLT
End Instruction Section