
#include "unicode_internal.h"

bool utf8_is_well_formed(const uint8_t *p, size_t len) {
  uint8_t b, b2;

  if (p == NULL) {
    return false;
  }

  while (len > 0) {
    b = *p++;
    len--;

    // One-byte form
    if (b <= 0x7F) {
      continue;
    }

    // Illegal form
    if (b < 0xC2) {
      return false;
    }

    // Two-byte form
    if (b <= 0xDF) {
      if (len < 1) {
        return false;
      }

      b = *p++;
      len--;

      if (b < 0x80 || b > 0xBF) {
        return false;
      }
      continue;
    }

    // Three-byte form
    if (b <= 0xEF) {
      if (len < 2) {
        return false;
      }

      b2 = *p++;
      len--;

      if (b == 0xE0) {
        if (b2 < 0xA0 || b2 > 0xBF) {
          return false;
        }
      } else if (b == 0xED) {
        if (b2 < 0x80 || b2 > 0x9F) {
          return false;
        }
      } else {
        if (b2 < 0x80 || b2 > 0xBF) {
          return false;
        }
      }

      b = *p++;
      len--;

      if (b < 0x80 || b > 0xBF) {
        return false;
      }
      continue;
    }

    // Four-byte form
    if (b <= 0xF4) {
      if (len < 3) {
        return false;
      }

      b2 = *p++;
      len--;

      if (b == 0xF0) {
        if (b2 < 0x90 || b2 > 0xBF) {
          return false;
        }
      } else if (b == 0xF4) {
        if (b2 < 0x80 || b2 > 0x8F) {
          return false;
        }
      } else {
        if (b2 < 0x80 || b2 > 0xBF) {
          return false;
        }
      }

      b = *p++;
      len--;

      if (b < 0x80 || b > 0xBF) {
        return false;
      }

      b = *p++;
      len--;

      if (b < 0x80 || b > 0xBF) {
        return false;
      }
      continue;
    }

    // Illegal form
    return false;
  }

  return true;
}

bool utf8_append_uint32(utf8_str *s, uint32_t cp) {
  uint8_t *p, b, l, i;

  if (!get_num_utf8_units_cp_requires(cp, &l)) {
    return false;
  }

  p = utf8_add_len(s, l);

  if (l == 1) {
    p[0] = (uint8_t)cp;
    return true;
  }

  for (i = l; --i >= 1;) {
    b = (cp & 0x3F) | 0x80;
    p[i] = b;
    cp >>= 6;
  }

  b = MASK_HIGH(l, 8);
  b |= cp & MASK_LOW(7 - l);
  p[0] = b;

  return true;
}
