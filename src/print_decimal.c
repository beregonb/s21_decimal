#include "s21_decimal.h"
/**
 * @brief Функция вывода ошибки
 *
 * @param param параметр
 */
void error_param(int param) {
  printf("\tinvalid param! choose 0 - 2, not %d\n", param);
  printf(
      "Parameters: \n"
      "0: "
      "1000000000000000000000000000000000000000000000000000000000000000000000"
      "00000000000000000000000001,\n1: 10000000000000000000000000000000 "
      "00000000000000000000000000000000 00000000000000000000000000000001"
      ",\n2: bits[3] = 10000000000000000000000000000000 \n   bits[2] = "
      "10000000000000000000000000000000 \n   bits[1] = "
      "00000000000000000000000000000000\n   bits[0] = "
      "00000000000000000000000000000001\n\tDUREHA!");
}

/**
 * @brief Функция вывода битов с 127-0 или 95-0
 *
 * @param value Является передаваемой структурой, которую надо вывести
 * @param range Диапазон от кого бита будет показывать
 * @param param Параметр на то как выводить
 * (0 - вывод мантисы без 3 ячейки,
 *  1 -вывод мантисы с пробелами  ,
 *  2 - вывод по ячейкам           )
 */
void print_bit(s21_decimal value, int range, int param) {
  for (int i = range, count = 3; i >= 0; --i) {
    if ((param == 2) && ((i + 1) % 32 == 0)) {
      printf("bits[%d] = ", count--);
    }
    int bit = get_bit(value, i);
    printf("%d", bit);
    if ((param == 1) && (i % 32 == 0) && (i != 0)) {
      printf(" ");
    } else if ((param == 2) && (i % 32 == 0) && (i != 0)) {
      printf("\n");
    }
  }
  printf("\n");
}

/**
 * @brief Парсер параметров
 *
 * @param value стуктура децимала
 * @param param параметр 0-2
 */
void output_bit_value(s21_decimal value, int param) {
  int range = 127;
  if (!param || param == 1) {
    printf("value in s21_decimal:\n");
    range = param ? 127 : 95;
    print_bit(value, range, param);
  } else if (param == 2) {
    print_bit(value, range, param);
  } else {
    error_param(param);
  }
}
/*------------------------BIG------------------------*/
void print_bit_b(s21_big_decimal value, int range, int param) {
  for (int i = range, count = 3; i >= 0; --i) {
    if ((param == 2) && ((i + 1) % 32 == 0)) {
      printf("bits[%d] = ", count--);
    }
    int bit = big_get_bit(value, i);
    printf("%d", bit);
    if ((param == 1) && (i % 32 == 0) && (i != 0)) {
      printf(" ");
    } else if ((param == 2) && (i % 32 == 0) && (i != 0)) {
      printf("\n");
    }
  }
  printf("\n");
}

void output_bit_value_b(s21_big_decimal value, int param) {
  int range = 255;
  if (!param || param == 1) {
    printf("value in s21_decimal:\n");
    range = param ? 255 : 223;
    print_bit_b(value, range, param);
  } else if (param == 2) {
    print_bit_b(value, range, param);
  } else {
    error_param(param);
  }
}

void test_output() {
  s21_decimal num = {
      0b00000000000000000000000000000001, 0b00000000000000000000000000000000,
      0b10000000000000000000000000000000, 0b00000000010100000000000000000000};
  output_bit_value(num, 2);
}
