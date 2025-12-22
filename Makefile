CC = gcc
# -std=c11 forces the compiler to use the C11 standard
CFLAGS = -I munit -std=c11

# List all your C files here
SRCS = main.c oxyobject.c munit/munit.c

# The name of the output executable
TARGET = tests

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRCS)

clean:
	rm -f $(TARGET)