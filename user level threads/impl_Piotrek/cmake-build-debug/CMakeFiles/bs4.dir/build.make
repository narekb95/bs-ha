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
CMAKE_COMMAND = /home/se2/Desktop/clion-2018.1.3/bin/cmake/bin/cmake

# The command to remove a file.
RM = /home/se2/Desktop/clion-2018.1.3/bin/cmake/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = "/home/se2/git/bs-ha/user level threads/impl_Piotrek"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "/home/se2/git/bs-ha/user level threads/impl_Piotrek/cmake-build-debug"

# Include any dependencies generated for this target.
include CMakeFiles/bs4.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/bs4.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/bs4.dir/flags.make

CMakeFiles/bs4.dir/main.c.o: CMakeFiles/bs4.dir/flags.make
CMakeFiles/bs4.dir/main.c.o: ../main.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/home/se2/git/bs-ha/user level threads/impl_Piotrek/cmake-build-debug/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/bs4.dir/main.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/bs4.dir/main.c.o   -c "/home/se2/git/bs-ha/user level threads/impl_Piotrek/main.c"

CMakeFiles/bs4.dir/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/bs4.dir/main.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E "/home/se2/git/bs-ha/user level threads/impl_Piotrek/main.c" > CMakeFiles/bs4.dir/main.c.i

CMakeFiles/bs4.dir/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/bs4.dir/main.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S "/home/se2/git/bs-ha/user level threads/impl_Piotrek/main.c" -o CMakeFiles/bs4.dir/main.c.s

CMakeFiles/bs4.dir/main.c.o.requires:

.PHONY : CMakeFiles/bs4.dir/main.c.o.requires

CMakeFiles/bs4.dir/main.c.o.provides: CMakeFiles/bs4.dir/main.c.o.requires
	$(MAKE) -f CMakeFiles/bs4.dir/build.make CMakeFiles/bs4.dir/main.c.o.provides.build
.PHONY : CMakeFiles/bs4.dir/main.c.o.provides

CMakeFiles/bs4.dir/main.c.o.provides.build: CMakeFiles/bs4.dir/main.c.o


CMakeFiles/bs4.dir/array.c.o: CMakeFiles/bs4.dir/flags.make
CMakeFiles/bs4.dir/array.c.o: ../array.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/home/se2/git/bs-ha/user level threads/impl_Piotrek/cmake-build-debug/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/bs4.dir/array.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/bs4.dir/array.c.o   -c "/home/se2/git/bs-ha/user level threads/impl_Piotrek/array.c"

CMakeFiles/bs4.dir/array.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/bs4.dir/array.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E "/home/se2/git/bs-ha/user level threads/impl_Piotrek/array.c" > CMakeFiles/bs4.dir/array.c.i

CMakeFiles/bs4.dir/array.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/bs4.dir/array.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S "/home/se2/git/bs-ha/user level threads/impl_Piotrek/array.c" -o CMakeFiles/bs4.dir/array.c.s

CMakeFiles/bs4.dir/array.c.o.requires:

.PHONY : CMakeFiles/bs4.dir/array.c.o.requires

CMakeFiles/bs4.dir/array.c.o.provides: CMakeFiles/bs4.dir/array.c.o.requires
	$(MAKE) -f CMakeFiles/bs4.dir/build.make CMakeFiles/bs4.dir/array.c.o.provides.build
.PHONY : CMakeFiles/bs4.dir/array.c.o.provides

CMakeFiles/bs4.dir/array.c.o.provides.build: CMakeFiles/bs4.dir/array.c.o


CMakeFiles/bs4.dir/ult.c.o: CMakeFiles/bs4.dir/flags.make
CMakeFiles/bs4.dir/ult.c.o: ../ult.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/home/se2/git/bs-ha/user level threads/impl_Piotrek/cmake-build-debug/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_3) "Building C object CMakeFiles/bs4.dir/ult.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/bs4.dir/ult.c.o   -c "/home/se2/git/bs-ha/user level threads/impl_Piotrek/ult.c"

CMakeFiles/bs4.dir/ult.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/bs4.dir/ult.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E "/home/se2/git/bs-ha/user level threads/impl_Piotrek/ult.c" > CMakeFiles/bs4.dir/ult.c.i

CMakeFiles/bs4.dir/ult.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/bs4.dir/ult.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S "/home/se2/git/bs-ha/user level threads/impl_Piotrek/ult.c" -o CMakeFiles/bs4.dir/ult.c.s

CMakeFiles/bs4.dir/ult.c.o.requires:

.PHONY : CMakeFiles/bs4.dir/ult.c.o.requires

CMakeFiles/bs4.dir/ult.c.o.provides: CMakeFiles/bs4.dir/ult.c.o.requires
	$(MAKE) -f CMakeFiles/bs4.dir/build.make CMakeFiles/bs4.dir/ult.c.o.provides.build
.PHONY : CMakeFiles/bs4.dir/ult.c.o.provides

CMakeFiles/bs4.dir/ult.c.o.provides.build: CMakeFiles/bs4.dir/ult.c.o


CMakeFiles/bs4.dir/stats.c.o: CMakeFiles/bs4.dir/flags.make
CMakeFiles/bs4.dir/stats.c.o: ../stats.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/home/se2/git/bs-ha/user level threads/impl_Piotrek/cmake-build-debug/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_4) "Building C object CMakeFiles/bs4.dir/stats.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/bs4.dir/stats.c.o   -c "/home/se2/git/bs-ha/user level threads/impl_Piotrek/stats.c"

CMakeFiles/bs4.dir/stats.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/bs4.dir/stats.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E "/home/se2/git/bs-ha/user level threads/impl_Piotrek/stats.c" > CMakeFiles/bs4.dir/stats.c.i

CMakeFiles/bs4.dir/stats.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/bs4.dir/stats.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S "/home/se2/git/bs-ha/user level threads/impl_Piotrek/stats.c" -o CMakeFiles/bs4.dir/stats.c.s

CMakeFiles/bs4.dir/stats.c.o.requires:

.PHONY : CMakeFiles/bs4.dir/stats.c.o.requires

CMakeFiles/bs4.dir/stats.c.o.provides: CMakeFiles/bs4.dir/stats.c.o.requires
	$(MAKE) -f CMakeFiles/bs4.dir/build.make CMakeFiles/bs4.dir/stats.c.o.provides.build
.PHONY : CMakeFiles/bs4.dir/stats.c.o.provides

CMakeFiles/bs4.dir/stats.c.o.provides.build: CMakeFiles/bs4.dir/stats.c.o


# Object files for target bs4
bs4_OBJECTS = \
"CMakeFiles/bs4.dir/main.c.o" \
"CMakeFiles/bs4.dir/array.c.o" \
"CMakeFiles/bs4.dir/ult.c.o" \
"CMakeFiles/bs4.dir/stats.c.o"

# External object files for target bs4
bs4_EXTERNAL_OBJECTS =

bs4: CMakeFiles/bs4.dir/main.c.o
bs4: CMakeFiles/bs4.dir/array.c.o
bs4: CMakeFiles/bs4.dir/ult.c.o
bs4: CMakeFiles/bs4.dir/stats.c.o
bs4: CMakeFiles/bs4.dir/build.make
bs4: CMakeFiles/bs4.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir="/home/se2/git/bs-ha/user level threads/impl_Piotrek/cmake-build-debug/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_5) "Linking C executable bs4"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/bs4.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/bs4.dir/build: bs4

.PHONY : CMakeFiles/bs4.dir/build

CMakeFiles/bs4.dir/requires: CMakeFiles/bs4.dir/main.c.o.requires
CMakeFiles/bs4.dir/requires: CMakeFiles/bs4.dir/array.c.o.requires
CMakeFiles/bs4.dir/requires: CMakeFiles/bs4.dir/ult.c.o.requires
CMakeFiles/bs4.dir/requires: CMakeFiles/bs4.dir/stats.c.o.requires

.PHONY : CMakeFiles/bs4.dir/requires

CMakeFiles/bs4.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/bs4.dir/cmake_clean.cmake
.PHONY : CMakeFiles/bs4.dir/clean

CMakeFiles/bs4.dir/depend:
	cd "/home/se2/git/bs-ha/user level threads/impl_Piotrek/cmake-build-debug" && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" "/home/se2/git/bs-ha/user level threads/impl_Piotrek" "/home/se2/git/bs-ha/user level threads/impl_Piotrek" "/home/se2/git/bs-ha/user level threads/impl_Piotrek/cmake-build-debug" "/home/se2/git/bs-ha/user level threads/impl_Piotrek/cmake-build-debug" "/home/se2/git/bs-ha/user level threads/impl_Piotrek/cmake-build-debug/CMakeFiles/bs4.dir/DependInfo.cmake" --color=$(COLOR)
.PHONY : CMakeFiles/bs4.dir/depend

