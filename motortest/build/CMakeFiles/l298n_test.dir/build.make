# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.18

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
CMAKE_SOURCE_DIR = /home/pi/motortest

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/pi/motortest/build

# Include any dependencies generated for this target.
include CMakeFiles/l298n_test.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/l298n_test.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/l298n_test.dir/flags.make

CMakeFiles/l298n_test.dir/main.cpp.o: CMakeFiles/l298n_test.dir/flags.make
CMakeFiles/l298n_test.dir/main.cpp.o: ../main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/pi/motortest/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/l298n_test.dir/main.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/l298n_test.dir/main.cpp.o -c /home/pi/motortest/main.cpp

CMakeFiles/l298n_test.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/l298n_test.dir/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/pi/motortest/main.cpp > CMakeFiles/l298n_test.dir/main.cpp.i

CMakeFiles/l298n_test.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/l298n_test.dir/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/pi/motortest/main.cpp -o CMakeFiles/l298n_test.dir/main.cpp.s

# Object files for target l298n_test
l298n_test_OBJECTS = \
"CMakeFiles/l298n_test.dir/main.cpp.o"

# External object files for target l298n_test
l298n_test_EXTERNAL_OBJECTS =

l298n_test: CMakeFiles/l298n_test.dir/main.cpp.o
l298n_test: CMakeFiles/l298n_test.dir/build.make
l298n_test: /usr/lib/libpigpio.so
l298n_test: CMakeFiles/l298n_test.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/pi/motortest/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable l298n_test"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/l298n_test.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/l298n_test.dir/build: l298n_test

.PHONY : CMakeFiles/l298n_test.dir/build

CMakeFiles/l298n_test.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/l298n_test.dir/cmake_clean.cmake
.PHONY : CMakeFiles/l298n_test.dir/clean

CMakeFiles/l298n_test.dir/depend:
	cd /home/pi/motortest/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/pi/motortest /home/pi/motortest /home/pi/motortest/build /home/pi/motortest/build /home/pi/motortest/build/CMakeFiles/l298n_test.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/l298n_test.dir/depend

