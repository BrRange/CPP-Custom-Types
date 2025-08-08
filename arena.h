#ifndef ARENAH
#define ARENAH

#include <stdlib.h>
#include "rustydef.h"

struct Arena{
  u8 *mem;
  i64 cur, end;
};
typedef struct Arena Arena;

void arenaNew(Arena *ar, i64 bytes){
  if(bytes <= 0) return;
  ar->mem = malloc(bytes);
  ar->cur = 0;
  ar->end = bytes;
}

void *arenaAlloc(Arena *ar, i64 bytes){
  if(ar->cur > ar->end - bytes) return NULL;
  void *mem = ar->mem + ar->cur;
  ar->cur += bytes;
  return mem;
}

void arenaPop(Arena *ar, i64 bytes){
  ar->cur -= bytes < 0 ? ar->cur : bytes;
  ar->cur < 0 ? ar->cur = 0 : 0;
}

void arenaDestroy(Arena *ar){
  free(ar->mem);
  *ar = (Arena){};
}

struct Arena4{
  u32 *mem;
  i64 cur, end;
};
typedef struct Arena4 Arena4;

void arena4New(Arena4 *ar, i64 bytes){
  if(bytes <= 0) return;
  bytes += -bytes & 0b11;
  ar->mem = malloc(bytes);
  ar->cur = 0;
  ar->end = bytes;
}

void *arena4Alloc(Arena4 *ar, i64 bytes){
  bytes += -bytes & 0b11;
  if(ar->cur > ar->end - bytes) return NULL;
  void *mem = ar->mem + ar->cur / 4;
  ar->cur += bytes;
  return mem;
}

void arena4Pop(Arena4 *ar, i64 bytes){
  ar->cur -= bytes < 0 ? ar->cur : bytes + (-bytes & 0b11);
  ar->cur < 0 ? ar->cur = 0 : 0;
}

void arena4Destroy(Arena4 *ar){
  free(ar->mem);
  *ar = (Arena4){};
}

#endif