
#include "unicode.h"
#include <assert.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
  // Hello, 世界
  uint32_t a[] = {0x48, 0x65, 0x6C, 0x6C, 0x6F, 0x2C, 0x20, 0x4E16, 0x754C};
  int len = sizeof(a) / sizeof(a[0]);

  // Unicode code points -> utf8
  utf8_str *s = utf32_to_utf8(a, len);
  assert(s);

  assert(utf8_is_well_formed(s->p, s->len));
  printf("%.*s\n", (int)s->len, s->p);

  // utf8 -> utf32
  utf32_str *s2 = utf8_to_utf32(s->p, s->len);
  assert(s2);
  assert(s2->len == len);

  for (int i = 0; i < len; i++) {
    assert(s2->p[i] == a[i]);
  }

  // free
  utf32_free(s2);
  utf8_free(s);
  return 0;
}
