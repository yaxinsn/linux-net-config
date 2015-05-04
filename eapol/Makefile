

KERDIR = /usr/src/kernels/2.6.32-431.29.2.el6.i686
obj-m := fpp_1xclient.o
fpp_1xclient-objs := eapol.o
.PHONY: all clean


all:
	$(MAKE) -C $(KERDIR) M=$(PWD) modules

clean:
	$(MAKE) -C $(KERDIR) M=$(PWD) clean
	-rm -f Module.symvers Module.markers modules.order
