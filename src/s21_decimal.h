#ifndef S21_DECIMAL_H
#define S21_DECIMAL_H
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define I_MAX 2147483647

typedef struct {
  unsigned int bits[4];
} s21_decimal;

typedef struct {
  unsigned int bits[8];
} s21_big_decimal;

// arithmetic block(core)
//  0 - OK
//  1 - число слишком велико или равно бесконечности
//  2 - число слишком мало или равно отрицательной бесконечности
//  3 - деление на 0
int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_add_calculation(s21_decimal value_1, s21_decimal value_2,
                        s21_decimal *result);
void s21_add_big_calculation(s21_big_decimal value_1, s21_big_decimal value_2,
                             s21_big_decimal *result);
int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_sub_culculation(s21_decimal value_1, s21_decimal value_2,
                        s21_decimal *result);
void s21_sub_big_culculation(s21_big_decimal value_1, s21_big_decimal value_2,
                             s21_big_decimal *result);
int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
s21_big_decimal s21_big_div(s21_big_decimal value_1, s21_big_decimal value_2,
                            s21_big_decimal *result);
int support_for_div(s21_big_decimal res, s21_big_decimal mem,
                    s21_big_decimal value_2, s21_decimal *result);
s21_decimal s21_abs(s21_decimal value_1);

// comparison block(core)
int s21_is_less(s21_decimal, s21_decimal);
int s21_is_less_or_equal(s21_decimal, s21_decimal);
int s21_is_greater(s21_decimal, s21_decimal);
int s21_is_greater_or_equal(s21_decimal, s21_decimal);
int s21_is_equal(s21_decimal, s21_decimal);
int s21_is_not_equal(s21_decimal, s21_decimal);
// comparison block for big_decimal
int s21_big_is_less(s21_big_decimal dec1, s21_big_decimal dec2);
int s21_big_is_less_or_equal(s21_big_decimal dec1, s21_big_decimal dec2);
int s21_big_is_greater(s21_big_decimal dec1, s21_big_decimal dec2);
int s21_big_is_greater_or_equal(s21_big_decimal dec1, s21_big_decimal dec2);
int s21_big_is_equal(s21_big_decimal dec1, s21_big_decimal dec2);

// conversion block(core)
//  0 - OK
//  1 - ошибка конвертации
int s21_from_int_to_decimal(int src, s21_decimal *dst);
int s21_from_float_to_decimal(float src, s21_decimal *dst);
int s21_from_decimal_to_int(s21_decimal src, int *dst);
int s21_from_decimal_to_float(s21_decimal src, float *dst);

// other(core)
//  0 - OK
//  1 - ошибка вычисления
int s21_floor(s21_decimal value, s21_decimal *result);
int s21_round(s21_decimal value, s21_decimal *result);
int s21_truncate(s21_decimal value, s21_decimal *result);
int s21_negate(s21_decimal value, s21_decimal *result);
int trancate_big(s21_big_decimal dst, s21_big_decimal *result);

// service funtions
int get_bit(s21_decimal value, int bit);
void set_bit(s21_decimal *value, int bit, unsigned int value_bit);
int get_sign(s21_decimal value);
void set_sign(s21_decimal *value, int sign_bit);
int get_scale(s21_decimal value);
void set_scale(s21_decimal *value, int scale_value);
void shift_left(s21_decimal *num, int shift);
void zero_decimal(s21_decimal *dst);
void shift_right(s21_decimal *num, int shift);
int decimal_is_null(s21_decimal value);
int decimal_is_two_null(s21_decimal value1, s21_decimal value2);
void import_into_normal(s21_decimal *value_1, s21_big_decimal value_2);
int highets_bit(s21_decimal value);

// service funtions (big decimal)
int big_get_bit(s21_big_decimal value, int bit);
void big_set_bit(s21_big_decimal *value, int bit, unsigned int value_bit);
int big_get_sign(s21_big_decimal value);
void big_set_sign(s21_big_decimal *value, int sign_bit);
int big_get_scale(s21_big_decimal value);
void big_set_scale(s21_big_decimal *value, int scale_value);
void shift_left_big(s21_big_decimal *num, int shift);
void zero_big_decimal(s21_big_decimal *dst);
void shift_right_big(s21_big_decimal *num, int shift);
void normalization_B(s21_big_decimal *value_1, s21_big_decimal *value_2);
int decimal_is_null_big(s21_big_decimal value);
void import_to_big(s21_decimal value_1, s21_big_decimal *value_2);
int big_highets_bit(s21_big_decimal value);
int rounding(s21_big_decimal t1, s21_big_decimal *result);
void overflow(s21_big_decimal value, int *res);

// print decimal
void output_bit_value(s21_decimal value, int param);
void print_bit(s21_decimal value, int range, int param);
void output_bit_value_b(s21_big_decimal value, int param);
void print_bit_b(s21_big_decimal value, int range, int param);
void error_param(int param);
// mul on 10
void costil(s21_big_decimal *value);
#endif