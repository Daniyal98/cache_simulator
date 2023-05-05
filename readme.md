# Cache Simulator

An implementation of a simulation of a level-1 cache. This project will be able to simulate direct-mapped, set-associative and fully associative unified and split Level-1 caches.

The simulator is known as a trace-driven simulator because it takes as input a trace of events, in this case memory references. In this project, the memory reference events specified in the trace(s) given will be used by the simulator to drive the movement of data into and out of the cache, thus simulating its behavior. Trace-driven simulators are very effective for studying caches.

The Level-1 Cache Simulator should accept the following command-line options:
-s < split > or < unified >.
-c < capacity > with < capacity > in KB: 4, 8, 16, 32, or 64.
-b < blocksize > with < blocksize > in bytes: 4, 8, 16, 32, 64, 128, 256, or 512.
-a < associativity > where < associativity > is integer size of set: 1, 2, 4, 8 or 16.

`cache -s -c8 -b16 -a4`

The -s option specifies a split cache. This option indicates that the L1 cache is split equally into L1D (Data Cache) and L1I (Instruction Cache). The -c option gives the combined size of the L1 cache, split equally between L1D and L1I. The block size and associativity is the same for both L1D and L1I. If the -s option is not given then the cache is unified by default, as before (i.e. instruction reads are also treated as data reads).

Additionally, the cache would have the following functionality to handle data write hits and misses with optional command-line options:
– -wb write back (if write strategy not specified then write-back should be default for write hits)
– -wt write through
– -wa write allocate (if write strategy not specified then write-allocate should be default for write misses) – -wn write no-allocate

The cache has LRU (least-recently-used) replacement policy for fully associative and set-associative caches.

The input to the cache simulator will be a sequence of memory access traces, one per line, terminated by end of file. In the following format, with a leading 0 for data loads, 1 for data stores and 2 for instruction load. You should ignore anything (possible comments) that follows.

0 < address >

1 < address > < dataword >

2 < address >

Each address will be the address of a 32-bit data word. The address and data are expressed in hexadecimal format.