# Makefile for Modular Multithreaded HTTP Server

CC = gcc
CFLAGS = -Wall -std=c11
TARGET = server
OBJS = main.o request.o response.o client_handler.o add_handler.o mul_handler.o div_handler.o static_handler.o

all: format $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) -lpthread

main.o: main.c client_handler.h
	$(CC) $(CFLAGS) -c main.c

client_handler.o: client_handler.c client_handler.h request.h response.h add_handler.h mul_handler.h div_handler.h static_handler.h
	$(CC) $(CFLAGS) -c client_handler.c

add_handler.o: add_handler.c add_handler.h response.h
	$(CC) $(CFLAGS) -c add_handler.c

mul_handler.o: mul_handler.c mul_handler.h response.h
	$(CC) $(CFLAGS) -c mul_handler.c

div_handler.o: div_handler.c div_handler.h response.h
	$(CC) $(CFLAGS) -c div_handler.c

static_handler.o: static_handler.c static_handler.h response.h
	$(CC) $(CFLAGS) -c static_handler.c

request.o: request.c request.h
	$(CC) $(CFLAGS) -c request.c

response.o: response.c response.h
	$(CC) $(CFLAGS) -c response.c

clean:
	rm -f $(TARGET) $(OBJS)

format:
	@echo "Formatting C source files with clang-format..."
	clang-format -i *.c *.h
