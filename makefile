CC = g++
CFLAGS = -Wall -g

SRC_DIR ?= ./src
LIB_DIR ?= ./lib
OBJ_DIR ?= ./obj
BIN_DIR ?= ./bin
INC_DIR ?= ./include

${BIN_DIR}/main: main.o serialib.o
	$(CC) $(CFLAGS) -o $(BIN_DIR)/main ${OBJ_DIR}/main.o ${OBJ_DIR}/serialib.o -lncurses

main.o: ${SRC_DIR}/main.cpp
	$(CC) $(CFLAGS) -c ${SRC_DIR}/main.cpp -o ${OBJ_DIR}/main.o

serialib.o: ${LIB_DIR}/serialib.cpp
	$(CC) $(CFLAGS) -c ${LIB_DIR}/serialib.cpp -o ${OBJ_DIR}/serialib.o