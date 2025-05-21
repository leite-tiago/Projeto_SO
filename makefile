# Grupo 40
# Rodrigo Frutuoso - 61865
# Simão Alexandre - 61874
# Tiago Leite - 61863

BIN_DIR = bin
INC_DIR = inc
OBJ_DIR = obj
SRC_DIR = src
CC = gcc
# CFLAGS = -Wno-deprecated-declarations

OBJECTS_FILES = main.o memory.o process.o server.o wallet.o clog.o csettings.o csignal.o cstats.o ctime.o synchronization.o
OBJECTS = $(addprefix $(OBJ_DIR)/, $(OBJECTS_FILES))

all: $(OBJECTS)
	@$(CC) $(OBJECTS) -o $(BIN_DIR)/SOchain

clean:
	@rm -f $(OBJ_DIR)/*
	@rm -f $(BIN_DIR)/SOchain

re: clean all

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@$(CC) $(CFLAGS) -c $< -o $@ -I $(INC_DIR)
