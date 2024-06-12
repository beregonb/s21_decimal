#include "s21_decimal.h"

int get_exp_from_string(const char *src, int *exp) {
  int error = 0, breaker = 1;
  char *ptrCopy = (char *)src;
  while (*ptrCopy && breaker) {
    if (*ptrCopy == 'E') {
      ++ptrCopy;
      *exp = strtol(ptrCopy, NULL, 10);
      breaker = 0;
    }
    ++ptrCopy;
  }
  if (abs(*exp) > 28) {
    error = 1;
  }
  return error;
}

s21_big_decimal cTd(const char num) {
  s21_big_decimal res = {{0, 0, 0, 0, 0, 0, 0}};
  if (num == '1') {
    res.bits[0] = 1;
  } else if (num == '2') {
    res.bits[0] = 2;
  } else if (num == '3') {
    res.bits[0] = 3;
  } else if (num == '4') {
    res.bits[0] = 4;
  } else if (num == '5') {
    res.bits[0] = 5;
  } else if (num == '6') {
    res.bits[0] = 6;
  } else if (num == '7') {
    res.bits[0] = 7;
  } else if (num == '8') {
    res.bits[0] = 8;
  } else if (num == '9') {
    res.bits[0] = 9;
  }
  return res;
}

void correctExp(char *str, int exp) {
  char exp4Change[4];
  sprintf(exp4Change, "%d", exp);
  int breaker = 1;
  while (*str && breaker) {
    if (*str == 'E') {
      str += 2;
      for (size_t i = 0; i < strlen(exp4Change); ++i) *str = exp4Change[i];
      breaker = 0;
    }
    ++str;
  }
}

void pow_ten_b(s21_big_decimal *src, int exp) {
  while (exp > 0) {
    costil(src);
    --exp;
  }
}

s21_decimal strToDec(const char *src, int correct_flag) {
  s21_big_decimal labor;
  zero_big_decimal(&labor);
  char *ptr = (char *)src;
  int corr_exp = 0;
  get_exp_from_string(src, &corr_exp);
  if (correct_flag) {
    corr_exp += 28;
  }
  int numberDigit = correct_flag ? corr_exp : 6;
  while (*ptr && numberDigit >= 0) {
    if (*ptr <= '9' && *ptr >= '0') {
      s21_big_decimal tmp = cTd(*ptr);
      pow_ten_b(&tmp, numberDigit);
      --numberDigit;
      s21_add_big_calculation(labor, tmp, &labor);
      ++ptr;
    } else if (*ptr == '.') {
      ++ptr;
      continue;
    }
  }
  if (*ptr >= '5' && correct_flag) {
    s21_big_decimal add = {{1, 0, 0, 0, 0, 0, 0, 0}};
    s21_add_big_calculation(labor, add, &labor);
  }

  int exp = 0;
  get_exp_from_string(src, &exp);
  if (abs(exp) == 28) {
    ptr++;
    if (*ptr >= '5') {
      s21_big_decimal add = {{1, 0, 0, 0, 0, 0, 0, 0}};
      s21_add_big_calculation(labor, add, &labor);
    }
  }
  if (exp < 0 && !correct_flag) {
    exp *= -1;
    exp += 6;
  }
  if (correct_flag) {
    exp = exp * -1 + corr_exp;
  }
  if (exp < 6 && exp != 0) {
    exp = 6 - exp;
  }
  if (exp == 6) {
    exp = 0;
  } else if (exp == 0) {
    exp = 6;
  }

  s21_decimal dst;
  zero_decimal(&dst);
  big_set_scale(&labor, exp);
  import_into_normal(&dst, labor);
  return dst;
}

int s21_from_float_to_decimal(float src, s21_decimal *dst) {
  int error = 0;
  char value[64] = {"\n"};
  int sign = abs((*(int *)&(src)) >> 31);
  if (sign) {
    src *= -1;
  }
  sprintf(value, "%.6E", src);
  int exp = 0;
  int correct_flag = 0;
  error = get_exp_from_string(value, &exp);
  if (!error) {
    if (exp <= -23) {
      correct_flag = 1;
    }
    *dst = strToDec(value, correct_flag);
    set_sign(dst, sign);
  }
  return error;
}

int s21_from_decimal_to_float(s21_decimal src, float *dst) {
  int error = 0;
  s21_decimal zero;
  zero_decimal(&zero);
  if (!dst) {
    error = 1;
  } else if (s21_is_equal(src, zero)) {
    int sign = get_sign(src);
    if (sign) {
      *dst = -0.0;
    } else {
      *dst = 0.0;
    }
  } else {
    *dst = 0.0;
    double temp = 0.0;
    int sign = get_sign(src);
    int scale = get_scale(src);
    for (int i = 0; i < 96; ++i) {
      temp += get_bit(src, i) * pow(2.0, i);
    }
    double ten = pow(10.0, -scale);
    temp *= ten;
    *dst = (float)temp;
    if (sign) {
      *dst *= -1;
    }
  }
  return error;
}