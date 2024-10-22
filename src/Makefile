CC = g++
CFLAGS = -Wall -Werror -Wextra -std=c++17
TFLAGS = -Wall -Werror -Wextra -std=c++17
SRC = s21_matrix_oop.cpp
OBJS = $(SRC:.cpp=.o)
LIBS = -lgtest -pthread
ifeq ($(filter-out $@,$(MAKECMDGOALS)),gcov_report)
	CFLAGS += --coverage
endif

ifeq ($(filter-out $@,$(MAKECMDGOALS)),debug)
	CFLAGS += -g
endif


all: s21_matrix_oop.a test clean clang 

clang:
	@echo "---Style---\n"
	@clang-format --style=Google -n *.cpp
	@clang-format --style=Google -n *.h

############### подключение библиотеки к юнит тестам######################

test: s21_matrix_oop.a matrix_test.o
	$(CC) $(CFLAGS) matrix_test.o s21_matrix_oop.a -o unit_test $(LIBS)
	./unit_test

matrix_test.o: matrix_test.cpp 
	$(CC) $(TFLAGS) -c matrix_test.cpp -o matrix_test.o



# main: s21_matrix_oop.a main.o
# 	$(CC) $(CFLAGS) main.o s21_matrix_oop.a -o main.out $(LIBS)
# 	./main.out

# main.o: main.cpp 
# 	$(CC) $(CFLAGS) -c main.cpp -o main.o

######чистая библиотека########
s21_matrix_oop.a: $(OBJS)
	ar rcs s21_matrix_oop.a $(OBJS)

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@
################################



clean_without_report: 
	rm -f  *.o *.a *.gcda *.gcno *.info main.out unit_test

clean: clean_without_report
	rm -rf report
