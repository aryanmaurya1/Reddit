CC=gcc
LINKER_INCLUDE_PATHS=-L./lib
PREPROCESSOR_INCLUDE_PATHS=-I./include
DEPS=-lglfw3 -lGL -lX11 -lm
OBJ_FILES=build/main.o build/glad.o build/utils.o
BIN=build/bin/window

all: main glad utils
	$(CC) $(OBJ_FILES) -o $(BIN) $(LINKER_INCLUDE_PATHS) $(DEPS)
main: src/main.c
	$(CC) -c src/main.c -o build/main.o $(PREPROCESSOR_INCLUDE_PATHS)
glad: src/glad.c
	$(CC) -c src/glad.c -o build/glad.o $(PREPROCESSOR_INCLUDE_PATHS)
utils: src/utils.c
	$(CC) -c src/utils.c -o build/utils.o $(PREPROCESSOR_INCLUDE_PATHS)
clean:
	rm $(BIN) $(OBJ_FILES)