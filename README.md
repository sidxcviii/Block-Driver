# Block-Driver

This program implements a device driver that sits between a virtual application and virtualized hardware. It implements the basic UNIX file interface using the memory system. The driver allows the user to read, write, seek, turn on and turn off the system. The device driver also implements persistence across sequential runs which allows it to remember the state of the device driver after it is powered off and on again. Along with persistence, the device driver also allows LRU caching to support several large files and frames dynamically. The resulting cache hit ratio is listed below.
Cache size Target hit ratio (%)
5             52.19%
10            53.71%
25            58.34%
50            67.34%
100           88.02%
250           99.12%
500           99.12%
1000          99.12%
1024          99.12%
