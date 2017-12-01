# Unicode

A C library for converting between Unicode encoding forms (i.e. UTF-8, UTF-16 and UTF-32), and manipulating Unicode strings.

### Examples

```C
#include "unicode.h"
#include <assert.h>
#include <inttypes.h>
#include <stdio.h>

static void utf8_code_point_iterate(utf8 *s) {
  uint8_t *p;
  size_t l1;
  int l2;
  bool b;
  uint32_t cp;

  p = s->p;
  l1 = s->len;

  while (true) {
    b = utf8_next_uint32_len(p, &l2);
    assert(b);
    assert(l2 <= l1);

    b = utf8_to_uint32(p, l2, &cp);
    assert(b);

    printf("%.*s -> %#x\n", l2, p, cp);

    p += l2;
    l1 -= l2;

    assert(l1 >= 0);
    if (l1 == 0) {
      break;
    }
  }
}

int main(int argc, char *argv[]) {
  // Hello, 世界
  uint32_t p[] = {0x48, 0x65, 0x6C, 0x6C, 0x6F, 0x2C, 0x20, 0x4E16, 0x754C};
  size_t len = sizeof(p) / sizeof(p[0]);

  // Unicode code points -> utf8
  utf8 *s = utf32_to_utf8(p, len);
  assert(s);

  assert(utf8_is_well_formed(s->p, s->len));
  printf("%.*s\n", (int)s->len, s->p);

  // utf8 -> utf32
  utf32 *s2 = utf8_to_utf32(s->p, s->len);
  assert(s2);
  assert(s2->len == len);

  for (int i = 0; i < len; i++) {
    assert(s2->p[i] == p[i]);
  }

  // utf8 code point iterate
  utf8_code_point_iterate(s);

  // free
  utf32_free(s2);
  utf8_free(s);
  return 0;
}
```

Output:

```
Hello, 世界
H -> 0x48
e -> 0x65
l -> 0x6c
l -> 0x6c
o -> 0x6f
, -> 0x2c
  -> 0x20
世 -> 0x4e16
界 -> 0x754c
```

### APIs

```C
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
```

### Note

For more examples, see [unicode_example.c](unicode_example.c)

For more APIs, see [unicode.h](unicode.h)
