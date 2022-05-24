IDIR :=./include
CC := gcc
CFLAGS := --coverage --pedantic -fsanitize=address --std=c17 -O3 -I$(IDIR) -Wall -Wextra
GCOV := gcov

BUILDDIR := ./build
SOURCEDIR := ./src

NAME := flit
TESTNAME := flittest

build:
	$(CC) $(CFLAGS) main.c -o $(NAME)

# remove executables and temporary files
clean:
	rm -f *.gcda
	rm -f *.gcno
	rm -f *.gcov
	rm -f $(NAME)
	rm -f $(TESTNAME)