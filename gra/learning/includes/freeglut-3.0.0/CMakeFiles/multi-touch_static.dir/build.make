# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.10

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/august/uni/code/gra/learning/includes/freeglut-3.0.0

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/august/uni/code/gra/learning/includes/freeglut-3.0.0

# Include any dependencies generated for this target.
include CMakeFiles/multi-touch_static.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/multi-touch_static.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/multi-touch_static.dir/flags.make

CMakeFiles/multi-touch_static.dir/progs/demos/multi-touch/multi-touch.c.o: CMakeFiles/multi-touch_static.dir/flags.make
CMakeFiles/multi-touch_static.dir/progs/demos/multi-touch/multi-touch.c.o: progs/demos/multi-touch/multi-touch.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/august/uni/code/gra/learning/includes/freeglut-3.0.0/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/multi-touch_static.dir/progs/demos/multi-touch/multi-touch.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/multi-touch_static.dir/progs/demos/multi-touch/multi-touch.c.o   -c /home/august/uni/code/gra/learning/includes/freeglut-3.0.0/progs/demos/multi-touch/multi-touch.c

CMakeFiles/multi-touch_static.dir/progs/demos/multi-touch/multi-touch.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/multi-touch_static.dir/progs/demos/multi-touch/multi-touch.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/august/uni/code/gra/learning/includes/freeglut-3.0.0/progs/demos/multi-touch/multi-touch.c > CMakeFiles/multi-touch_static.dir/progs/demos/multi-touch/multi-touch.c.i

CMakeFiles/multi-touch_static.dir/progs/demos/multi-touch/multi-touch.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/multi-touch_static.dir/progs/demos/multi-touch/multi-touch.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/august/uni/code/gra/learning/includes/freeglut-3.0.0/progs/demos/multi-touch/multi-touch.c -o CMakeFiles/multi-touch_static.dir/progs/demos/multi-touch/multi-touch.c.s

CMakeFiles/multi-touch_static.dir/progs/demos/multi-touch/multi-touch.c.o.requires:

.PHONY : CMakeFiles/multi-touch_static.dir/progs/demos/multi-touch/multi-touch.c.o.requires

CMakeFiles/multi-touch_static.dir/progs/demos/multi-touch/multi-touch.c.o.provides: CMakeFiles/multi-touch_static.dir/progs/demos/multi-touch/multi-touch.c.o.requires
	$(MAKE) -f CMakeFiles/multi-touch_static.dir/build.make CMakeFiles/multi-touch_static.dir/progs/demos/multi-touch/multi-touch.c.o.provides.build
.PHONY : CMakeFiles/multi-touch_static.dir/progs/demos/multi-touch/multi-touch.c.o.provides

CMakeFiles/multi-touch_static.dir/progs/demos/multi-touch/multi-touch.c.o.provides.build: CMakeFiles/multi-touch_static.dir/progs/demos/multi-touch/multi-touch.c.o


# Object files for target multi-touch_static
multi__touch_static_OBJECTS = \
"CMakeFiles/multi-touch_static.dir/progs/demos/multi-touch/multi-touch.c.o"

# External object files for target multi-touch_static
multi__touch_static_EXTERNAL_OBJECTS =

bin/multi-touch_static: CMakeFiles/multi-touch_static.dir/progs/demos/multi-touch/multi-touch.c.o
bin/multi-touch_static: CMakeFiles/multi-touch_static.dir/build.make
bin/multi-touch_static: /usr/lib/x86_64-linux-gnu/libGLU.so
bin/multi-touch_static: /usr/lib/x86_64-linux-gnu/libGL.so
bin/multi-touch_static: /usr/lib/x86_64-linux-gnu/libSM.so
bin/multi-touch_static: /usr/lib/x86_64-linux-gnu/libICE.so
bin/multi-touch_static: /usr/lib/x86_64-linux-gnu/libX11.so
bin/multi-touch_static: /usr/lib/x86_64-linux-gnu/libXext.so
bin/multi-touch_static: /usr/lib/x86_64-linux-gnu/libXrandr.so
bin/multi-touch_static: /usr/lib/x86_64-linux-gnu/libXxf86vm.so
bin/multi-touch_static: /usr/lib/x86_64-linux-gnu/libXi.so
bin/multi-touch_static: lib/libglut.a
bin/multi-touch_static: /usr/lib/x86_64-linux-gnu/libGL.so
bin/multi-touch_static: /usr/lib/x86_64-linux-gnu/libSM.so
bin/multi-touch_static: /usr/lib/x86_64-linux-gnu/libICE.so
bin/multi-touch_static: /usr/lib/x86_64-linux-gnu/libX11.so
bin/multi-touch_static: /usr/lib/x86_64-linux-gnu/libXext.so
bin/multi-touch_static: /usr/lib/x86_64-linux-gnu/libXrandr.so
bin/multi-touch_static: /usr/lib/x86_64-linux-gnu/libXxf86vm.so
bin/multi-touch_static: /usr/lib/x86_64-linux-gnu/libXi.so
bin/multi-touch_static: CMakeFiles/multi-touch_static.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/august/uni/code/gra/learning/includes/freeglut-3.0.0/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable bin/multi-touch_static"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/multi-touch_static.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/multi-touch_static.dir/build: bin/multi-touch_static

.PHONY : CMakeFiles/multi-touch_static.dir/build

CMakeFiles/multi-touch_static.dir/requires: CMakeFiles/multi-touch_static.dir/progs/demos/multi-touch/multi-touch.c.o.requires

.PHONY : CMakeFiles/multi-touch_static.dir/requires

CMakeFiles/multi-touch_static.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/multi-touch_static.dir/cmake_clean.cmake
.PHONY : CMakeFiles/multi-touch_static.dir/clean

CMakeFiles/multi-touch_static.dir/depend:
	cd /home/august/uni/code/gra/learning/includes/freeglut-3.0.0 && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/august/uni/code/gra/learning/includes/freeglut-3.0.0 /home/august/uni/code/gra/learning/includes/freeglut-3.0.0 /home/august/uni/code/gra/learning/includes/freeglut-3.0.0 /home/august/uni/code/gra/learning/includes/freeglut-3.0.0 /home/august/uni/code/gra/learning/includes/freeglut-3.0.0/CMakeFiles/multi-touch_static.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/multi-touch_static.dir/depend

