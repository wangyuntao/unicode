
#ifndef __UNICODE_H
#define __UNICODE_H

/*
 * References:
 *
 * https://en.wikipedia.org/wiki/UTF-8
 * https://en.wikipedia.org/wiki/UTF-16
 * https://en.wikipedia.org/wiki/UTF-32
 * http://www.unicode.org/versions/Unicode10.0.0/UnicodeStandard-10.0.pdf
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef struct {
  uint8_t *p;
  size_t len;
  size_t cap;
} utf8;

typedef struct {
  uint16_t *p;
  size_t len;
  size_t cap;
} utf16;

typedef struct {
  uint32_t *p;
  size_t len;
  size_t cap;
} utf32;

enum encoding_scheme {
  UTF_8,
  UTF_16BE,
  UTF_16LE,
  UTF_32BE,
  UTF_32LE,
};

typedef struct {
  uint8_t *p;
  size_t len;
  enum encoding_scheme scheme;
} utf_bytes;

// new & free
utf8 *utf8_new(size_t cap);
utf16 *utf16_new(size_t cap);
utf32 *utf32_new(size_t cap);

void utf8_free(utf8 *s);
void utf16_free(utf16 *s);
void utf32_free(utf32 *s);

// convert from one encoding form to another
utf16 *utf8_to_utf16(const uint8_t *p, size_t len);
utf32 *utf8_to_utf32(const uint8_t *p, size_t len);

utf8 *utf16_to_utf8(const uint16_t *p, size_t len);
utf32 *utf16_to_utf32(const uint16_t *p, size_t len);

utf8 *utf32_to_utf8(const uint32_t *p, size_t len);
utf16 *utf32_to_utf16(const uint32_t *p, size_t len);

// utf8 utils
bool utf8_is_well_formed(const uint8_t *p, size_t len);
bool utf8_next_uint32_len(const uint8_t *p, int *len);
bool utf8_to_uint32(const uint8_t *p, int len, uint32_t *cp);
bool utf8_append_uint32(utf8 *s, uint32_t cp);

// utf16 utils
bool utf16_is_well_formed(const uint16_t *p, size_t len);
bool utf16_next_uint32_len(const uint16_t *p, int *len);
bool utf16_to_uint32(const uint16_t *p, int len, uint32_t *cp);
bool utf16_append_uint32(utf16 *s, uint32_t cp);

// utf32 utils
bool utf32_is_well_formed(const uint32_t *p, size_t len);
bool utf32_append_uint8(utf32 *s, const uint8_t *p, size_t len);
bool utf32_append_uint16(utf32 *s, const uint16_t *p, size_t len);
bool utf32_append_uint32(utf32 *s, uint32_t cp);

// other utils

#endif
