
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
