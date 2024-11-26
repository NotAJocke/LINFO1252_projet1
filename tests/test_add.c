#include "adder.h"
#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>

#include <cmocka.h>

void test_adder(void **state) {
  (void)state;

  assert_int_equal(4, add(2, 2));
}
