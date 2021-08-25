CC=g++
FLAGS = libglfw.so.3.4 -lGL -lX11 -lpthread -lXrandr -lXi -ldl
OUTPUT = cmt
OBJ_DIR = ./objectDirectory
OBJ_FILES = $(OBJ_DIR)/cubemap.o $(OBJ_DIR)/glad.o $(OBJ_DIR)/shader.o $(OBJ_DIR)/camera.o


vpath %.c ./glad/src/
vpath %.cpp ./shaderClass/
vpath %.cpp ./
vpath %.o $(OBJ_DIR)

cmt : $(OBJ_FILES) 
	$(CC) $^ -o $(OUTPUT) $(FLAGS)

$(OBJ_DIR)/cubemap.o : cubemap.cpp
	$(CC) -c $^ -o $@

$(OBJ_DIR)/shader.o : shader.cpp 
	$(CC) -c $^ -o $@

$(OBJ_DIR)/camera.o : camera.cpp 
	$(CC) -c $^ -o $@

$(OBJ_DIR)/glad.o : glad.c
	$(CC) -c $^ -o $@

.PHONY: clean
clean:
	rm -rf $(OBJ_DIR)/*.o $(OUTPUT)
