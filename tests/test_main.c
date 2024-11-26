#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>

#include <cmocka.h>

static void test_true(void **state) {
  (void)state;

  assert_true(1);
}

int main() {
  const struct CMUnitTest tests[] = {
      cmocka_unit_test(test_true),
  };
  return cmocka_run_group_tests(tests, NULL, NULL);
}
