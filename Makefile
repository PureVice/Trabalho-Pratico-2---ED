#
# Copyright ...
#

# cc and flags
CC = g++
CXXFLAGS = -std=c++11 -g -Wall
#CXXFLAGS = -std=c++11 -O3 -Wall

# folders
INCLUDE_FOLDER = ./include/
BIN_FOLDER = ./bin/
OBJ_FOLDER = ./obj/
SRC_FOLDER = ./src/

# all sources, objs, and header files
MAIN = Main
TARGET = tp3.out
SRC = $(wildcard $(SRC_FOLDER)*.cc)
OBJ = $(patsubst $(SRC_FOLDER)%.cc, $(OBJ_FOLDER)%.o, $(SRC))

# Garante que diretórios existam antes de compilar objetos
$(OBJ_FOLDER)%.o: $(SRC_FOLDER)%.cc | create_dirs
	$(CC) $(CXXFLAGS) -c $< -o $@ -I$(INCLUDE_FOLDER)

# Regra principal
all: create_dirs $(OBJ)
	$(CC) $(CXXFLAGS) -o $(BIN_FOLDER)$(TARGET) $(OBJ)
	./bin/$(TARGET) teste.wkl

# Regra para criação dos diretórios
create_dirs:
	@mkdir -p $(BIN_FOLDER)
	@mkdir -p $(OBJ_FOLDER)

clean:
	@rm -rf $(OBJ_FOLDER)* $(BIN_FOLDER)*

run2: all
	./bin/$(TARGET) teste.txt