#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int compare_ints(const void *a, const void *b) {
  int x = *((int*)a);
  int y = *((int*)b);
  return (x > y) - (x < y);
}

int compare_shorts(const void *a, const void *b) {
  short x = *((short*)a);
  short y = *((short*)b);
  return (x > y) - (x < y);
}

int compare_floats(const void *a, const void *b) {
  float x = *((float*)a);
  float y = *((float*)b);
  return (x > y) - (x < y);
}

int compare_chars(const void *a, const void *b) {
  char x = *((char*)a);
  char y = *((char*)b);
  return (x > y) - (x < y);
}

void swap(void *a, void *b, size_t size) {
  char *pa = a, *pb = b;
  for (size_t i = 0; i < size; ++i) {
    char tmp = *pa;
    *pa++ = *pb;
    *pb++ = tmp;
  }
}

void bubble_sort(void *array, size_t count, size_t size, int (*compare)(const void*, const void*)) {
  for (size_t i = 0; i < count - 1; ++i) {
    for (size_t j = 0; j < count - 1 - i; ++j) {
      char *a = (char*)array + j * size;
      char *b = (char*)array + (j + 1) * size;
      if (compare(a, b) > 0) {
        swap(a, b, size);
      }
    }
  }
}



