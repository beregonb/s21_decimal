#include "s21_decimal.h"

int s21_is_null(s21_decimal dec) {
  int null = 1;
  for (int i = 0; i < 96 && null; i++) {
    if (get_bit(dec, i) == 1) {
      null = 0;
    }
  }
  return null;
}
int s21_compar(s21_decimal dec1, s21_decimal dec2) {
  int flag = 1;
  for (int i = 0; i < 96 && flag; i++) {
    if (get_bit(dec1, i) != get_bit(dec2, i)) {
      flag = 0;
    }
  }
  return flag;
}
int s21_big_compar(s21_big_decimal dec1, s21_big_decimal dec2) {
  int flag = 1;
  for (int i = 0; i < 223 && flag; i++) {
    if (big_get_bit(dec1, i) != big_get_bit(dec2, i)) {
      flag = 0;
    }
  }
  return flag;
}
int s21_is_equal(s21_decimal dec1, s21_decimal dec2) {
  int flag = 1;
  if (get_sign(dec1) != get_sign(dec2)) {
    if (!(s21_is_null(dec1) && s21_is_null(dec2))) {
      flag = 0;
    }
  } else if ((get_scale(dec1) != get_scale(dec2)) &&
             (get_sign(dec1) == get_sign(dec2))) {
    s21_big_decimal copy_dec1 = {0};
    s21_big_decimal copy_dec2 = {0};

    import_to_big(dec1, &copy_dec1);
    import_to_big(dec2, &copy_dec2);
    normalization_B(&copy_dec1, &copy_dec2);

    flag = s21_big_compar(copy_dec1, copy_dec2);
  } else
    flag = s21_compar(dec1, dec2);
  return flag;
}
int s21_is_not_equal(s21_decimal dec1, s21_decimal dec2) {
  return !s21_is_equal(dec1, dec2);
}
int s21_is_greater(s21_decimal dec1, s21_decimal dec2) {
  int flag = 0;
  if ((get_sign(dec1) == 0) && (get_sign(dec2) == 1)) {
    flag = 1;
  } else if (get_sign(dec1) == get_sign(dec2)) {
    int bit1 = 0, bit2 = 0;
    s21_big_decimal copy_dec1 = {0};
    s21_big_decimal copy_dec2 = {0};
    import_to_big(dec1, &copy_dec1);
    import_to_big(dec2, &copy_dec2);
    if (get_scale(dec1) != get_scale(dec2)) {
      normalization_B(&copy_dec1, &copy_dec2);
    }
    int i = 223;
    while (bit1 == bit2 && i >= 0) {
      bit1 = big_get_bit(copy_dec1, i);
      bit2 = big_get_bit(copy_dec2, i);
      i--;
    }
    if (bit1 > bit2) flag = 1;
    if ((get_sign(dec1) == 1) && (get_sign(dec2) == 1) &&
        !s21_big_is_equal(copy_dec1, copy_dec2)) {
      flag = !flag;
    }
  }
  return flag;
}
int s21_is_greater_or_equal(s21_decimal dec1, s21_decimal dec2) {
  return (s21_is_equal(dec1, dec2) || s21_is_greater(dec1, dec2));
}
int s21_is_less(s21_decimal dec1, s21_decimal dec2) {
  int flag = 0;
  if (!s21_is_equal(dec1, dec2)) {
    flag = !s21_is_greater(dec1, dec2);
  }
  return flag;
}
int s21_is_less_or_equal(s21_decimal dec1, s21_decimal dec2) {
  return (s21_is_equal(dec1, dec2) || s21_is_less(dec1, dec2));
}

/*---big---*/
int s21_big_is_null(s21_big_decimal dec) {
  int null = 1;
  for (int i = 0; i < 223 && null; i++) {
    if (big_get_bit(dec, i) == 1) {
      null = 0;
    }
  }
  return null;
}

int s21_big_is_greater(s21_big_decimal dec1, s21_big_decimal dec2) {
  int flag = 0;
  if ((big_get_sign(dec1) == 0) && (big_get_sign(dec2) == 1)) {
    flag = 1;
  } else if (big_get_sign(dec1) == big_get_sign(dec2)) {
    int bit1 = 0, bit2 = 0;
    s21_big_decimal copy_dec1 = {0};
    s21_big_decimal copy_dec2 = {0};
    for (int i = 0; i <= 7; i++) {
      copy_dec1.bits[i] = dec1.bits[i];
      copy_dec2.bits[i] = dec2.bits[i];
    }
    normalization_B(&copy_dec1, &copy_dec2);
    int i = 223;

    while (bit1 == bit2 && i >= 0) {
      bit1 = big_get_bit(copy_dec1, i);
      bit2 = big_get_bit(copy_dec2, i);
      i--;
    }
    if (bit1 > bit2) flag = 1;
    if ((big_get_sign(dec1) == 1) && (big_get_sign(dec2) == 1) &&
        s21_big_is_equal(dec1, dec2)) {
      flag = !flag;
    }
  }
  return flag;
}
int s21_big_is_equal(s21_big_decimal dec1, s21_big_decimal dec2) {
  int flag = 1;
  if (big_get_sign(dec1) != big_get_sign(dec2)) {
    if (!(s21_big_is_null(dec1) && s21_big_is_null(dec2))) {
      flag = 0;
    }
  } else if ((big_get_scale(dec1) != big_get_scale(dec2)) &&
             (big_get_sign(dec1) == big_get_sign(dec2))) {
    s21_big_decimal copy_dec1 = {0};
    s21_big_decimal copy_dec2 = {0};
    for (int i = 0; i <= 7; i++) {
      copy_dec1.bits[i] = dec1.bits[i];
      copy_dec2.bits[i] = dec2.bits[i];
    }
    normalization_B(&copy_dec1, &copy_dec2);
    flag = s21_big_compar(copy_dec1, copy_dec2);
  } else
    flag = s21_big_compar(dec1, dec2);
  return flag;
}
int s21_big_is_greater_or_equal(s21_big_decimal dec1, s21_big_decimal dec2) {
  int flag = 0;
  flag = s21_big_is_greater(dec1, dec2) || s21_big_is_equal(dec1, dec2);
  return flag;
}
int s21_big_is_less(s21_big_decimal dec1, s21_big_decimal dec2) {
  return !s21_big_is_greater(dec1, dec2);
}
int s21_big_is_less_or_equal(s21_big_decimal dec1, s21_big_decimal dec2) {
  return !s21_big_is_greater_or_equal(dec1, dec2);
}