
#include "unicode_internal.h"

bool utf32_is_well_formed(const uint32_t *p, size_t len) {
  while (--len >= 0) {
    if (!CP_IS_VALID(p[len])) {
      return false;
    }
  }
  return true;
}

bool utf32_append_uint8(const utf32_str *s, const uint8_t *p, size_t len) {
  uint32_t cp;
  uint8_t b, l, i;

  while (len > 0) {
    b = *p++;
    len--;

    if (b <= 0x7F) {  // 0xxxxxxx
      utf32_append_uint32(s, (uint32_t)b);
      continue;
    }

    if ((b & 0xC0) == 0x80) {  // 10xxxxxx
      return false;
    }

    if ((b & 0xE0) == 0xC0) {  // 110xxxxx
      l = 2;
    } else if ((b & 0xF0) == 0xE0) {  // 1110xxxx
      l = 3;
    } else if ((b & 0xF8) == 0xF0) {  // 11110xxx
      l = 4;
    } else {
      return false;
    }

    if (l - 1 > len) {
      return false;
    }

    cp = b & MASK_LOW(7 - l);
    for (i = 1; i < l; i++) {
      b = *p++;
      len--;

      if ((b & 0xC0) != 0x80) {
        return false;
      }

      cp <<= 6;
      cp |= b & 0x3F;
    }

    if (!get_num_utf8_units_cp_requires(cp, &b)) {
      return false;
    }

    if (l != b) {  // overlong
      return false;
    }

    utf32_append_uint32(s, cp);
  }

  return true;
}

bool utf32_append_uint16(const utf32_str *s, const uint16_t *p, size_t len) {
  uint16_t a, b;
  uint32_t cp;

  while (len > 0) {
    a = *p++;
    len--;

    if (!CP_IS_SURROGATE(a)) {
      utf32_append_uint32((uint32_t)a);
      continue;
    }

    if (!CP_IS_HIGH_SURROGATE(a)) {
      return false;
    }

    b = *p++;
    len--;

    if (!CP_IS_LOW_SURROGATE(b)) {
      return false;
    }

    a -= CP_MIN_HIGH_SURROGATE;
    b -= CP_MIN_LOW_SURROGATE;

    cp = (a << 10) | b;
    cp += CP_MIN_SUPPLEMENTARY;

    utf32_append_uint32(cp);
  }

  return true;
}

bool utf32_append_uint32(const utf32_str *s, uint32_t cp) {
  uint32_t *p = utf32_add_len(s, 1);
  *p = cp;
  return true;
}
