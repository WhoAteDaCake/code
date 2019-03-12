#CC specifies which compiler we're using
CC = g++

#COMPILER_FLAGS specifies the additional compilation options we're using
# -w suppresses all warnings
COMPILER_FLAGS = -I./src -g -std=c++11 -DGL_GLEXT_PROTOTYPES

#LINKER_FLAGS specifies the libraries we're linking against
LINKER_FLAGS = -lGL -lGLU -lglut -lSOIL

# find cpp files in subdirectories
SOURCES := $(shell find . -name '*.cpp')

# find headers
HEADERS := $(shell find . -name '*.h')

#OBJ_NAME specifies the name of our exectuable
# OBJ_NAME = program.exe

# #This is the target that compiles our executable
# all : $(OBJS)
# 	$(CC) $(OBJS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)
# Everything depends on the output
OUTPUT := ttt

all: $(OUTPUT)

# The output depends on sources and headers
$(OUTPUT): $(SOURCES) $(HEADERS)
	$(CC) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OUTPUT) $(SOURCES)

clean:
	$(RM) $(OUTPUT)