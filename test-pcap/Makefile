

#########################################
#
#target : all,lib env,install
#
##########################################
CXX = gcc
CXXFLAGS += -Wall
#CXXLDFLAGS=;

src = main.c
obj = $(src:.c=.o)
progam = test-pcap
CXXLDF = -lpthread

all: t $(progam)

t:
	@echo $(src)
	@echo $(obj)



$(src:.c=.o): %.o : %.c
	$(CXX) $(CXXFLAGS)  $^ -c -o $@

$(progam): $(obj)
	$(CXX) $(CXXFLAGS)  $^ -o $@ $(CXXLDF) 

clean:
	@rm $(progam) *.o -f
