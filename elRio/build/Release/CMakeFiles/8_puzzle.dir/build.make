# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.20

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
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
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/solrak/Documents/Inteligencia-Artificial/elRio

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/solrak/Documents/Inteligencia-Artificial/elRio/build/Release

# Include any dependencies generated for this target.
include CMakeFiles/8_puzzle.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/8_puzzle.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/8_puzzle.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/8_puzzle.dir/flags.make

CMakeFiles/8_puzzle.dir/src/8_puzzle.cpp.o: CMakeFiles/8_puzzle.dir/flags.make
CMakeFiles/8_puzzle.dir/src/8_puzzle.cpp.o: ../../src/8_puzzle.cpp
CMakeFiles/8_puzzle.dir/src/8_puzzle.cpp.o: CMakeFiles/8_puzzle.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/solrak/Documents/Inteligencia-Artificial/elRio/build/Release/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/8_puzzle.dir/src/8_puzzle.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/8_puzzle.dir/src/8_puzzle.cpp.o -MF CMakeFiles/8_puzzle.dir/src/8_puzzle.cpp.o.d -o CMakeFiles/8_puzzle.dir/src/8_puzzle.cpp.o -c /home/solrak/Documents/Inteligencia-Artificial/elRio/src/8_puzzle.cpp

CMakeFiles/8_puzzle.dir/src/8_puzzle.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/8_puzzle.dir/src/8_puzzle.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/solrak/Documents/Inteligencia-Artificial/elRio/src/8_puzzle.cpp > CMakeFiles/8_puzzle.dir/src/8_puzzle.cpp.i

CMakeFiles/8_puzzle.dir/src/8_puzzle.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/8_puzzle.dir/src/8_puzzle.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/solrak/Documents/Inteligencia-Artificial/elRio/src/8_puzzle.cpp -o CMakeFiles/8_puzzle.dir/src/8_puzzle.cpp.s

# Object files for target 8_puzzle
8_puzzle_OBJECTS = \
"CMakeFiles/8_puzzle.dir/src/8_puzzle.cpp.o"

# External object files for target 8_puzzle
8_puzzle_EXTERNAL_OBJECTS =

8_puzzle: CMakeFiles/8_puzzle.dir/src/8_puzzle.cpp.o
8_puzzle: CMakeFiles/8_puzzle.dir/build.make
8_puzzle: CMakeFiles/8_puzzle.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/solrak/Documents/Inteligencia-Artificial/elRio/build/Release/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable 8_puzzle"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/8_puzzle.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/8_puzzle.dir/build: 8_puzzle
.PHONY : CMakeFiles/8_puzzle.dir/build

CMakeFiles/8_puzzle.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/8_puzzle.dir/cmake_clean.cmake
.PHONY : CMakeFiles/8_puzzle.dir/clean

CMakeFiles/8_puzzle.dir/depend:
	cd /home/solrak/Documents/Inteligencia-Artificial/elRio/build/Release && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/solrak/Documents/Inteligencia-Artificial/elRio /home/solrak/Documents/Inteligencia-Artificial/elRio /home/solrak/Documents/Inteligencia-Artificial/elRio/build/Release /home/solrak/Documents/Inteligencia-Artificial/elRio/build/Release /home/solrak/Documents/Inteligencia-Artificial/elRio/build/Release/CMakeFiles/8_puzzle.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/8_puzzle.dir/depend
