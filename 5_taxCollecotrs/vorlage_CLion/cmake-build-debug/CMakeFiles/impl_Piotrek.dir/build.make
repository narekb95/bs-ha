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
CMAKE_SOURCE_DIR = /home/se2/git/bs-ha/5_taxCollecotrs/impl_Piotrek

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/se2/git/bs-ha/5_taxCollecotrs/impl_Piotrek/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/impl_Piotrek.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/impl_Piotrek.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/impl_Piotrek.dir/flags.make

CMakeFiles/impl_Piotrek.dir/tcd.c.o: CMakeFiles/impl_Piotrek.dir/flags.make
CMakeFiles/impl_Piotrek.dir/tcd.c.o: ../tcd.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/se2/git/bs-ha/5_taxCollecotrs/impl_Piotrek/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/impl_Piotrek.dir/tcd.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/impl_Piotrek.dir/tcd.c.o   -c /home/se2/git/bs-ha/5_taxCollecotrs/impl_Piotrek/tcd.c

CMakeFiles/impl_Piotrek.dir/tcd.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/impl_Piotrek.dir/tcd.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/se2/git/bs-ha/5_taxCollecotrs/impl_Piotrek/tcd.c > CMakeFiles/impl_Piotrek.dir/tcd.c.i

CMakeFiles/impl_Piotrek.dir/tcd.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/impl_Piotrek.dir/tcd.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/se2/git/bs-ha/5_taxCollecotrs/impl_Piotrek/tcd.c -o CMakeFiles/impl_Piotrek.dir/tcd.c.s

CMakeFiles/impl_Piotrek.dir/tcd.c.o.requires:

.PHONY : CMakeFiles/impl_Piotrek.dir/tcd.c.o.requires

CMakeFiles/impl_Piotrek.dir/tcd.c.o.provides: CMakeFiles/impl_Piotrek.dir/tcd.c.o.requires
	$(MAKE) -f CMakeFiles/impl_Piotrek.dir/build.make CMakeFiles/impl_Piotrek.dir/tcd.c.o.provides.build
.PHONY : CMakeFiles/impl_Piotrek.dir/tcd.c.o.provides

CMakeFiles/impl_Piotrek.dir/tcd.c.o.provides.build: CMakeFiles/impl_Piotrek.dir/tcd.c.o


# Object files for target impl_Piotrek
impl_Piotrek_OBJECTS = \
"CMakeFiles/impl_Piotrek.dir/tcd.c.o"

# External object files for target impl_Piotrek
impl_Piotrek_EXTERNAL_OBJECTS =

impl_Piotrek: CMakeFiles/impl_Piotrek.dir/tcd.c.o
impl_Piotrek: CMakeFiles/impl_Piotrek.dir/build.make
impl_Piotrek: CMakeFiles/impl_Piotrek.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/se2/git/bs-ha/5_taxCollecotrs/impl_Piotrek/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable impl_Piotrek"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/impl_Piotrek.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/impl_Piotrek.dir/build: impl_Piotrek

.PHONY : CMakeFiles/impl_Piotrek.dir/build

CMakeFiles/impl_Piotrek.dir/requires: CMakeFiles/impl_Piotrek.dir/tcd.c.o.requires

.PHONY : CMakeFiles/impl_Piotrek.dir/requires

CMakeFiles/impl_Piotrek.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/impl_Piotrek.dir/cmake_clean.cmake
.PHONY : CMakeFiles/impl_Piotrek.dir/clean

CMakeFiles/impl_Piotrek.dir/depend:
	cd /home/se2/git/bs-ha/5_taxCollecotrs/impl_Piotrek/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/se2/git/bs-ha/5_taxCollecotrs/impl_Piotrek /home/se2/git/bs-ha/5_taxCollecotrs/impl_Piotrek /home/se2/git/bs-ha/5_taxCollecotrs/impl_Piotrek/cmake-build-debug /home/se2/git/bs-ha/5_taxCollecotrs/impl_Piotrek/cmake-build-debug /home/se2/git/bs-ha/5_taxCollecotrs/impl_Piotrek/cmake-build-debug/CMakeFiles/impl_Piotrek.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/impl_Piotrek.dir/depend

