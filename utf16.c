
#include "unicode_internal.h"

bool utf16_is_well_formed(const uint16_t *p, size_t len) {
  uint16_t a, b;

  while (len > 0) {
    a = *p++;
    len--;

    if (!CP_IS_SURROGATE(a)) {
      continue;
    }

    if (!CP_IS_HIGH_SURROGATE(a)) {
      return false;
    }

    if (len < 1) {
      return false;
    }

    b = *p++;
    len--;

    if (!CP_IS_LOW_SURROGATE(b)) {
      return false;
    }
  }

  return true;
}

bool utf16_next_uint32_len(const uint16_t *p, int *len) {
  uint16_t a = *p;

  if (!CP_IS_SURROGATE(a)) {
    *len = 1;
    return true;
  }

  if (!CP_IS_HIGH_SURROGATE(a)) {
    return false;
  }

  *len = 2;
  return true;
}

bool utf16_to_uint32(const uint16_t *p, int len, uint32_t *cp) {
  uint16_t a, b;

  if (len != 1 && len != 2) {
    return false;
  }

  a = *p++;

  if (len == 1) {
    if (CP_IS_SURROGATE(a)) {
      return false;
    }
    *cp = (uint32_t)a;
    return true;
  }

  if (!CP_IS_HIGH_SURROGATE(a)) {
    return false;
  }

  b = *p;
  if (!CP_IS_LOW_SURROGATE(b)) {
    return false;
  }

  a -= CP_MIN_HIGH_SURROGATE;
  b -= CP_MIN_LOW_SURROGATE;

  *cp = (a << 10) | b;
  *cp += CP_MIN_SUPPLEMENTARY;

  return true;
}

bool utf16_append_uint32(utf16_str *s, uint32_t cp) {
  uint16_t *p, a, b;

  if (cp < CP_MIN_SUPPLEMENTARY) {
    if (CP_IS_SURROGATE(cp)) {
      return false;
    }

    p = utf16_add_len(s, 1);
    *p = (uint16_t)cp;
    return true;
  }

  if (cp < CP_MAX) {
    cp -= CP_MIN_SUPPLEMENTARY;

    b = cp & 0x3FF;
    a = (cp >> 10) & 0x3FF;

    a += CP_MIN_HIGH_SURROGATE;
    b += CP_MIN_LOW_SURROGATE;

    assert(CP_IS_HIGH_SURROGATE(a));
    assert(CP_IS_LOW_SURROGATE(b));

    p = utf16_add_len(s, 2);
    p[0] = a;
    p[1] = b;

    return true;
  }

  return false;
}
