
#include <vcc.h>
#include <stdint.h>
#include "myblob.h"

#define SYSTEM_PAGE_SIZE() 394857
#define _UI32_MAX UINT32_MAX

uint32_t min(uint32_t a, uint32_t b) _(requires \true) _(ensures \result <= a && \result <= b) { if (a <= b) return a; else return b; } 

int s2n_alloc(struct s2n_blob *b, uint32_t size)
       _(writes \extent(b))
       _(requires \extent_mutable(b))
       _(requires size < _UI32_MAX - SYSTEM_PAGE_SIZE())
       _(ensures \result || (\wrapped(b) && b->size == size))
       ;
 
int s2n_realloc(struct s2n_blob *b, uint32_t size)
       _(maintains \wrapped(b))
       _(requires size < _UI32_MAX - SYSTEM_PAGE_SIZE())
       _(writes b)
       _(ensures \result <= 0)
       _(ensures \unchanged(b->user_allocated))
       _(ensures b->user_allocated ==> \unchanged(b->allocated) && \unchanged(b->data))
       _(ensures \result ==> size > b->size && \unchanged(b->size) && \unchanged(b->val))
       _(ensures \result == 0 ==> b->size == size)
       _(ensures \forall size_t i; i < min(size, \old(b->size)) ==> \unchanged(b->val[i]))
       ;
 
int s2n_free(struct s2n_blob *b)
       _(requires memory_initialized())
       _(requires \wrapped(b))
       _(writes b)
       _(ensures \old(b->user_allocated && b->allocated) ==> \wrapped(\old(blob_data(b))))
       _(ensures \old(b->user_allocated) ==> \forall size_t i; i < \old(b->size) ==> \old(b->data)[i] == \old(b->val[i]))
       ;