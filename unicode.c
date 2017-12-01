
#include "unicode_internal.h"

void *u_calloc(size_t nitems, size_t size) {
  void *p = calloc(nitems, size);
  assert(p);
  return p;
}

void *u_malloc(size_t size) {
  void *p = malloc(size);
  assert(p);
  return p;
}

void *u_realloc(void *p, size_t size) {
  p = realloc(p, size);
  assert(p);
  return p;
}

void u_free(void *p) { free(p); }

utf8_str *utf8_new(size_t cap) {
  utf8_str *s = u_malloc(sizeof(utf8_str));
  s->p = u_malloc(sizeof(uint8_t) * cap);
  s->len = 0;
  s->cap = cap;
  return s;
}

utf16_str *utf16_new(size_t cap) {
  utf16_str *s = u_malloc(sizeof(utf16_str));
  s->p = u_malloc(sizeof(uint16_t) * cap);
  s->len = 0;
  s->cap = cap;
  return s;
}

utf32_str *utf32_new(size_t cap) {
  utf32_str *s = u_malloc(sizeof(utf32_str));
  s->p = u_malloc(sizeof(uint32_t) * cap);
  s->len = 0;
  s->cap = cap;
  return s;
}

void utf8_free(utf8_str *s) {
  u_free(s->p);
  u_free(s);
}

void utf16_free(utf16_str *s) {
  u_free(s->p);
  u_free(s);
}

void utf32_free(utf32_str *s) {
  u_free(s->p);
  u_free(s);
}

uint8_t *utf8_add_len(utf8_str *s, size_t add_len) {
  size_t len, cap;

  len = s->len + add_len;
  assert(len >= s->len);

  cap = s->cap;
  if (cap < len) {
    cap = s->cap * 2;
    if (cap < len) {
      cap = len;
    }

    assert(cap >= s->cap);
    assert(cap >= len);

    s->p = u_realloc(s->p, sizeof(uint8_t) * cap);
    s->cap = cap;
  }

  s->len = len;
  return s->p + (len - add_len);
}

uint16_t *utf16_add_len(utf16_str *s, size_t add_len) {
  size_t len, cap;

  len = s->len + add_len;
  assert(len >= s->len);

  cap = s->cap;
  if (cap < len) {
    cap = s->cap * 2;
    if (cap < len) {
      cap = len;
    }

    assert(cap >= s->cap);
    assert(cap >= len);

    s->p = u_realloc(s->p, sizeof(uint16_t) * cap);
    s->cap = cap;
  }

  s->len = len;
  return s->p + (len - add_len);
}

uint32_t *utf32_add_len(utf32_str *s, size_t add_len) {
  size_t len, cap;

  len = s->len + add_len;
  assert(len >= s->len);

  cap = s->cap;
  if (cap < len) {
    cap = s->cap * 2;
    if (cap < len) {
      cap = len;
    }

    assert(cap >= s->cap);
    assert(cap >= len);

    s->p = u_realloc(s->p, sizeof(uint32_t) * cap);
    s->cap = cap;
  }

  s->len = len;
  return s->p + (len - add_len);
}

utf16_str *utf8_to_utf16(const uint8_t *p, size_t len) {
  utf32_str *s = utf8_to_utf32(p, len);
  if (s == NULL) return NULL;

  utf16_str *s2 = utf32_to_utf16(s->p, s->len);
  utf32_free(s);

  if (s2 == NULL) return NULL;
  return s2;
}

utf32_str *utf8_to_utf32(const uint8_t *p, size_t len) {
  utf32_str *s = utf32_new(len / 2);
  if (!utf32_append_uint8(s, p, len)) {
    utf32_free(s);
    return NULL;
  }
  return s;
}

utf8_str *utf16_to_utf8(const uint16_t *p, size_t len) {
  utf32_str *s = utf16_to_utf32(p, len);
  if (s == NULL) return NULL;

  utf8_str *s2 = utf32_to_utf8(s->p, s->len);
  utf32_free(s);

  if (s2 == NULL) return NULL;
  return s2;
}

utf32_str *utf16_to_utf32(const uint16_t *p, size_t len) {
  utf32_str *s = utf32_new(len / 2);
  if (!utf32_append_uint16(s, p, len)) {
    utf32_free(s);
    return NULL;
  }
  return s;
}

utf8_str *utf32_to_utf8(const uint32_t *p, size_t len) {
  utf8_str *s;
  size_t i;

  s = utf8_new(len);
  for (i = 0; i < len; i++) {
    if (!utf8_append_uint32(s, p[i])) {
      utf8_free(s);
      return NULL;
    }
  }
  return s;
}

utf16_str *utf32_to_utf16(const uint32_t *p, size_t len) {
  utf16_str *s;
  size_t i;

  s = utf16_new(len);
  for (i = 0; i < len; i++) {
    if (!utf16_append_uint32(s, p[i])) {
      utf16_free(s);
      return NULL;
    }
  }
  return s;
}

bool get_num_utf8_units_cp_requires(uint32_t cp, uint8_t *n) {
  if (cp <= 0x7F) {
    *n = 1;
    return true;
  }

  if (cp <= 0x7FF) {
    *n = 2;
    return true;
  }

  if (cp <= 0xFFFF) {
    if (CP_IS_SURROGATE(cp)) {
      return false;
    }
    *n = 3;
    return true;
  }

  if (cp <= CP_MAX) {
    *n = 4;
    return true;
  }

  return false;
}

bool get_num_utf16_units_cp_requires(uint32_t cp, uint8_t *n) {
  if (cp < CP_MIN_SUPPLEMENTARY) {
    if (CP_IS_SURROGATE(cp)) {
      return false;
    }
    *n = 1;
    return true;
  }

  if (cp <= CP_MAX) {
    *n = 2;
    return true;
  }

  return false;
}
