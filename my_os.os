Begin Data Section
0 0  # pc
1 755  # stack pointer
2 2
3 3
4 0
5 5
6 6
7 7
8 8
9 9
10 10
11 11
12 12
13 13
14 14
15 15
16 16
17 2
18 12300
19 0
20 0
21 66  
22 91  
23 116 
24 141 
25 166 
26 191 
27 216 
28 241 
29 266 
30 291 
31 316
50 50
51 51
52 52
53 53
54 1
55 55
56 56
57 57
58 58
59 59
60 60
61 61
62 62
63 63
64 64
65 65
66 66
75 0
79 1
150 150
151 151
152 152
153 153
154 154
155 155
156 156
157 157
158 158
159 159
160 160
161 161
162 162
163 163
164 164
165 165
166 166
167 167
168 168

# Round robin list
398 400 # head
399 418 # tail
400 1
401 402
402 2
403 404
404 3
405 406
406 4
407 408
408 5
409 410
410 6
411 412
412 7
413 414
414 8
415 416
416 9
417 418
418 10
419 0

End Data Section
Begin Instruction Section

# LOOP From Mem to Reg
# register 20, 19, 18, 17 will not be switched prensibally
80 SET 16 501    # [501] = 16  # i = 16 
81 SET 0 502     # [502] = 0   # x = 0 
82 CPYI2 500 501 # [[501]] = [[500]] # [16] = [68]
83 SET -1 504    # [504] = -1   # y = 0 
84 ADD 501 -1    # 16 # i--
85 ADDI 500 504  # 67 # [500] -= y
86 JIF 501 88
87 JIF 502 82

88 JIF 4 91      # if it is os jmp 90

89 CPYI 500 500  # [500] = [[500]]
90 USER 500      # This will move [19] = tt[4] and [0] = tt[0] for context switching
# OS End here if os -> user

91 JIF 502 4     # Return to handle syscall events

# LOOP From Reg to Mem
# 4 is the currently running_thread [501, 502, 504, 505, 506] will be used
50 CPY 4 505   # [505] = current_tn
51 ADD 505 21   # [505] = thread_number + 21  (address of the end of that thread_table location)
52 CPYI 505 505 # [505] = [[505]] # [505] = [68] # assign to [505] address of the end of the thread_table
53 SET 16 501    # [501] = 16  # i = 16 
54 SET 0 502     # [502] = 0   # x = 0 
55 CPYI2 501 505 # [[505]] = [[501]] # [68] = [16]
56 SET -1 504    # [504] = -1   # y = 0 
57 ADD 501 -1    # 16 # i--
58 ADDI 505 504  # 67 # [505] -= y
59 JIF 501 61
60 JIF 502 55

61 JIF 4 66      # if it is os jmp 64 to set 0 as pc

# if we switch from user thread to os thread
62 SET 18 506    # old_pc for the thread we need to save this
63 CPYI2 506 505 # [[505]] = [[506]] # [68 - 16] = (old_pc from [18]) # the correct pc for thread
64 CPY 4 33      # [33] = [4] thread number is assigned to syscaller  
65 JIF 502 2     # jump to calle

# if we switch from os to user thread
66 SET 502 506
67 CPYI2 506 505 # [[505]] = [[506]] # [68 - 16] = 0 # this will assing 0 as new pc for make os start from begining when came back
68 JIF 502 24

# CONTEXT SWITCH (thread_number) mem [34] will be the thread number that we want to changed to
21 CPY 34 500   # [500] = tn thread that will be SWITCH ed to
22 SET 0 502    # [502] = 0   # x = 0
23 JIF 502 50   # Send Registers to memory
24 CPY 34 500  # [500] = [503]   # temp
25 ADD 500 21   # [500] = thread_number + 21  (address of the end of that thread_table location)
26 CPYI 500 500 # [500] = [[500]] # [500] = [68] # assign to [500] address of the end of the thread_table
27 JIF 502 80
28 RET

# CONTEXT SWITCH END

# ROUN ROBIN FUNCTION BEGIN [330(TID SET), 331, 332, 333, 334]
91 JIF 398 109   # Return if the head is 0 which means that each thread is done
92 CPYI 398 330   # [330] = 0 Copy thread ID (T6) to register 4 (RUN the thread)
93 CPY 398 331    # [331] = 100 Copy address of that thread to register 331 TMP
94 CPY 331 332     # [332] = 100
95 ADD 332 1      # [332] = 101 which is address of  next of T6
96 CPYI 332 398   # [398] = 102 which is next of T6
97 CPY 399 333    # [333] = 120
98 ADD 333 1      # [333] = 121
99 SET 331 334     # [334] = 331
100 CPYI2 334 333  # [121] = 100 which means that tail-> next is T6 now
101 CPY 331 399   # [399] = 100 which means that tail shows T6 now
102 SET 0 331     # [331] = 0
103 SET 331 333    # [333] = 331
104 CPYI2 333 332  # [101] = 0 which means that tail shows T6 now
105 SET 0 331
106 SET 0 332
107 SET 0 333
108 SET 0 334
109 RET
# ROUN ROBIN FUNCTION END


# ROUND ROBIN REMOVE FUNCTION BEGIN (thread_number) [335, 336, 337, 338, 339, 340, 341, 342, 343, 344, 345, 346 = 0 (it needed to be set to 0 before calling)]
200 POP 341      # for return address
201 POP 335      # [335] = tn thread that will be removed from round robin
202 PUSH 341     # repush the return address
203 SET 0 337    # [337] = 0 will be used to jump
204 CPY 398 336  # [336] = [398] assign to 336 the heading thread number
205 CPYI 336 337 # [337] = [[336]] 
206 CPYI 336 339 # [339] = [[336]] 
207 CPY 335 338  # [338] = [335]
208 SUBI 335 337 # [337] = [335] - [337] # 4 - 5 = -1  =>   <= 0
209 SUBI 339 338 # [338] = [339] - [338] # 5 - 4 =  1  => ! <= 0
210 CPY 340 13   # [13] = [340] (398)
211 CPY 336 340  # [340] = [336] (100)
212 ADD 336 1    # [336] = [336] + 1 (101)
213 CPYI 336 336 # [336] = [[336]]
214 JIF 337 216
215 JIF 337 205
216 JIF 338 218
217 JIF 337 205


# If the thread was FOUND
218 JIF 13 221
219 CPY 13 343
220 ADD 343 1
221 CPY 340 342
222 ADD 342 1
223 JIF 13 234


# check convertions for cases that are not head
224 CPYI2 342 343 # [[343]] = [[342]] = 104
225 SET 346 344
226 CPYI 342 345  # [345] = [[342]] = 104
227 CPYI2 344 342 # [[342]] = [[344]] = 0 # 8->next = 0
228 JIF 345 230 # if it is tail
229 JIF 337 237 # if it is not tail


# change tail to [343] - 1 (101 - 1)
230 CPY 343 344   # [344] = [343] = 101
231 ADD 344 -1    # [344] = 100
232 CPY 344 399
233 JIF 337 237

# change head to head -> next if the removed one is first one
234 CPYI 342 398 # [head] = [101]
235 SET 346 344
236 CPYI2 344 342 # [[342]] = [[344]]

237 SET 0 335
238 SET 0 340
239 SET 0 336
240 SET 0 337
241 SET 0 338
242 RET
# ROUN ROBIN REMOVE FUNCTION END


# SYSCALL YIELD START
120 CALL 91      # TID set to 330
121 CPY 330 505  # [505] = current_tn
122 ADD 505 21   # [505] = thread_number + 21  (address of the end of that thread_table location)
123 CPYI 505 505 # [505] = [[505]] # [505] = [68] # assign to [505] address of the end of the thread_table
124 ADD 505 -7   # [505] = thread state
125 CPYI 505 505 # [505] =  (rdy:0, blc:1, run:2)
126 JIF 505 129  # thread is ready go context switch
127 ADD 505 -1
128 JIF 505 120  # thread is BLOCKED call new one from round robin
129 SET 1 34
130 CALL 21
# SYSCALL PRN END


# SYSCALL HLT START
131 SET 0 346
132 PUSH 33   # call remove by [33] syscallee
133 CALL 200  # Call remove round robin for hlt
134 CALL 120  # CALL Round Robin
# SYSCALL HLT END


# SYSCALL PRN START
# 33, CALLER THREAD WILL BE BLOCKED FOR 100 instruction

# CALL ROUND ROBIN
# SYSCALL PRN END


# SYSCALL occured. We need to context switch user -> os
0 SET 0 32
1 JIF 32 50  # put user data from reg to mem
2 CPY 21 500
3 JIF 32 80  # put os data from mem to reg
4 CPY 17 500 # syscall type to [500]
5 JIF 500 131 # if HLT
6 ADD 500 -1
7 JIF 500 120 # if YIELD
#8 ADD 500 -1
#9 JIF 500 3000 # PRN
 HLT

# MAY be Standart start of OS with thread 1
# 0 SET 1 34
# 1 CALL 21
# 2 HLT



End Instruction Section

# [11-20]
# [29-49] -- SYSCALL PRN
# [69-79]
# [109-119]
# [131-]
# Example usage of the after round robin context-switch 
# Start Of the function
#40 PUSH 4
#41 CALL 21
#42 HLT

Begin Data Section
0 0
1 958
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
23 1
24 0
25 0
26 0
27 0
28 0
29 0
End Data Section
Begin Instruction Section
0 SET 1 1
1 SET 1 16
2 SET 1 16
3 SYSCALL HLT
End Instruction Section

