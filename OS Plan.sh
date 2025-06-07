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

# Thread Tables:
#
# thread_table 0: [21] = 66  # (0 + 21) // end of the thread_tables
# thread_table 1: [22] = 91  # (1 + 21)
# thread_table 2: [23] = 116  # (2 + 21)
# thread_table 3: [24] = 141  # (3 + 21)
# thread_table 4: [25] = 166  # (4 + 21)
# thread_table 5: [26] = 191  # (5 + 21)
# thread_table 6: [27] = 216  # (6 + 21)
# thread_table 7: [28] = 241  # (7 + 21)
# thread_table 8: [29] = 266  # (8 + 21)
# thread_table 9: [30] = 291  # (9 + 21)
# thread_table 10: [31] = 316  # (10 + 21)
#
# thread_tables size is: [50-324]
#
# Syscaller thread: [33]
#
# empty memory blocks: [🗑️32], [🗑️34], [🗑️35], [🗑️36] [🗑️37] [🗑️38] [🗑️39] [🗑️40] [🗑️41] [42-49], [325-329] 🗑️[330-347], [348-397]
# Round Robin Scheduler:
#
# head of the round robin scheduler:             [398]
# tail of the round robin scheduler:             [399]
# first thread number on round robin scheduler:  [400]
# the address of the next of first thread:       [401]
# second thread number on round robin scheduler: [402]
# the address of the next of second thread:      [403]
# third thread number on round robin scheduler:  [404]
# the address of the next of third thread:       [405]
# fourth thread number on round robin scheduler: [406]
# the address of the next of fourth thread:      [407]
# fifth thread number on round robin scheduler:  [408]
# the address of the next of fifth thread:       [409]
# sixth thread number on round robin scheduler:  [410]
# the address of the next of sixth thread:       [411]
# seventh thread number on round robin scheduler:[412]
# the address of the next of seventh thread:     [413]
# eighth thread number on round robin scheduler: [414]
# the address of the next of eighth thread:      [415]
# ninth thread number on round robin scheduler:  [416]
# the address of the next of ninth thread:       [417]
# tenth thread number on round robin scheduler:  [418]
# the address of the next of tenth thread:       [419]
#
# round robin scheduler size is: [398-419]
#

thread_tables:
  thread_table os: # [50-74]
    1 1 1 1 1 1 // all the registers
  thread_table 1:  # [75-99]
    1 1 1 1 1 1
  thread_table 2:  # [100-124]
    1 1 1 1 1 1
  thread_table 3:  # [125-149]
    1 1 1 1 1 1
    . . .
  thread_table 10: # [300-324]
    1 1 1 1 1 1

    0  1  2  3   4   5   6   7   8   9   10
    25 50 75 100 125 150 175 200 225 250 275

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