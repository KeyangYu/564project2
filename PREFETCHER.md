1. Describe how your prefetcher works.

    The prefetcher I implemented is the stride prefetching. 
    The prefetcher detects regular patterns in memory accesses and prefetches data according to the record. The method is effecgtive for linear data structures. 
    If the memory access instruction has constant stride, if so, the algorithm will prefetch block at 'last_accessed_address + len(stride)'

2. Explain how you chose that prefetch strategy.

    This is one of the most well-known prefetching strategy, and easy to implement.

3. Discuss the pros and cons of your prefetch strategy.

    Pros:
    1. Efficient use of memory bandwidth: Stride prefetching helps reduce cache misses by prefetching memory blocks that are likely to be accessed in the near future. 
    2. Simple implement: Stride prefetching is relatively simple which requires less hardware and software resources. 
    3. Adaptive to program behavior: Stride prefetching is adaptive to the memory access pattern of the program. The prefetching behavior can be adjusted based on the memory access pattern. This helps to improve the accuracy of prefetching and reduce the number of unnecessary prefetches.
    
    Cons:
    1. Limited prediction accuracy: Stride prefetching have limited accuracy due to the prediction algorithm. A failure for accurately predict the stride will lead to negative performance impacts.
    2. High hardware overhead: Stride prefetching requires additional hardware to keep track of te predicted strides and perform the prefecting operations. 
    3. Sensitivity to cache size: Stride prefetching is sensitive to the size of the cache, as larger caches can reduce the number of cache misses and prefetching operations needed. The performance of the stride pretecting may be limited by smaller cache sizes.
4. Demonstrate that the prefetcher could be implemented in hardware (this can be
   as simple as pointing to an existing hardware prefetcher using the strategy
   or a paper describing a hypothetical hardware prefetcher which implements
   your strategy).
    Reference Prediction Table (RPT) is capable of holding the information about most recently executed memory instructions to observe the access pattern.

5. Cite any additional sources that you used to develop your prefetcher.
    https://my.eng.utah.edu/~cs7810/pres/14-7810-13-pref.pdf
    https://compas.cs.stonybrook.edu/~nhonarmand/courses/sp15/cse502/slides/13-prefetch.pdf
    https://www.sciencedirect.com/science/article/abs/pii/S1383762100000114
    https://chat.openai.com/c/18f8f184-2862-4ef2-b269-64b548ed402e
