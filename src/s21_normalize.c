#include <stdio.h>

#include "s21_decimal.h"

/*-------service for normalization--------*/

void typeCasting_B(s21_big_decimal data1, s21_big_decimal *data2) {
  for (int i = 0; i <= 7; ++i) data2->bits[i] = data1.bits[i];
}

/*Normalize*/

void costil(s21_big_decimal *value) {
  s21_big_decimal num_1 = {0};
  s21_big_decimal num_2 = {0};
  typeCasting_B(*value, &num_1);
  typeCasting_B(*value, &num_2);
  shift_left_big(&num_1, 3);
  shift_left_big(&num_2, 1);
  s21_add_big_calculation(num_1, num_2, value);
}

void s21_increase_scale(s21_big_decimal *value, int order) {
  int old_scale = big_get_scale(*value);
  for (int i = 0; i < order; ++i) {
    costil(value);
  }
  big_set_scale(value, old_scale + order);
}

void normalization_B(s21_big_decimal *value_1, s21_big_decimal *value_2) {
  int order_diff = big_get_scale(*value_1) - big_get_scale(*value_2);
  if (order_diff > 0) {
    s21_increase_scale(value_2, order_diff);
  } else if (order_diff != 0) {
    s21_increase_scale(value_1, -order_diff);
  }
}