


CC=gcc

CFLAGS =  -Wno-error=deprecated-declarations -Wextra -Wwrite-strings -Wno-unused-parameter -std=gnu99 -D_GNU_SOURCE -D_REEN^CANT -g -O2
CFLAGS +=-I./json-c -I./


main:main.c
	$(CC) $(CFLAGS) -o $@ main.c  ./json-c/.libs/libjson-c.so
