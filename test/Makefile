##################################################################################
SRCDIR	= ../src
LIBDIR	= ../lib
INCDIR	= ../.

APP	= atm_test
##################################################################################
#                                   SQLITE
##################################################################################
CSRCS := \
	$(SRCDIR)/atm.c \
	$(SRCDIR)/atm_usb.c

OBJS := $(CSRCS:.c=.o)

%.o: %.c
	$(CC) $(CFLAGS) -c $^ -o $@
##################################################################################
#                                   GCC FLAGS
##################################################################################
CFLAGS := \
	-Wall -Wextra -Werror -O0 -g -std=gnu99 \
	-I$(INCDIR) \
	-I$(LIBDIR)

CXXFLAGS := \
	-fexceptions -Wall -Wextra -Werror -O0 -g -std=c++11 \
	-I$(INCDIR) \
	-I$(LIBDIR) \
	-I/usr/local/include/

LDFLAGS := -ldl -lgtest -lpthread -lusb-1.0
##################################################################################
#                              MAIN MAKEFILE
##################################################################################
.PHONY: all
all: $(APP)

lib$(APP).a: $(OBJS)
	$(AR) rcs lib$(APP).a $(OBJS)

$(APP): lib$(APP).a
	$(CXX) $(CXXFLAGS) $(APP).cc -o $(APP) lib$(APP).a $(LDFLAGS)

check:
	./$(APP)

clean: 
	rm  -rf $(APP) lib$(APP).a \
		$(OBJS) \
		*.dSYM
