#include "test.h"

int main(void) {
  int failed = 0;
  Suite *s21_decimal_test[] = {test_negate(), test_fromint(),   test_mul(),
                               test_comp(),   test_truncate(),  test_add(),
                               test_sub(),    test_round(),     test_floor(),
                               test_div(),    test_fromfloat(), test_tofloat(),
                               NULL};

  for (int i = 0; s21_decimal_test[i] != NULL; i++) {
    SRunner *sr = srunner_create(s21_decimal_test[i]);

    srunner_set_fork_status(sr, CK_NOFORK);
    srunner_run_all(sr, CK_NORMAL);

    failed += srunner_ntests_failed(sr);
    srunner_free(sr);
  }
  printf("========= FAILED: %d =========\n", failed);

  return failed == 0 ? 0 : 1;
}
