
TARGET=epoll_lib.a



CSOURCE=epoll_lib.c
COBJS=$(CSOURCE:.c=.o)

OBJS=$(COBJS)

all: $(TARGET)


$(COBJS): %.o: %.c
	gcc $(CFLAGS) -c $< -o $@

$(TARGET): $(OBJS)
	ar -rc -o $@ $(OBJS) $(LIBS)




clean:
	rm e *.o $(TARGET) -rf

