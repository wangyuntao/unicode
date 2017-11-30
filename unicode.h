
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
} utf8_str;

typedef struct {
  uint16_t *p;
  size_t len;
  size_t cap;
} utf16_str;

typedef struct {
  uint32_t *p;
  size_t len;
  size_t cap;
} utf32_str;

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
utf8_str *utf8_new(size_t cap);
utf16_str *utf16_new(size_t cap);
utf32_str *utf32_new(size_t cap);

void utf8_free(utf8_str *s);
void utf16_free(utf16_str *s);
void utf32_free(utf32_str *s);

// convert from one encoding form to another
utf16_str *utf8_to_utf16(const uint8_t *p, size_t len);
utf32_str *utf8_to_utf32(const uint8_t *p, size_t len);

utf8_str *utf16_to_utf8(const uint16_t *p, size_t len);
utf32_str *utf16_to_utf32(const uint16_t *p, size_t len);

utf8_str *utf32_to_utf8(const uint32_t *p, size_t len);
utf16_str *utf32_to_utf16(const uint32_t *p, size_t len);

// utf8 utils
bool utf8_is_well_formed(const uint8_t *p, size_t len);
bool utf8_append_uint32(const utf8_str *s, uint32_t cp);

// utf16 utils
bool utf16_is_well_formed(const uint16_t *p, size_t len);
bool utf16_append_uint32(const utf16_str *s, uint32_t cp);

// utf32 utils
bool utf32_is_well_formed(const uint32_t *p, size_t len);
bool utf32_append_uint8(const utf32_str *s, const uint8_t *p, size_t len);
bool utf32_append_uint16(const utf32_str *s, const uint16_t *p, size_t len);
bool utf32_append_uint32(const utf32_str *s, uint32_t cp);

// other utils


#endif
