#ifndef __ZMALLOC_H
#define __ZMALLOC_H

#include <malloc/malloc.h>   // apple


void *zmalloc(size_t size);

void zfree(void *ptr);

#endif