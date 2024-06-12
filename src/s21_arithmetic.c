#include "s21_decimal.h"

/**
 * @brief Функция сложения двух чисел decimal. Тут проводится проверка на
 * валидность и знак чисел
 *
 * @param value_1 Первое число decimal
 * @param value_2 Второе число decimal
 * @param result Результат сложения
 * @return int Тут будут ошибки, в будущем добавлю, когда додумаюсь
 */
int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int error = 0;
  if (!decimal_is_null(value_1) && !decimal_is_null(value_2)) {
    if ((get_sign(value_1) && get_sign(value_2))) {
      // два числа отрицательные
      // делаем модульы и суммируем, меняем знак
      error = s21_add_calculation(s21_abs(value_1), s21_abs(value_2), result);
      s21_negate(*result, result);
    } else if (!get_sign(value_1) && !get_sign(value_2)) {
      // два числа положительные
      // суммируем
      error = s21_add_calculation(value_1, value_2, result);
    } else if (!get_sign(value_1) && get_sign(value_2)) {
      // первое положительное, второе отрицательное
      // делаем модуль второго, вычитаем и меняем знак
      error = s21_sub(value_1, s21_abs(value_2), result);
    } else {
      // первое отрицательное, второе положительное
      // делаем модуль первого, вычитаем и меняем знак
      error = s21_sub(s21_abs(value_1), value_2, result);
      s21_negate(*result, result);
    }
  } else if (!decimal_is_null(value_1) && decimal_is_null(value_2)) {
    *result = value_1;
  } else if (decimal_is_null(value_1) && !decimal_is_null(value_2)) {
    *result = value_2;
  } else {
    zero_decimal(result);
  }

  return error;
}
/**
 * @brief Основная функция сложения двух чисел decimal
 *
 * @param value_1 Первое число decimal
 * @param value_2 Второе число decimal
 * @param result Результат сложения
 */
int s21_add_calculation(s21_decimal value_1, s21_decimal value_2,
                        s21_decimal *result) {
  int error = 0;
  s21_big_decimal value_1_b = {0};
  s21_big_decimal value_2_b = {0};
  s21_big_decimal result_b = {0};
  import_to_big(value_1, &value_1_b);
  import_to_big(value_2, &value_2_b);
  import_to_big(*result, &result_b);
  normalization_B(&value_1_b, &value_2_b);
  unsigned int memo = 0;
  for (int i = 0; i < 224; i++) {
    unsigned int j =
        big_get_bit(value_1_b, i) ^ big_get_bit(value_2_b, i) ^ memo;
    if ((big_get_bit(value_1_b, i) && big_get_bit(value_2_b, i))) {
      memo = 1;
      if (big_get_bit(value_1_b, i + 1) == 0 && memo == 1) {
        big_set_bit(&value_1_b, i + 1, 1);
        memo = 0;
      }
      if (big_get_bit(value_2_b, i + 1) == 0 && memo == 1) {
        big_set_bit(&value_2_b, i + 1, 1);
        memo = 0;
      }
    }
    big_set_bit(&result_b, i, j);
  }
  for (int i = 224; i < 256; i++) {
    big_set_bit(&result_b, i, big_get_bit(value_1_b, i));
  }
  error = rounding(result_b, &result_b);
  import_into_normal(result, result_b);
  return error;
}

/**
 * @brief Функция вычитания двух чисел decimal. Тут проводится проверка на
 * валидность и знак чисел
 *
 * @param value_1 Первое число decimal
 * @param value_2 Второе число decimal
 * @param result Результат вычитания
 * @return int Тут будут ошибки, в будущем добавлю, когда додумаюсь
 */

int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int error = 0;
  if (!decimal_is_null(value_1) && !decimal_is_null(value_2)) {
    if (!get_sign(value_1) && !get_sign(value_2)) {
      // два числа положительные
      if (s21_is_greater_or_equal(value_1, value_2)) {
        // если первое больше или равно второму. Вычитаем
        error = s21_sub_culculation(value_1, value_2, result);
      } else {
        // если первое меньше второго. Вычитаем и меняем знак
        error = s21_sub_culculation(value_2, value_1, result);
        s21_negate(*result, result);
      }
    } else if (!get_sign(value_1) && get_sign(value_2)) {
      // первое число положительное, второе отрицательное
      // делаем модуль второго, суммируем и меняем знак
      error = s21_add(value_1, s21_abs(value_2), result);
    } else if (get_sign(value_1) && !get_sign(value_2)) {
      // первое отрицательное, второе положительное
      // делаем модуль первого, суммируем и меняем знак
      error = s21_add(s21_abs(value_1), value_2, result);
      s21_negate(*result, result);
    } else {
      // первое отрицательное, второе отрицательное
      if (s21_is_greater_or_equal(value_1, value_2)) {
        // если первое больше или равно второму.
        // Делаем модуль обоих чисел. Вычитаем
        error = s21_sub_culculation(s21_abs(value_2), s21_abs(value_1), result);
      } else {
        // если первое меньше второго. Вычитаем и меняем знак
        // Делаем модуль обоих чисел. Вычитаем
        error = s21_sub_culculation(s21_abs(value_1), s21_abs(value_2), result);
        s21_negate(*result, result);
      }
    }
  } else if (!decimal_is_null(value_1) && decimal_is_null(value_2)) {
    *result = value_1;
  } else if (decimal_is_null(value_1) && !decimal_is_null(value_2)) {
    *result = value_2;
  } else {
    zero_decimal(result);
  }
  return error;
}
/**
 * @brief Основная функция вычитания двух чисел decimal
 *
 * @param value_1 Первое число decimal
 * @param value_2 Второе число decimal
 * @param result Результат вычитания
 */
int s21_sub_culculation(s21_decimal value_1, s21_decimal value_2,
                        s21_decimal *result) {
  int error = 0;
  s21_big_decimal value_1_b = {0};
  s21_big_decimal value_2_b = {0};
  s21_big_decimal result_b = {0};
  import_to_big(value_1, &value_1_b);
  import_to_big(value_2, &value_2_b);
  import_to_big(*result, &result_b);
  normalization_B(&value_1_b, &value_2_b);
  unsigned int memo = 0;
  for (int i = 0; i < 224; i++) {
    if (!big_get_bit(value_1_b, i) && big_get_bit(value_2_b, i)) {
      memo = 1;
      int k = i;
      for (; !big_get_bit(value_1_b, k); k++) {
        big_set_bit(&value_1_b, k, 1);
      }
      big_set_bit(&value_1_b, k, 0);
    }
    unsigned int j =
        (big_get_bit(value_1_b, i) ^ big_get_bit(value_2_b, i)) | memo;
    memo = 0;
    big_set_bit(&result_b, i, j);
  }
  for (int i = 224; i < 256; i++) {
    big_set_bit(&result_b, i, big_get_bit(value_1_b, i));
  }
  error = rounding(result_b, &result_b);
  import_into_normal(result, result_b);
  return error;
}

/**
 * @brief Функция модуля для decimal
 *
 * @param value_1 Число decimal
 * @return Число decimal по модулю
 */
s21_decimal s21_abs(s21_decimal value_1) {
  s21_decimal result = value_1;
  set_sign(&result, 0);
  return result;
}

//----------------------------------------------------------------BIG----------------------------------------------------------------//

/**
 * @brief Функция модуля для big_decimal
 *
 * @param value_1 Число decimal
 * @return Число decimal по модулю
 */
s21_big_decimal s21_abs_B(s21_big_decimal value_1) {
  s21_big_decimal result = value_1;
  big_set_sign(&result, 0);
  return result;
}
/**
 * @brief Основная функция сложения двух чисел big_decimal
 *
 * @param value_1 Первое число big_decimal
 * @param value_2 Второе число big_decimal
 * @param result Результат сложения
 */
void s21_add_big_calculation(s21_big_decimal value_1, s21_big_decimal value_2,
                             s21_big_decimal *result) {
  unsigned int memo = 0;
  for (int i = 0; i < 223; i++) {
    unsigned int j = big_get_bit(value_1, i) ^ big_get_bit(value_2, i) ^ memo;
    if ((big_get_bit(value_1, i) && big_get_bit(value_2, i))) {
      memo = 1;
      if (big_get_bit(value_1, i + 1) == 0 && memo == 1) {
        big_set_bit(&value_1, i + 1, 1);
        memo = 0;
      }
      if (big_get_bit(value_2, i + 1) == 0 && memo == 1) {
        big_set_bit(&value_2, i + 1, 1);
        memo = 0;
      }
    }
    big_set_bit(result, i, j);
  }
  for (int i = 224; i < 256; i++) {
    big_set_bit(result, i, big_get_bit(value_1, i));
  }
}
/**
 * @brief Основная функция вычитания двух чисел big_decimal
 *
 * @param value_1 Первое число big_decimal
 * @param value_2 Второе число big_decimal
 * @param result Результат вычитания
 */
void s21_sub_big_culculation(s21_big_decimal value_1, s21_big_decimal value_2,
                             s21_big_decimal *result) {
  unsigned int memo = 0;
  for (int i = 0; i < 224; i++) {
    if (!big_get_bit(value_1, i) && big_get_bit(value_2, i)) {
      memo = 1;
      int k = i;
      for (; !big_get_bit(value_1, k); k++) {
        big_set_bit(&value_1, k, 1);
      }
      big_set_bit(&value_1, k, 0);
    }
    unsigned int j = (big_get_bit(value_1, i) ^ big_get_bit(value_2, i)) | memo;
    memo = 0;
    big_set_bit(result, i, j);
  }
}

/**
 * @brief функция умножения
 *
 * @param value_1 первое число
 * @param value_2 второе число, на которое будет происходить умножение
 * @param result результат умножения
 * @return int возврашает 0 - ОК, 1 - переполнение в сторону бесконечности
 * 2 - переполнение в сторону отрицательной бесконечности
 */

int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int res = 0;
  zero_decimal(result);
  if (decimal_is_null(value_1) || decimal_is_null(value_2)) {
    res = 0;
  } else {
    int sign1 = get_bit(value_1, 127);
    int sign2 = get_bit(value_2, 127);

    s21_big_decimal tmp1 = {0};
    s21_big_decimal tmp2 = {0};
    s21_big_decimal resultb = {0};
    int scale = get_scale(value_1) + get_scale(value_2);
    zero_big_decimal(&resultb);

    import_to_big(value_2, &tmp2);
    import_to_big(value_1, &tmp1);
    while (!decimal_is_null_big(tmp1)) {
      if (tmp1.bits[0] % 2 == 1) {
        s21_add_big_calculation(resultb, tmp2, &resultb);
      }
      shift_right_big(&tmp1, 1);
      shift_left_big(&tmp2, 1);
    }
    big_set_scale(&resultb, scale);
    res = rounding(resultb, &resultb);
    if (res == 0) {
      import_into_normal(result, resultb);
      if (sign1 == sign2) {  // знак
        set_bit(result, 127, 0);
      } else if (sign1 != sign2) {
        set_bit(result, 127, 1);
      }
    }
  }

  return res;
}