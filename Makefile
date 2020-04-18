#The compiler: gcc for C programs, define as g++ for C++
CC = gcc
CXX = g++
RM = rm

# The build target executable
TARGET = atm

#Defines any directories containing header files other than /usr/include
INCLUDES = .

#Defines library paths in addition to /usr/lib
#LFLAGS = -Llib
LDFLAGS = -L/usr/local/lib -lgtest -l pthread

#Defines any libraries to link into executable
#LIB = -lmylib -m

#Define source files
SRC = src/main.c src/atm.c
OBJS = $(SRC:.c=.o)
TEST = test/test.cc

#Compiler flags:
# -g     adds debugging information to the executable file
# -Wall  turns on most, but not all, compiler warnings
CFLAGS = -g -Wall -I$(INCLUDES)

.PHONY:

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) $(LFLAGS) $(LIBS)

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

check:
	$(CXX) -I/usr/local/include/ $(TEST) $(LDFLAGS)

clean:
	$(RM) -rf $(TARGET)\
		*.dSYM \
		$(OBJS) \
		.DS_Store\
