#include <stdio.h>

#include "aioc_defs.h"

aioc_error_t aioc_self_test(void);

int main(void) {
  aioc_self_test();
  return 0;
}