

#########################################
#
#target : all,lib env,install
#
##########################################

HEADER = -I/usr/include
HEADER += -I./ -I./libnl-3.2.25/include/netlink -I./libnl-3.2.25/include/


###################################

CXX = gcc
CXXFLAGS += -Wall $(HEADER) 


LDFLAGS = -Wl,-rpath=/work/test-nl/libnl-3.2.25/lib/.libs -L/work/test-nl/libnl-3.2.25/lib/.libs -lnl-3
#CXXLDFLAGS=;
#############################################

src = main.c
obj = $(src:.c=.o)
progam = test-nl

all:  $(progam)

t:
	@echo $(src)
	@echo $(obj)



$(src:.c=.o): %.o : %.c
	$(CXX) $(CXXFLAGS) $^ -c -o $@

$(progam): $(obj)
	$(CXX) $(LDFLAGS) $^ -o $@

clean:
	@rm $(progam) *.o -f
