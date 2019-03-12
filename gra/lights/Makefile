#CC specifies which compiler we're using
CC = g++

BASE_DIR := ./src/G53GRA.Framework/G53GRA.Framework
SC_H := $(BASE_DIR)/Framework/Engine
DS_H := $(BASE_DIR)/Framework/Interface
#COMPILER_FLAGS specifies the additional compilation options we're using
# -w suppresses all warnings
COMPILER_FLAGS = -I$(SC_H) -I$(DS_H) -g -std=c++11 -DGL_GLEXT_PROTOTYPES

#LINKER_FLAGS specifies the libraries we're linking against
LINKER_FLAGS = -lGL -lGLU -lglut -lSOIL

# find cpp files in subdirectories
SOURCES := $(shell find . -name '*.cpp')

#OBJ_NAME specifies the name of our exectuable
OBJ_NAME = program.exe

#This is the target that compiles our executable
all : $(SOURCES)
	$(CC) $(SOURCES) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)