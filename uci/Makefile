


ifeq ($(AP_GCC_PLATFORM),LSDK)
CCFLAGS +=-DAP_PLATFORM_LSDK
else ifeq ($(AP_GCC_PLATFORM),QSDK)# QSDK
CCFLAGS +=-DAP_PLATFORM_QSDK
endif


############################################################################################
EXEC_SRC = uci.c
EXEC_NAME=uci

LIB_SRC =  uci_lib.c
LIB_NAME = libuci.so
.PHONY: clean

# top-level rule, to compile everything. 
all : $(LIB_NAME) $(EXEC_NAME)

CCFLAGS += -g
$(LIB_SRC:.c=.o): %.o : %.c
	$(CC) $(CCFLAGS)  -fPIC -I$(TOOLCHAIN)/usr/include $^ -c -o $@ -Wall

$(LIB_NAME): $(LIB_SRC:.c=.o)
	$(CC) $^ $(LDFLAGS)  -shared  -o $@ -Wall

$(EXEC_SRC:.c=.o): %.o : %.c
	$(CC) $(CCFLAGS)  -I$(TOOLCHAIN)/usr/include $^ -c -o $@ -Wall

$(EXEC_NAME): $(EXEC_SRC:.c=.o)
	$(CC)  $^ $(LDFLAGS) -L./ -luci  -o  $@
INST:
	/bin/cp $(EXEC_NAME) $(RELEASE_DIR)/sbin/ -f	
	/bin/cp $(LIB_NAME) $(RELEASE_DIR)/lib/ -f	

clean:
	rm -f $(LIB_NAME) *.o *.so $(EXEC_NAME)
