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


round robin scheduler:
  [100] 0 thread_id
  [101] 102 # next
  # ---------------
  [102] 1 thread_id
  [103] 120 # next
  # ---------------
  [104] 2 thread_id
  [105] 110 # next
  # ---------------
  [106] 3 thread_id
  [107] 104 # next
  # ---------------
  [108] 4 thread_id
  [109] 100 # next
  # ---------------
  [110] 5 thread_id
  [111] 114 # next
  # ---------------
  [112] 6 thread_id
  [113] 108 # next
  # ---------------
  [114] 7 thread_id
  [115] 118 # next
  # ---------------
  [116] 8 thread_id
  [117] 112 # next
  # ---------------
  [118] 9 thread_id
  [119] 116 # next
  # ---------------
  [120] 10 thread_id
  [121] 106 # next
  # ---------------