#The compiler: gcc for C programs, define as g++ for C++
CC = gcc
RM = rm

# The build target executable
TARGET = atm

#Defines any directories containing header files other than /usr/include
INCLUDES = -I.

#Defines library paths in addition to /usr/lib
#LFLAGS = -Llib

#Defines any libraries to link into executable
#LIB = -lmylib -m

#Define source files
SRC = src/main.c src/atm.c
OBJS = $(SRC:.c=.o)

#Compiler flags:
# -g     adds debugging information to the executable file
# -Wall  turns on most, but not all, compiler warnings
#CFLAGS = -g -Wall -I$(INCLUDES)

.PHONY:

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(INCLUDES) -o $(TARGET) $(OBJS) $(LFLAGS) $(LIBS)
.c.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@
clean:
	$(RM) -rf $(TARGET)\
		*.dSYM \
		$(OBJS) \