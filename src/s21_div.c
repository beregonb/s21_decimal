#include "s21_decimal.h"

s21_big_decimal decimal_ten(void) {
  s21_big_decimal ten = {{10, 0, 0, 0, 0, 0, 0}};
  return ten;
}

int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int error = 0;
  s21_decimal zero = {{0, 0, 0, 0}};
  int sign_1 = get_sign(value_1);
  int sign_2 = get_sign(value_2);
  if (sign_1) {
    s21_negate(value_1, &value_1);
  }
  if (sign_2) {
    s21_negate(value_2, &value_2);
  }
  if (s21_is_equal(value_2, zero)) {
    error = 3;
  } else if (s21_is_greater_or_equal(value_1, value_2)) {
    s21_big_decimal copy_value_1;
    s21_big_decimal copy_value_2;
    s21_big_decimal res_tmp;
    zero_big_decimal(&copy_value_1);
    zero_big_decimal(&copy_value_2);
    zero_big_decimal(&res_tmp);
    import_to_big(value_1, &copy_value_1);
    import_to_big(value_2, &copy_value_2);
    normalization_B(&copy_value_1, &copy_value_2);
    s21_big_decimal mem;
    zero_big_decimal(&mem);

    mem = s21_big_div(copy_value_1, copy_value_2, &res_tmp);
    if (res_tmp.bits[3] || res_tmp.bits[4] || res_tmp.bits[5]) {
      if (sign_1 != sign_2) {
        error = 2;
      } else {
        error = 1;
      }
      s21_decimal inf = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x7FFFFFFF}};
      *result = inf;
    } else {
      error = support_for_div(res_tmp, mem, copy_value_2, result);
      if (sign_1 != sign_2) {
        set_sign(result, 1);
      }
      if (get_sign(*result) == 1 && error == 1) {
        error = 2;
      }
      if (error == 0 && !s21_is_equal(value_1, zero) &&
          s21_is_equal(*result, zero)) {
        error = 2;
      }
    }
  } else if (s21_is_less(value_1, value_2)) {
    s21_big_decimal copy_value_2;
    s21_big_decimal res_tmp;
    s21_big_decimal mem;
    zero_big_decimal(&mem);
    zero_big_decimal(&copy_value_2);
    zero_big_decimal(&res_tmp);
    import_to_big(value_1, &mem);
    import_to_big(value_2, &copy_value_2);
    normalization_B(&mem, &copy_value_2);
    error = support_for_div(res_tmp, mem, copy_value_2, result);
    if (sign_1 != sign_2) {
      set_sign(result, 1);
    }
    if (get_sign(*result) == 1 && error == 1) {
      error = 2;
    }
    if (error == 0 && !s21_is_equal(value_1, zero) &&
        s21_is_equal(*result, zero)) {
      error = 2;
    }
  }
  return error;
}

/*worked with d*/
s21_big_decimal s21_big_div(s21_big_decimal value_1, s21_big_decimal value_2,
                            s21_big_decimal *result) {
  s21_big_decimal subRes = {{0, 0, 0, 0, 0, 0, 0}};
  s21_big_decimal tmpRes = {{0, 0, 0, 0, 0, 0, 0}};
  s21_big_decimal mem;
  zero_big_decimal(&mem);
  zero_big_decimal(result);
  big_set_scale(&value_2, 0);
  for (int i = big_highets_bit(value_1); i >= 0; --i) {
    shift_left_big(&mem, 1);
    if (big_get_bit(value_1, i)) {
      big_set_bit(&mem, 0, 1);
    }
    if (s21_big_is_greater_or_equal(mem, value_2)) {
      zero_big_decimal(&subRes);
      s21_sub_big_culculation(mem, value_2, &subRes);
      mem = subRes;
      big_set_bit(&tmpRes, 0, 1);
    }
    shift_left_big(&tmpRes, 1);
  }
  shift_right_big(&tmpRes, 1);
  *result = tmpRes;
  return mem;
}

int remainder_calc(s21_big_decimal *res, s21_big_decimal value_2,
                   s21_big_decimal *mem) {
  int scale = 0;
  s21_big_decimal zero = {{0, 0, 0, 0, 0, 0, 0}};
  s21_big_decimal whole = *res;
  s21_big_decimal mem_tmp = *mem;
  s21_big_decimal temp;
  zero_big_decimal(&temp);
  big_set_scale(&mem_tmp, 0);
  while (scale < 28 && !s21_big_is_equal(mem_tmp, zero)) {
    costil(&whole);
    costil(&mem_tmp);
    mem_tmp = s21_big_div(mem_tmp, value_2, &temp);
    s21_add_big_calculation(whole, temp, &whole);
    scale++;
  }

  *res = whole;
  *mem = mem_tmp;
  return scale;
}

int support_for_div(s21_big_decimal res, s21_big_decimal mem,
                    s21_big_decimal value_2, s21_decimal *result) {
  int code = 0;
  int scale = remainder_calc(&res, value_2, &mem);
  s21_big_decimal tmp_res;
  zero_big_decimal(&tmp_res);
  big_set_scale(&res, scale);
  rounding(res, &tmp_res);
  overflow(tmp_res, &code);
  if (!code) {
    import_into_normal(result, tmp_res);
  }
  return code;
}