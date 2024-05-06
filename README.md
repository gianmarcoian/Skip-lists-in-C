# Skip List Implementation in C

This repository contains an implementation of skip lists in C. Skip lists are a probabilistic data structure that provide an efficient alternative to balanced trees for many operations, particularly for search, insertion, and deletion.

## Skip Lists: Brief Overview

Skip lists are a probabilistic alternative to balanced trees, invented by William Pugh in 1989. They consist of a series of linked lists with multiple "levels". Each level contains a subset of the elements in the lower level. 

### Key Features:

- **Efficient Search**: Skip lists provide logarithmic time complexity for search, insertion, and deletion operations on average.
- **Simple Structure**: Skip lists are relatively easy to implement compared to other balanced tree structures like AVL trees or red-black trees.
- **Dynamic Operations**: Unlike trees that require rebalancing after insertion or deletion, skip lists naturally adjust their structure.
- **Probabilistic Structure**: The use of randomness in skip lists gives them a dynamic structure without the need for complex balancing algorithms.

### How it Works:

1. **Base Level**: The bottom level contains all the elements in sorted order.
2. **Tower Structure**: Each element has a tower of pointers, with the highest pointer pointing to the next occurrence of that element in the next level.
3. **Multiple Levels**: Higher levels contain fewer elements with exponentially decreasing probability. 

## Implementation Details

The implementation provided in `skiplist.c` includes functions for creating a skip list, searching for elements, inserting elements, and deleting elements. It is designed to be simple and efficient.


