# TickMerger
  - Program uses k-way merge to merge multiple sorted tickfiles in a single sorted file.
  - Program is designed to handle very large input data sets.
  - Program smartly uses 2 queues (activeQ and backupQ) while activeQ is being consumed by main thread for merging, backupQ is filled
    by as async task. When activeQ is fully consumed. It switches the activeQ and backUpQ and both can be used at same time.
  - Program uses memory mapped files for faster read/write (currently setup to consume data in 4MB chunks)
  - Program lacks handling 10000 files to keep it simple and within timeframe. I would take 2 approaches to solve that problem, 
       i. - Either run multiple instances and give them disjoint data sets (can be handled by a script)
      ii. - Implement multiple levels, write intermediate files and merge them later using the same algo.
     iii. - K- value is defaulted to 7. Can be fine tuned further by doing sufficient testing.
  - To keep it simple I have not focused too much on performance, program can be tuned further to improve.

# depends
1. Gnumake
2. c++ 17
3. Boost library

# compilation
cd src
make
