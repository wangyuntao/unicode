
#include "unicode_internal.h"

bool utf32_is_well_formed(const uint32_t *p, size_t len) {
  while (--len >= 0) {
    if (!CP_IS_VALID(p[len])) {
      return false;
    }
  }
  return true;
}

bool utf32_append_uint8(utf32 *s, const uint8_t *p, size_t len) {
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

bool utf32_append_uint16(utf32 *s, const uint16_t *p, size_t len) {
  uint32_t cp;
  int l;

  while (len > 0) {
    if (!utf16_next_uint32_len(p, &l)) {
      return false;
    }

    if (l > len) {
      return false;
    }

    if (!utf16_to_uint32(p, l, &cp)) {
      return false;
    }

    utf32_append_uint32(s, cp);
    p += l;
    len -= l;
  }

  return true;
}

bool utf32_append_uint32(utf32 *s, uint32_t cp) {
  uint32_t *p = utf32_add_len(s, 1);
  *p = cp;
  return true;
}
