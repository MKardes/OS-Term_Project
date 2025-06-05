OS job:

# thread_table 1:
#  The registers of the threads.
#   - The PC                                                                                        -> will be written to reg[0]
#   - The stack pointer                                                                             -> will be written to reg[1]
#  The thread ID                                                                                    -> will be written to reg[4] (0(os), 1, 2, 3, ... 10)
#  Starting time of the thread (how many instructions were executed since the system bootup by CPU) -> will be written to reg[5]
#  How many instruction executions the thread has used so far (for that thread)                     -> will be written to reg[6]
#  The state of the thread (ready, blocked, running)                                                -> will be written to reg[7] (rdy:0, blc:1, run:2)
#  Start time (instruction) of the block (100 inst etc.)                                            -> will be written to reg[8] (reg[3] - reg[8] > 100 ? blc->rdy: blc->blc)

# Starts from OS's 30'th memory block 10 blocs foreach
thread_tables:
  thread_table os: # [30-39]
    1 1 1 1 1 1
  thread_table 1:  # [40-49]
    1 1 1 1 1 1
  thread_table 2:  # [50-59]
    1 1 1 1 1 1
  thread_table 3:  # [60-69]
    1 1 1 1 1 1
    . . .
  thread_table 10: # [120-129]
    1 1 1 1 1 1

EXECUTE [head]        # -'Head'.content will be executed (T6)
Set [Head] 94         # -put address T6 to a temp var
set [94 + 1\] head    # -'Head' will be T6 -> next
set [94] ([tail] + 1) # -'Tail'->next will be 'T6'
set [94] [tail]       # -'Tail' will be the address of T6

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

head: [98] 100
tail: [99] 120
round robin scheduler:
  [100] 0 thread_id
  [101] 102 # next
  # ---------------
  [102] 1 thread_id
  [103] 104 # next
  # ---------------
  [104] 2 thread_id
  [105] 106 # next
  # ---------------
  [106] 3 thread_id
  [107] 108 # next
  # ---------------
  [108] 4 thread_id
  [109] 110 # next
  # ---------------
  [110] 5 thread_id
  [111] 112 # next
  # ---------------
  [112] 6 thread_id
  [113] 114 # next
  # ---------------
  [114] 7 thread_id
  [115] 116 # next
  # ---------------
  [116] 8 thread_id
  [117] 118 # next
  # ---------------
  [118] 9 thread_id
  [119] 120 # next
  # ---------------
  [120] 10 thread_id
  [121] 0 # next
  # ---------------