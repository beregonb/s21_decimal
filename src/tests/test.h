#ifndef SRC_TESTS_ME_H
#define SRC_TESTS_ME_H

#include <check.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

#include "../s21_decimal.h"

Suite* test_negate(void);
Suite* test_fromint(void);
Suite* test_mul(void);
Suite* test_comp(void);
Suite* test_truncate(void);
Suite* test_add(void);
Suite* test_sub(void);
Suite* test_round(void);
Suite* test_floor(void);
Suite* test_div(void);
Suite* test_fromfloat(void);
Suite* test_tofloat(void);
#endif  // SRC_TESTS_ME_H
