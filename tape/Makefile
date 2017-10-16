


CC=gcc


progam_src = main.c log.c json.c config.c
progam_obj =  $(progam_src:.c=.o)

CFLAGS += -Wall -Wno-unused-result -Wno-discarded-qualifiers
CFLAGS +=  -Wno-deprecated-declarations -Wextra -Wwrite-strings -Wno-unused-parameter -std=gnu99 -D_GNU_SOURCE -D_REEN^CANT -g -O2
CFLAGS +=-I./json-c -I./

all: main
LIB=./json-c/.libs/libjson-c.so


$(progam_src:.c=.o): %.o : %.c
	$(CC) $(CFLAGS)  $^ -c -o $@


main: $(progam_obj)
#	$(CC) $(LDFLAGS) -Wl,-rpath=./  -L./json-c/.libs  -ljson-c $^ $(LIB) -o $@ 
	$(CC) $(LDFLAGS) -Wl,-rpath=./   -ljson-c -L./json-c/.libs $^ $(LIB) -o $@ 
#	$(CC) $(LDFLAGS) -Wl,-rpath=./  $^ -o $@ -L./ ./json-c/.libs/libjson-c.so


clean:
	rm main *.o -f
