CC = gcc
CFLAGS = -Wall -Werror -Wextra
FLAG_COV = --coverage
TST_LIBS = -lcheck -lpthread -lm
TESTS = tests/test.c tests/test_negate.c tests/test_fromint.c tests/test_mul.c tests/test_comp.c tests/test_truncate.c tests/test_add.c tests/test_sub.c tests/test_round.c tests/test_floor.c tests/test_div.c tests/test_fromfloat.c tests/test_tofloat.c
SOURCES = s21_service.c other.c s21_comparison.c s21_normalize.c s21_arithmetic.c conversion.c s21_div.c s21_float_conv.c
OBJECTS = $(patsubst %.c,%.o,$(SOURCES))
TEST_OBJECTS = $(patsubst %.c,%.o,$(TESTS))
MAIN_TEST_EXECUTABLE = s21_decimal

all: pic clean s21_decimal.a test

pic:
	bash main.sh

s21_decimal.a: $(OBJECTS)
	ar rcs $@ $^

gcov_report: test
	$(CC) $(FLAGS) --coverage $(SOURCES) $(TESTS) $(TST_LIBS) -o gcov_test
	chmod +x *
	./gcov_test
	gcov s21_*.gcda
	lcov -t "test" -o s21_decimal.info -c -d .
	genhtml -o ./report s21_decimal.info
	open ./report/index.html

rebuild: clean all

$(OBJECTS):
	gcc -c $(CFLAGS) $(patsubst %.o,%.c,$@) -o $@
	
$(TEST_OBJECTS):
	gcc -c $(CFLAGS) $(patsubst %.o,%.c,$@) -o $@

test: s21_decimal.a $(TEST_OBJECTS)	
	gcc $(TST_LIBS) $^ -o $(MAIN_TEST_EXECUTABLE)
	echo "RUNNING TESTS"
	./$(MAIN_TEST_EXECUTABLE)

clean:
	rm -f *.a *.o ./tests/*.a ./tests/*.o | true
	rm -r report | true
	rm s21_decimal | true
	rm gcov_test | true
	rm *.gcda | true
	rm *.gcno | true
	rm *.gcov | true
	rm *.info | true

style:
	clang-format -n --style=google *.c *.h
	cd tests && clang-format -n --style=google *.c *.h
