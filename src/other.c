#include "s21_decimal.h"

int s21_negate(s21_decimal value, s21_decimal *result) {
  int res = 0;

  *result = value;
  if (get_sign(value) == 0) {
    set_bit(result, 127, 1);
  } else if (get_sign(value) == 1) {
    set_bit(result, 127, 0);
  }

  return res;
}

int s21_truncate(s21_decimal value, s21_decimal *result) {
  zero_decimal(result);
  s21_big_decimal val = {{0, 0, 0, 0, 0, 0, 0, 0}};
  s21_big_decimal rem = {{0, 0, 0, 0, 0, 0, 0, 0}};
  s21_big_decimal ten = {{10, 0, 0, 0, 0, 0, 0}};
  s21_big_decimal tmp_val = {{0, 0, 0, 0, 0, 0, 0, 0}};
  import_to_big(value, &val);
  int scale = get_scale(value);
  while (scale) {
    zero_big_decimal(&rem);
    rem = s21_big_div(val, ten, &tmp_val);
    val = tmp_val;
    zero_big_decimal(&tmp_val);
    scale--;
  }
  import_into_normal(result, val);
  set_scale(result, scale);
  if (get_sign(value)) {
    set_bit(result, 127, 1);
  }
  return 0;
}

int s21_round(s21_decimal value, s21_decimal *result) {
  int error = 0;
  s21_big_decimal value_b = {{0, 0, 0, 0, 0, 0, 0, 0}};
  s21_big_decimal result_b = {{0, 0, 0, 0, 0, 0, 0, 0}};
  int sign = get_bit(value, 127);
  import_to_big(value, &value_b);
  import_to_big(*result, &result_b);
  zero_big_decimal(&result_b);
  s21_big_decimal div = {{10, 0, 0, 0, 0, 0, 0, 0}};
  s21_big_decimal res = {{0, 0, 0, 0, 0, 0, 0, 0}};
  s21_big_decimal add = {{1, 0, 0, 0, 0, 0, 0, 0}};
  int scale = big_get_scale(value_b);
  while (scale) {
    res = s21_big_div(value_b, div, &value_b);
    scale--;
    if (scale == 0) {
      if (res.bits[0] >= 5) {
        s21_add_big_calculation(value_b, add, &value_b);
      }
    }
  }
  import_into_normal(result, value_b);
  if (sign == 1) {
    set_bit(result, 127, 1);
  }
  return error;
}

int s21_floor(s21_decimal value, s21_decimal *result) {
  int error = 0;
  s21_big_decimal value_b = {{0, 0, 0, 0, 0, 0, 0, 0}};
  s21_big_decimal result_b = {{0, 0, 0, 0, 0, 0, 0, 0}};
  int sign = get_bit(value, 127);
  import_to_big(value, &value_b);
  import_to_big(*result, &result_b);
  zero_big_decimal(&result_b);
  s21_big_decimal div = {{10, 0, 0, 0, 0, 0, 0, 0}};
  s21_big_decimal add = {{1, 0, 0, 0, 0, 0, 0, 0}};
  int scale = big_get_scale(value_b);
  if (sign == 1) {
    while (scale) {
      s21_big_div(value_b, div, &value_b);
      scale--;
      if (scale == 0) {
        s21_add_big_calculation(value_b, add, &value_b);
      }
    }
  } else {
    trancate_big(value_b, &value_b);
  }
  import_into_normal(result, value_b);
  if (sign == 1) {
    set_bit(result, 127, 1);
  }
  return error;
}