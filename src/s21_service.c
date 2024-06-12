#include "s21_decimal.h"

/**
 * @brief Показывает бит из 32-ой позиции 3-ей ячейки s21_decimal'a
 *
 * @param value подается s21_decimal, где надо показать, какой знак стоит на 32
 * бите
 * @return возвращается 1 или 0 (1 - знак отрицательный, 0 - знак положительный)
 */
int get_sign(s21_decimal value) { return ((value.bits[3] >> 31) & 1u); }

/**
 * @brief Устанавливает нужный бит в 32-ю позицию 3-ей ячейки s21_decimal'a
 *
 * @param value s21_decimal, где производится манипуляции
 * @param sign_bit знак, который необходимо установить (1 - знак отрицательный,
 * 0 - знак положительный)
 */
void set_sign(s21_decimal *value, int sign_bit) {
  if (sign_bit) {
    value->bits[3] |= (1u << 31);
  } else {
    value->bits[3] &= ~(1u << 31);
  }
}

/**
 * @brief Показывает число из битов позиций 17-24 из 3-ей ячейки s21_decimal'a
 *
 * @param value s21_decimal, где надо посмотреть scale
 * @return число степени 10
 */
int get_scale(s21_decimal value) {
  int mask = (value.bits[3]) & ~(1u << 31);
  int scale = (mask & value.bits[3]) >> 16;
  return scale;
}

/**
 * @brief Устанавливает число в позиции 17-24 3-ей ячейки s21_decimal'a
 *
 * @param value s21_decimal, где будет устанавливаться степень 10
 * @param scale_value число, которое будет установлено в биты степени
 */
void set_scale(s21_decimal *value, int scale_value) {
  int sign = get_sign(*value);
  value->bits[3] = 0;
  value->bits[3] |= (scale_value << 16);
  if (sign) set_sign(value, sign);
}

/**
 * @brief Показывает бит на нужной позиции в s21_decimal
 *
 * @param value s21_decimal, в котором надо узнать бит
 * @param bit индекс бита (0-127)
 * @return бит на нужной позиции
 */
int get_bit(s21_decimal value, int bit) {
  return ((value.bits[bit / 32] & (1u << (bit % 32))) >> (bit % 32));
}

/**
 * @brief Устанавливает нужный бит на нужную позицию в s21_decimal
 *
 * @param value s21_decimal, в котором надо установить бит
 * @param bit индекс бита (0-127)
 * @param value_bit какой бит устанавливается
 */
void set_bit(s21_decimal *value, int bit, unsigned int value_bit) {
  if (value_bit) {
    value->bits[bit / 32] |= (1u << (bit % 32));
  } else {
    value->bits[bit / 32] &= ~(1u << (bit % 32));
  }
}

/**
 * @brief Показывает бит из 32-ой позиции 7-ей ячейки s21_big_decimal'a
 *
 * @param value подается s21_big_decimal, где надо показать, какой знак стоит на
 * 32 бите
 * @return возвращается 1 или 0 (1 - знак отрицательный, 0 - знак положительный)
 */
int big_get_sign(s21_big_decimal value) { return ((value.bits[7] >> 31) & 1u); }

/**
 * @brief Устанавливает нужный бит в 32-ю позицию 7-ей ячейки s21_big_decimal'a
 *
 * @param value s21_big_decimal, где производится манипуляции
 * @param sign_bit знак, который необходимо установить (1 - знак отрицательный,
 * 0 - знак положительный)
 */
void big_set_sign(s21_big_decimal *value, int sign_bit) {
  if (sign_bit) {
    value->bits[7] |= (1u << 31);
  } else {
    value->bits[7] &= ~(1u << 31);
  }
}

/**
 * @brief Показывает число из битов позиций 17-24 из 7-ей ячейки
 * s21_big_decimal'a
 *
 * @param value s21_big_decimal, где надо посмотреть scale
 * @return число степени 10
 */
int big_get_scale(s21_big_decimal value) {
  int mask = (value.bits[7]) & ~(1u << 31);
  int scale = (mask & value.bits[7]) >> 16;
  return scale;
}

/**
 * @brief Устанавливает число в позиции 17-24 7-ей ячейки s21_big_decimal'a
 *
 * @param value s21_big_decimal, где будет устанавливаться степень 10
 * @param scale_value число, которое будет установлено в биты степени
 */
void big_set_scale(s21_big_decimal *value, int scale_value) {
  int sign = big_get_sign(*value);
  value->bits[7] = 0;
  value->bits[7] |= (scale_value << 16);
  if (sign) big_set_sign(value, sign);
}

/**
 * @brief Показывает бит на нужной позиции в s21_big_decimal
 *
 * @param value s21_big_decimal, в котором надо узнать бит
 * @param bit индекс бита (0-255)
 * @return бит на нужной позиции
 */
int big_get_bit(s21_big_decimal value, int bit) {
  return ((value.bits[bit / 32] & (1u << (bit % 32))) >> (bit % 32));
}

/**
 * @brief Устанавливает нужный бит на нужную позицию в s21_big_decimal
 *
 * @param value s21_big_decimal, в котором надо установить бит
 * @param bit индекс бита (0-255)
 * @param value_bit какой бит устанавливается
 */
void big_set_bit(s21_big_decimal *value, int bit, unsigned int value_bit) {
  if (value_bit) {
    value->bits[bit / 32] |= (1u << (bit % 32));
  } else {
    value->bits[bit / 32] &= ~(1u << (bit % 32));
  }
}

/**
 * @brief Сдвигает все биты биг децимала структуры налево
 *
 * @param num структура s21_big_decimal
 * @param shift количество сдвигов
 */
void shift_left_big(s21_big_decimal *num, int shift) {
  while (shift > 0) {
    unsigned memory = 0;
    for (int i = 0; i < 7; ++i) {
      unsigned temp = num->bits[i];
      num->bits[i] <<= 1;
      num->bits[i] |= memory;
      memory = temp >> (32 - 1);
    }

    shift--;
  }
}

/**
 * @brief Сдвигает все биты децимала структуры налево
 *
 * @param num структура s21_decimal
 * @param shift количество сдвигов
 */
void shift_left(s21_decimal *num, int shift) {
  while (shift > 0) {
    unsigned memory = 0;
    for (int i = 0; i < 3; ++i) {
      unsigned temp = num->bits[i];
      num->bits[i] <<= 1;
      num->bits[i] |= memory;
      memory = temp >> (32 - 1);
    }

    shift--;
  }
}

/**
 * @brief Сдвигает все биты биг децимала структуры направо
 *
 * @param num структура s21_decimal
 * @param shift количество сдвигов
 */
void shift_right_big(s21_big_decimal *num, int shift) {
  while (shift > 0) {
    unsigned memory = 0;
    for (int i = 6; i >= 0; --i) {
      unsigned temp = num->bits[i];
      num->bits[i] >>= 1;
      num->bits[i] |= memory;
      memory = temp << (32 - 1);
    }

    shift--;
  }
}

/**
 * @brief Сдвигает все биты децимала структуры направо
 *
 * @param num структура s21_decimal
 * @param shift количество сдвигов
 */
void shift_right(s21_decimal *num, int shift) {
  while (shift > 0) {
    unsigned memory = 0;
    for (int i = 2; i >= 0; --i) {
      unsigned temp = num->bits[i];
      num->bits[i] >>= 1;
      num->bits[i] |= memory;
      memory = temp << (32 - 1);
    }

    shift--;
  }
}

/**
 * @brief Импортирует данные из обычного s21_decimal в s21_big_decimal
 * @param value_1 откуда импортируется s21_decimal
 * @param value_2 куда импортируется s21_big_decimal
 */
void import_to_big(s21_decimal value_1, s21_big_decimal *value_2) {
  value_2->bits[0] = value_1.bits[0];
  value_2->bits[1] = value_1.bits[1];
  value_2->bits[2] = value_1.bits[2];
  value_2->bits[7] = value_1.bits[3];
}

/**
 * @brief Импортирует данные из big decimal в обычный decimal
 *
 * @param value_1 куда импортируется s21_decimal
 * @param value_2 откуда импортируется s21_big_decimal
 */
void import_into_normal(s21_decimal *value_1, s21_big_decimal value_2) {
  value_1->bits[0] = value_2.bits[0];
  value_1->bits[1] = value_2.bits[1];
  value_1->bits[2] = value_2.bits[2];
  value_1->bits[3] = value_2.bits[7];
}

/**
 * @brief обнуляет весь s21_decimal
 *
 * @param dst указатель на структуру s21_decimal
 *
 */
void zero_decimal(s21_decimal *dst) {
  dst->bits[0] = 0;
  dst->bits[1] = 0;
  dst->bits[2] = 0;
  dst->bits[3] = 0;
}

/**
 * @brief обнуляет весь s21_big_decimal
 *
 * @param dst указатель на структуру s21_big_decimal
 *
 */
void zero_big_decimal(s21_big_decimal *dst) {
  for (int i = 0; i <= 7; ++i) {
    dst->bits[i] = 0;
  }
}

/**
 * @brief Находит высший бит числа в s21_decimal
 *
 * @param value подается структура s21_decimal, где производится поиск
 * @return i выводит индекс высшего бита
 */
int highets_bit(s21_decimal value) {
  int i = 95, breaker = 1;
  while (i >= 0 && breaker) {
    if (get_bit(value, i)) {
      breaker = 0;
    } else {
      --i;
    }
  }
  return i;
}

/**
 * @brief Находит высший бит числа в s21_big_decimal
 *
 * @param value подается структура s21_big_decimal, где производится поиск
 * @return i выводит индекс высшего бита
 */
int big_highets_bit(s21_big_decimal value) {
  int i = 224, breaker = 1;
  while (i >= 0 && breaker) {
    if (big_get_bit(value, i)) {
      breaker = 0;
    } else {
      --i;
    }
  }
  return i;
}

/**
 * @brief Проверка s21_decimal на 0
 *
 * @param value структура s21_decimal
 * @return int 0 - s21_decimal пустой, 1 - если он не нулевой
 */
int decimal_is_null(s21_decimal value) {
  int res = 0;
  if (value.bits[0] == 0 && value.bits[1] == 0 && value.bits[2] == 0) {
    res = 1;
  }
  return res;
}

/**
 * @brief Проверка s21_big_decimal на 0
 *
 * @param value структура s21_big_decimal
 * @return int 0 - s21_big_decimal пустой, 1 - если он не нулевой
 */
int decimal_is_null_big(s21_big_decimal value) {
  int res = 0;
  if (value.bits[0] == 0 && value.bits[1] == 0 && value.bits[2] == 0 &&
      value.bits[3] == 0 && value.bits[4] == 0 && value.bits[5] == 0 &&
      value.bits[6] == 0) {
    res = 1;
  }
  return res;
}

/**
 * @brief Проверка двух s21_decimal на пустоту
 *
 * @param value1 подается s21_decimal 1
 * @param value2 подается s21_decimal 2
 * @return int результат проверки, 0 - не пустой, 1 - пустой
 */
int decimal_is_two_null(s21_decimal value1, s21_decimal value2) {
  int res = 0;
  if (decimal_is_null(value1) && decimal_is_null(value2)) {
    res = 1;
  }
  return res;
}

/**
 * @brief Проверка на переполнение s21_big_decimal'a
 *
 * @param value подается s21_big_decimal
 * @param res переменная, в которую будет вкладываться код ошибки
 */
void overflow(s21_big_decimal value, int *res) {
  if ((value.bits[3] > 0 || value.bits[4] > 0 || value.bits[5] > 0 ||
       value.bits[6] > 0) &&
      big_get_sign(value) == 0) {
    *res = 1;
  } else if ((value.bits[3] > 0 || value.bits[4] > 0 || value.bits[5] > 0 ||
              value.bits[6] > 0) &&
             big_get_sign(value) == 1) {
    *res = 2;
  } else
    *res = 0;
}

/**
 * @brief Банковское округление
 *
 * @param t1 структура big decimal, которую нужно округлить
 * @param result результат округления
 */
int rounding(s21_big_decimal t1, s21_big_decimal *result) {
  int error = 0;
  s21_big_decimal div = {{10, 0, 0, 0, 0, 0, 0, 0}};
  s21_big_decimal ost = {{0, 0, 0, 0, 0, 0, 0, 0}};
  s21_big_decimal add = {{1, 0, 0, 0, 0, 0, 0, 0}};
  s21_big_decimal ost2 = {{0, 0, 0, 0, 0, 0, 0, 0}};
  s21_big_decimal z = {{0, 0, 0, 0, 0, 0, 0, 0}};
  int scale = big_get_scale(t1);
  overflow(t1, &error);
  if (error != 0) {
    while (error != 0 && scale > 0) {
      ost = s21_big_div(t1, div, &t1);
      ost2 = s21_big_div(t1, div, &z);
      if (scale != 1 && ost2.bits[0] % 2 == 0 && ost.bits[0] > 5) {
        s21_add_big_calculation(t1, add, &t1);
      } else if (scale != 1 && ost2.bits[0] % 2 != 0 && ost.bits[0] > 4) {
        s21_add_big_calculation(t1, add, &t1);
      }
      overflow(t1, &error);
      scale--;
      if (scale == 0 && t1.bits[0] % 2 == 0 && ost.bits[0] > 5) {
        s21_add_big_calculation(t1, add, &t1);
        overflow(t1, &error);
      } else if (scale == 0 && t1.bits[0] % 2 != 0 && ost.bits[0] > 4) {
        s21_add_big_calculation(t1, add, &t1);
        overflow(t1, &error);
      }
    }
  }
  big_set_scale(&t1, scale);
  *result = t1;
  return error;
}

/**
 * @brief удаляет числа после запятой
 *
 * @param dst откуда произойдет удаление
 * @param result целое число
 * @return int код ошибки
 */
int trancate_big(s21_big_decimal dst, s21_big_decimal *result) {
  zero_big_decimal(result);
  int res = 0;
  int scale = big_get_scale(dst);
  s21_big_decimal div = {{10, 0, 0, 0, 0, 0, 0, 0}};
  while (scale) {
    s21_big_div(dst, div, &dst);
    scale--;
  }
  *result = dst;
  return res;
}