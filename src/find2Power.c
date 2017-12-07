#include <stdint.h>
#include <stdio.h>

static inline int
is_pow_of_2(int x)
{
  return !(x & (x - 1));
}

static inline int
next_pow_of_2(int x)
{
  if (is_pow_of_2(x))
    return x;
  x |= x >> 1;
  x |= x >> 2;
  x |= x >> 4;
  x |= x >> 8;
  x |= x >> 16;
  return x + 1;
}

int main () {
  int memory_size = 1024;
  printf("next power of 2: %i", next_pow_of_2(32));
  return 0;
}