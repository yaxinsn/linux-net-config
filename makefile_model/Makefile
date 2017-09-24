

#########################################
#
#target : all,lib env,install
#
##########################################
CXX = gcc
CXXFLAGS += -Wall
LDFLAGS= 

#src = $(wildcard *.c)
#obj = $(src:.c=.o)

vpath %.c ./add
vpath %.h ./add
vpath %.cpp ./add

CXXFLAGS += -I./add/

progam_src = main.c
progam_obj =  $(progam_src:.c=.o)

lib_src = libadd.c
lib_obj =  $(lib_src:.c=.o)

progam = program

lib=libadd.so
all:  $(lib) $(progam)

t:
	@echo $(src)
	@echo $(obj)

COMPILE_MODE=1

ifeq ($(COMPILE_MODE),1)
$(warning compile mode 1)

$(progam_src:.c=.o): %.o : %.c
	$(CXX) $(CXXFLAGS) $^ -c -o $@

$(lib_src:.c=.o): %.o : %.c
	$(CXX) $(CXXFLAGS) -fPIC $^ -c -o $@
else
$(warning compile mode 0)
%.o : %.c
	$(CXX) $(CXXFLAGS) -c -o $@ $(CXXFLAGS) $<
endif

$(lib): $(lib_obj) 
	$(CXX) $(LDFLAGS)  $^ -shared -o $@

$(progam): $(progam_obj) $(lib)
	$(CXX) $(LDFLAGS) -Wl,-rpath=./add  -Wl,-rpath=./ -ladd -L./ $^ -o $@

clean:
	@rm $(progam) $(lib) *.o -f
