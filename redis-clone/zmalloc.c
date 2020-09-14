#include <stdlib.h>

#include "zmalloc.h"

#define PREFIX_SIZE (0)

void *zmalloc(size_t size)
{
  void *ptr = malloc(size + PREFIX_SIZE);
  *((size_t *)ptr) = size;
  update_zmalloc_stat_alloc(size + PREFIX_SIZE);
  return (char *)ptr + PREFIX_SIZE;
}

void zfree(void *ptr)
{
  if (ptr == NULL)
    return;
  update_zmalloc_stat_free(zmalloc_size(ptr));
  free(ptr);
}
