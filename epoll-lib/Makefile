
TARGET=epoll_lib.a



CSOURCE=epoll_lib.c
COBJS=$(CSOURCE:.c=.o)

OBJS=$(COBJS)

all: $(TARGET) client service


$(COBJS): %.o: %.c
	gcc $(CFLAGS) -c $< -o $@

$(TARGET): $(OBJS)
	ar -rc -o $@ $(OBJS) $(LIBS)



service:unix_service.o $(TARGET)
	gcc $(CFLAGS) -o $@ $^

client:	unix_client.c
	gcc $(CFLAGS) -o $@ $<


clean:
	rm  *.o $(TARGET) client -rf

