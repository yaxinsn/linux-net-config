


CC=arm-linux-gnueabihf-gcc

TARGET_DIR=../../rundir
STAGING_DIR=../
TOOLCHAIN_DIR=/opt/gcc-linaro-arm-linux-gnueabihf-4.7-2013.03-20130313_linux/arm-linux-gnueabihf/libc/
progam_src =   main.c  sniffer_sip.c  upload.c config.c sniffer_rtp.c
progam_obj =  $(progam_src:.c=.o)
lib_src =  linux-utils.c  log.c  sniffer_lib.c  thread_msg_engine.c  wake_utils.c json.c str_lib.c
lib_obj =  $(lib_src:.c=.o)

all_src = $(progam_src) $(lib_src)
CFLAGS += -Wall  -Wno-unused-result -Wno-discarded-qualifiers
CFLAGS +=  -Wno-deprecated-declarations -Wextra -Wwrite-strings -Wno-unused-parameter 
CFLAGS += -g -O2
#CFLAGS += -Werror=parentheses
#-std=gnu99 -D_GNU_SOURCE -D_REEN^CANT -g -O2
CFLAGS +=-I$(TARGET_DIR)/usr/include  -I./
CFLAGS += -I./ -I./libs/ -I$(TOOLCHAIN_DIR)/usr/include

LDFLAGS = -L$(TARGET_DIR)/usr/lib/ -ljson-c -lpcap -lpthread
vpath %.c ./libs
vpath %.h ./libs

all: main
LIB=$(TARGET_DIR)/usr/lib/libjson-c.so


$(all_src:.c=.o): %.o : %.c
	$(CC) $(CFLAGS)  $^ -c -o $@


main: $(progam_obj) $(lib_obj)
#	$(CC) $(LDFLAGS) -Wl,-rpath=./  -L./json-c/.libs  -ljson-c $^ $(LIB) -o $@ 
	$(CC) $(LDFLAGS) -Wl,-rpath=./  $(CFLAGS) $^ $(LIB) -o $@ 
#	$(CC) $(LDFLAGS) -Wl,-rpath=./  $^ -o $@ -L./ ./json-c/.libs/libjson-c.so


clean:
	rm main *.o -f
