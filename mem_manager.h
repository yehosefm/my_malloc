#ifndef MEM_MANAGER_H
#define MEM_MANAGER_H

#include <stddef.h>

#define BLOCK1 64
#define BLOCK2 16
#define BLOCK3 8
#define NUM_OF_BLOCKS 3

void Init(void *_pblock, size_t _size);

void* MyMalloc(size_t _size);

void MyFree(void *_pblock);

#endif /*MEM_MANAGER_H*/
