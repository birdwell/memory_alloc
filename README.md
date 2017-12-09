# Memory Allocation Algorithms

## First Fit
Location: memory_algos/firstfit.c
This algorithm starts at the beginning of the memory allocaiton. It searches until it finds a spot that can hold it. 
It finishes here and goes with that slot.

1) Runs through the array keeping track of empty slots
2) 
  - It breaks out when it finds an exact slot size. 
  - It resets if it finds something other than an empty string
3) It then allocates the block.

## Next Fit
Location: memory_algos/nextfit.c
This algorithm is very similar to first fit. The difference is that it keeps track of where it left off in the insertion. 
This alogrithm also needs to wrap around to the start if there wasn't a found slot from the next index to the end of the memory allocation.

1) It starts at the next index location.
2) Runs through the array keep track of empty slots. 
3)
  - It finds a location and breaks to allocate.
  - It doesn't find a location and reruns from 0 to next_index.

## Best Fit
Location: memory_algos/bestfit.c
This algorithm searches the whole memory allocation for the best possible spot. It can opt out if an exact spot is found. 

1) It starts to run through the array looking for the best fit.
2) There are variables for the current best fit: bf_size, bf_start, bf_diff.
3) There are temp variables that keep track of the next possible slot. 
4) Once that slot size is finished, it compares to see if it's diff between size and process request size is bigger or smaller than that 
   of the current best. It can opt out if it finds a perfect match
5) At the end, it allocates to the best possible fit.

## Buddy

Buddy uses 2^k slots. You start with an 2^n slot. Then you split until a small enough slot is found. You search for a whole if it exists after that. Upon removal of a node, you clean up and merge back the slots until you have a big slot left. 

This is created with a buddy struct as nodes in a binary tree. 

I used this to print out for testing purposes my binrary tree: http://www.randygaul.net/2015/06/15/printing-pretty-ascii-trees/
This help me visualize what my tree should actually look like. 

```
typedef struct buddy
{
  int size;
  int memory_space;
  bool is_alloc;
  char process_id[16];
  int start;

  struct buddy *left;
  struct buddy *right;
} buddy_t;
```

All of the functions are seperate for the buddy system. This was a seperate project in my mind. I finished the first three algos and then started fresh here. This took most of my time during the project. My buddy system isn't spotless. I deeply understand buddy system now. This stretched my limits in C programming. I used recursion and binrary trees. I wanted to actually attempt it. I took up the challenge. I wish I could continue to polish my algorithm. 

### Allocaiton:
I am able to search for a suitable hole for it. It creates empty nodes until a perfect hole is found. It then allocates it in the hole. 

### Release:
I searched the tree for the process tree. Once I find it, I empty out that node. I then have a clean tree function that recursively merges empty slots in the tree until it's clean. 

For the other, commands I did a simple approach of iterating through the tree and printing the needed information of where they are allocated. 

Here are some areas I want to continue to improve on. 

BUDDY TODO: 
  - releative locations for allocating, releasing
  - edges cases on list avaliable
  - edge edge cases with the binrary tree in usuage

# Commands

## REQUEST 
Useage: REQUEST A n
```
  Allocates n bytes for a process named A . On successful allocation, print
  ALLOCATED A x , where A is the name of the allocated process and x is
  the relative location of the allocated memory. On error, print
  FAIL REQUEST A n .
```

This is the only command that isn't shared between the algorithims. Each has their own allocate method inside memory_algos. 
The implementation details will be laid out in the Memory Allocation Algorithms section.


# RELEASE
Useage: RELEASE A
```
Releases the memory held by a process A . On success, print FREE A n x
where n is the amount of reclaimed memory and x is the start relative
address of the released memory. On error, print FAIL RELEASE A .
```

Release simply iterates through the array and finds the occurance of that process id and removes it. It keeps track of size and start of the process will doing so.

# LIST AVAILABLE
```
Prints location information for all available memory. Prints a list of pairs of "(n1,x1) (n2, x2)...", 
where is the amount available and is the starting
location. If there are no available blocks you should return FULL.
```

This runs through the array and looks for empty slots. Once it encounters a allocation, it prints out. There is an edge case covered when there is an allocation at the end of the array. Because, it will not encounter another allocated block so it needs to print before completion.

# LIST ASSIGNED
```
Returns a list of space separates=d triples including of the form "
", where represents process labels, represents the number
of allocated bytes for that process, and is the relative starting address of the
process. If there are no allocated blocks, this should return NONE .
```

This runs through and keeps track of a start and size. Once it hits an empty memory slot, it closes off the last memory allocation it was tracking and prints it.

# FIND
```
Locates the starting address and size allocated by the process labeled A . If
successful, this command returns an tuple , where is the amount
allocated by A and x is the relative starting address of the process labeled
A .
```

Find is specifiying the search parameter. Once it is found, we start tracking the information we need to print. 