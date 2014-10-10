

#########################################
#
#target : all,lib env,install
#
##########################################
CXX = gcc
CXXFLAGS += -Wall
#CXXLDFLAGS=;

src = syn-flood.c
obj = $(src:.c=.o)
progam = synflood

all: t $(progam)

t:
	@echo $(src)
	@echo $(obj)



$(src:.c=.o): %.o : %.c
	$(CXX) $(CXXFLAGS) $^ -c -o $@

$(progam): $(obj)
	$(CXX) $(CXXFLAGS) $^ -o $@

clean:
	@rm $(progam) *.o -f
