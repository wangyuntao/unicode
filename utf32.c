
#include "unicode_internal.h"

bool utf32_is_well_formed(const uint32_t *p, size_t len) {
  while (--len >= 0) {
    if (!CP_IS_VALID(p[len])) {
      return false;
    }
  }
  return true;
}

bool utf32_append_uint8(utf32_str *s, const uint8_t *p, size_t len) {
  uint32_t cp;
  int l;

  while (len > 0) {
    if (!utf8_next_uint32_len(p, &l)) {
      return false;
    }

    if (l > len) {
      return false;
    }
    
    if (!utf8_to_uint32(p, l, &cp)) {
      return false;
    }
    
    utf32_append_uint32(s, cp);
    p += l;
    len -= l;
  }

  return true;
}

bool utf32_append_uint16(utf32_str *s, const uint16_t *p, size_t len) {
  uint16_t a, b;
  uint32_t cp;

  while (len > 0) {
    a = *p++;
    len--;

    if (!CP_IS_SURROGATE(a)) {
      utf32_append_uint32(s, (uint32_t)a);
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

    utf32_append_uint32(s, cp);
  }

  return true;
}

bool utf32_append_uint32(utf32_str *s, uint32_t cp) {
  uint32_t *p = utf32_add_len(s, 1);
  *p = cp;
  return true;
}
