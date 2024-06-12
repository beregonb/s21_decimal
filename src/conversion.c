#include "s21_decimal.h"

int s21_from_int_to_decimal(int src, s21_decimal *dst) {
  int res = 0;
  zero_decimal(dst);

  if (src < 0) {
    set_sign(dst, 1);
    src = (-1) * src;
  }
  if (src > I_MAX)
    res = 1;
  else
    dst->bits[0] = src;

  return res;
}

int s21_from_decimal_to_int(s21_decimal src, int *dst) {
  int res = 0;

  int scale = get_scale(src);
  if (src.bits[1] || src.bits[2]) {
    res = 1;
  } else {
    *dst = src.bits[0];
    if (scale > 0 && scale <= 28) {
      *dst /= pow(10, scale);
    }
  }
  if (get_bit(src, 127)) *dst = *dst * (-1);

  return res;
}
