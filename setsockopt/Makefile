

KERDIR = /usr/src/kernels/2.6.32-431.29.2.el6.i686
obj-m := netfiltersetopt.o
netfiltersetopt-objs := netfilter_setopt.o
.PHONY: all clean

all: kernel-all app

app:
	gcc  main.c -o app 
	chmod +x app

kernel-all:
	$(MAKE) -C $(KERDIR) M=$(PWD) modules

clean:
	$(MAKE) -C $(KERDIR) M=$(PWD) clean
	-rm -f Module.symvers Module.markers modules.order 
	-rm -f app
