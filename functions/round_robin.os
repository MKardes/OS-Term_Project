Begin Data Section
0 19    # pc
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
200 0 # i
201 0
End Data Section
Begin Instruction Section

# ROUN ROBIN FUNCTION BEGIN
0 JIF 98 18   # Return if the head is 0 which means that each thread is done
1 CPYI 98 14  # [14] = 0 Copy thread ID (T6) to register 4 (RUN the thread) //TODO: 14 for now it will be 4 to make it execute it's tasks but don't forget it needed to move to 4 at the and of the scheduling
2 CPY 98 15   # [15] = 100 Copy address of that thread to register 15 TMP
3 CPY 15 16   # [16] = 100
4 ADD 16 1    # [16] = 101 which is address of  next of T6
5 CPYI 16 98  # [98] = 102 which is next of T6
6 CPY 99 17   # [17] = 120
7 ADD 17 1    # [17] = 121
8 SET 15 18   # [18] = 15
9 CPYI2 18 17 # [121] = 100 which means that tail-> next is T6 now
10 CPY 15 99   # [99] = 100 which means that tail shows T6 now
11 SET 0 15    # [15] = 0
12 SET 15 17   # [17] = 15
13 CPYI2 17 16 # [101] = 0 which means that tail shows T6 now
14 SET 0 15
15 SET 0 16
16 SET 0 17
17 SET 0 18
18 RET
# ROUN ROBIN FUNCTION END

19 SET -25 200
20 ADD 200 1
21 CALL 0
22 JIF 200 20
23 HLT

End Instruction Section