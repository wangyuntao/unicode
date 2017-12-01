
#ifndef __UNICODE_INTERNAL_H
#define __UNICODE_INTERNAL_H

#include <assert.h>
#include <stdlib.h>
#include "unicode.h"

#define CP_MIN 0x0000
#define CP_MAX 0x10FFFF
#define CP_MIN_SUPPLEMENTARY 0x10000

#define CP_MIN_HIGH_SURROGATE 0xD800
#define CP_MAX_HIGH_SURROGATE 0xDBFF
#define CP_MIN_LOW_SURROGATE 0xDC00
#define CP_MAX_LOW_SURROGATE 0xDFFF

#define CP_IS_HIGH_SURROGATE(cp) \
  ((cp) >= CP_MIN_HIGH_SURROGATE && (cp) <= CP_MAX_HIGH_SURROGATE)

#define CP_IS_LOW_SURROGATE(cp) \
  ((cp) >= CP_MIN_LOW_SURROGATE && (cp) <= CP_MAX_LOW_SURROGATE)

#define CP_IS_SURROGATE(cp) \
  ((cp) >= CP_MIN_HIGH_SURROGATE && (cp) <= CP_MAX_LOW_SURROGATE)

#define CP_IS_VALID(cp) \
  ((cp) >= CP_MIN && (cp) <= CP_MAX && !CP_IS_SURROGATE(cp))

#define MASK_LOW(n) ((((uint32_t)1) << (n)) - 1)
#define MASK_SHIFT(n, m) (MASK_LOW(n) << (m))
#define MASK_HIGH(n, t) (MASK_SHIFT(n, (t) - (n)))

void *u_calloc(size_t nitems, size_t size);
void *u_malloc(size_t size);
void *u_realloc(void *p, size_t size);
void u_free(void *p);

uint8_t *utf8_add_len(utf8_str *s, size_t add_len);
uint16_t *utf16_add_len(utf16_str *s, size_t add_len);
uint32_t *utf32_add_len(utf32_str *s, size_t add_len);

bool get_num_utf8_units_cp_requires(uint32_t cp, uint8_t *n);
bool get_num_utf16_units_cp_requires(uint32_t cp, uint8_t *n);

#endif


// TODO *_str -> *, append const
