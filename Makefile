#OBJS specifies which files to compile as part of the project
MAIN = sb.cpp
SRC = ./src/glad.c ./src/simdjson.cpp button.cpp opt_window.cpp opt_field.cpp save_config.cpp


OBJS = $(MAIN) $(SRC)
INCL = -I ./include -I ./include/learnopengl -I/usr/include/freetype2 -I/usr/include/SDL2

#CC specifies which compiler we're using
CC = g++

#COMPILER_FLAGS specifies the additional compilation options we're using
# -w suppresses all warnings
COMPILER_FLAGS = -std=c++17 -Wall

#LINKER_FLAGS specifies the libraries we're linking against
LINKER_FLAGS = -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer -lGLU -lGL -ldl -lfreetype -lstdc++fs

#OBJ_NAME specifies the name of our exectuable
OBJ_NAME = button

#This is the target that compiles our executable
all : $(OBJS)
	$(CC) $(INCL) $(OBJS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)

