#The compiler: gcc for C programs, define as g++ for C++
CC 		= gcc
RM 		= rm
AR		= ar
MKDIR 	= mkdir -p

OUTDIR	= exe
SRCDIR	= src
LIBDIR	= lib
TESTDIR	= test
OBJDIR	= obj
TARGET 	= atm

##################################################################################
#                                   SRCS
##################################################################################
#Define source files
SRC = \
	$(SRCDIR)/main.c \
	$(SRCDIR)/atm.c \
	$(SRCDIR)/atm_usb.c

OBJS := $(SRC:.c=.o)

%.o: %.c
	$(CC) $(CFLAGS) -c $^ -o $@
##################################################################################
#                                   GCC FLAGS
##################################################################################
#Compiler flags:
# -g     adds debugging information to the executable file
# -Wall  turns on most, but not all, compiler warnings
CFLAGS = -g -Wall -Wextra -O0 -std=gnu99 \
		 -I. \
		 -I$(LIBDIR)

#Defines any libraries to link into executable
LIB = -lgtest -lpthread -lusb-1.0

#Defines library paths in addition to /usr/lib
LDFLAGS = -L/usr/local/lib $(LIB) 
##################################################################################
#                              MAIN MAKEFILE
##################################################################################
.PHONY:

all: dir $(TARGET)

dir:
	${MKDIR} ${OUTDIR}

lib$(TARGET).a: $(OBJS)
	$(AR) rcs lib$(TARGET).a $(OBJS)

$(TARGET): lib$(TARGET).a
	$(CC) $(CFLAGS) $(SRCDIR)/$@.c -o $(OUTDIR)/$@ lib$(TARGET).a $(LDFLAGS) 

#.c.o:
#	$(CC) $(CFLAGS) -c $< -o $@

#check:
#	$(CXX) -I/usr/local/include/ $(TEST) $(LDFLAGS) $(LIB)

test: clean build_test

clean: clean_test
	$(RM) -rf $(OUTDIR) \
		*.dSYM \
		$(OBJS) \
		.DS_Store inc/.DS_Store \
		lib$(TARGET).a
##################################################################################
#                              TEST
##################################################################################
build_test:
	$(call make-test)
clean_test:
	$(call clean-test)
run_test:
	$(call run-test)
##################################################################################
#                              DEFINES
##################################################################################
define make-test
	cd $(TESTDIR); \
	make;
endef
define clean-test
	cd $(TESTDIR); \
	make clean;
endef
define run-test
	cd $(TESTDIR); \
	make check;
endef