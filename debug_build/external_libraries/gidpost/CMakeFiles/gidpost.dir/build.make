# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.2

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
CMAKE_SOURCE_DIR = /home/julio/Kratos

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/julio/Kratos/debug_build

# Include any dependencies generated for this target.
include external_libraries/gidpost/CMakeFiles/gidpost.dir/depend.make

# Include the progress variables for this target.
include external_libraries/gidpost/CMakeFiles/gidpost.dir/progress.make

# Include the compile flags for this target's objects.
include external_libraries/gidpost/CMakeFiles/gidpost.dir/flags.make

external_libraries/gidpost/CMakeFiles/gidpost.dir/source/gidpost.c.o: external_libraries/gidpost/CMakeFiles/gidpost.dir/flags.make
external_libraries/gidpost/CMakeFiles/gidpost.dir/source/gidpost.c.o: ../external_libraries/gidpost/source/gidpost.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/julio/Kratos/debug_build/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object external_libraries/gidpost/CMakeFiles/gidpost.dir/source/gidpost.c.o"
	cd /home/julio/Kratos/debug_build/external_libraries/gidpost && /usr/bin/gcc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/gidpost.dir/source/gidpost.c.o   -c /home/julio/Kratos/external_libraries/gidpost/source/gidpost.c

external_libraries/gidpost/CMakeFiles/gidpost.dir/source/gidpost.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/gidpost.dir/source/gidpost.c.i"
	cd /home/julio/Kratos/debug_build/external_libraries/gidpost && /usr/bin/gcc  $(C_DEFINES) $(C_FLAGS) -E /home/julio/Kratos/external_libraries/gidpost/source/gidpost.c > CMakeFiles/gidpost.dir/source/gidpost.c.i

external_libraries/gidpost/CMakeFiles/gidpost.dir/source/gidpost.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/gidpost.dir/source/gidpost.c.s"
	cd /home/julio/Kratos/debug_build/external_libraries/gidpost && /usr/bin/gcc  $(C_DEFINES) $(C_FLAGS) -S /home/julio/Kratos/external_libraries/gidpost/source/gidpost.c -o CMakeFiles/gidpost.dir/source/gidpost.c.s

external_libraries/gidpost/CMakeFiles/gidpost.dir/source/gidpost.c.o.requires:
.PHONY : external_libraries/gidpost/CMakeFiles/gidpost.dir/source/gidpost.c.o.requires

external_libraries/gidpost/CMakeFiles/gidpost.dir/source/gidpost.c.o.provides: external_libraries/gidpost/CMakeFiles/gidpost.dir/source/gidpost.c.o.requires
	$(MAKE) -f external_libraries/gidpost/CMakeFiles/gidpost.dir/build.make external_libraries/gidpost/CMakeFiles/gidpost.dir/source/gidpost.c.o.provides.build
.PHONY : external_libraries/gidpost/CMakeFiles/gidpost.dir/source/gidpost.c.o.provides

external_libraries/gidpost/CMakeFiles/gidpost.dir/source/gidpost.c.o.provides.build: external_libraries/gidpost/CMakeFiles/gidpost.dir/source/gidpost.c.o

external_libraries/gidpost/CMakeFiles/gidpost.dir/source/gidpostInt.c.o: external_libraries/gidpost/CMakeFiles/gidpost.dir/flags.make
external_libraries/gidpost/CMakeFiles/gidpost.dir/source/gidpostInt.c.o: ../external_libraries/gidpost/source/gidpostInt.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/julio/Kratos/debug_build/CMakeFiles $(CMAKE_PROGRESS_2)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object external_libraries/gidpost/CMakeFiles/gidpost.dir/source/gidpostInt.c.o"
	cd /home/julio/Kratos/debug_build/external_libraries/gidpost && /usr/bin/gcc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/gidpost.dir/source/gidpostInt.c.o   -c /home/julio/Kratos/external_libraries/gidpost/source/gidpostInt.c

external_libraries/gidpost/CMakeFiles/gidpost.dir/source/gidpostInt.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/gidpost.dir/source/gidpostInt.c.i"
	cd /home/julio/Kratos/debug_build/external_libraries/gidpost && /usr/bin/gcc  $(C_DEFINES) $(C_FLAGS) -E /home/julio/Kratos/external_libraries/gidpost/source/gidpostInt.c > CMakeFiles/gidpost.dir/source/gidpostInt.c.i

external_libraries/gidpost/CMakeFiles/gidpost.dir/source/gidpostInt.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/gidpost.dir/source/gidpostInt.c.s"
	cd /home/julio/Kratos/debug_build/external_libraries/gidpost && /usr/bin/gcc  $(C_DEFINES) $(C_FLAGS) -S /home/julio/Kratos/external_libraries/gidpost/source/gidpostInt.c -o CMakeFiles/gidpost.dir/source/gidpostInt.c.s

external_libraries/gidpost/CMakeFiles/gidpost.dir/source/gidpostInt.c.o.requires:
.PHONY : external_libraries/gidpost/CMakeFiles/gidpost.dir/source/gidpostInt.c.o.requires

external_libraries/gidpost/CMakeFiles/gidpost.dir/source/gidpostInt.c.o.provides: external_libraries/gidpost/CMakeFiles/gidpost.dir/source/gidpostInt.c.o.requires
	$(MAKE) -f external_libraries/gidpost/CMakeFiles/gidpost.dir/build.make external_libraries/gidpost/CMakeFiles/gidpost.dir/source/gidpostInt.c.o.provides.build
.PHONY : external_libraries/gidpost/CMakeFiles/gidpost.dir/source/gidpostInt.c.o.provides

external_libraries/gidpost/CMakeFiles/gidpost.dir/source/gidpostInt.c.o.provides.build: external_libraries/gidpost/CMakeFiles/gidpost.dir/source/gidpostInt.c.o

external_libraries/gidpost/CMakeFiles/gidpost.dir/source/gidpostHash.c.o: external_libraries/gidpost/CMakeFiles/gidpost.dir/flags.make
external_libraries/gidpost/CMakeFiles/gidpost.dir/source/gidpostHash.c.o: ../external_libraries/gidpost/source/gidpostHash.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/julio/Kratos/debug_build/CMakeFiles $(CMAKE_PROGRESS_3)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object external_libraries/gidpost/CMakeFiles/gidpost.dir/source/gidpostHash.c.o"
	cd /home/julio/Kratos/debug_build/external_libraries/gidpost && /usr/bin/gcc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/gidpost.dir/source/gidpostHash.c.o   -c /home/julio/Kratos/external_libraries/gidpost/source/gidpostHash.c

external_libraries/gidpost/CMakeFiles/gidpost.dir/source/gidpostHash.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/gidpost.dir/source/gidpostHash.c.i"
	cd /home/julio/Kratos/debug_build/external_libraries/gidpost && /usr/bin/gcc  $(C_DEFINES) $(C_FLAGS) -E /home/julio/Kratos/external_libraries/gidpost/source/gidpostHash.c > CMakeFiles/gidpost.dir/source/gidpostHash.c.i

external_libraries/gidpost/CMakeFiles/gidpost.dir/source/gidpostHash.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/gidpost.dir/source/gidpostHash.c.s"
	cd /home/julio/Kratos/debug_build/external_libraries/gidpost && /usr/bin/gcc  $(C_DEFINES) $(C_FLAGS) -S /home/julio/Kratos/external_libraries/gidpost/source/gidpostHash.c -o CMakeFiles/gidpost.dir/source/gidpostHash.c.s

external_libraries/gidpost/CMakeFiles/gidpost.dir/source/gidpostHash.c.o.requires:
.PHONY : external_libraries/gidpost/CMakeFiles/gidpost.dir/source/gidpostHash.c.o.requires

external_libraries/gidpost/CMakeFiles/gidpost.dir/source/gidpostHash.c.o.provides: external_libraries/gidpost/CMakeFiles/gidpost.dir/source/gidpostHash.c.o.requires
	$(MAKE) -f external_libraries/gidpost/CMakeFiles/gidpost.dir/build.make external_libraries/gidpost/CMakeFiles/gidpost.dir/source/gidpostHash.c.o.provides.build
.PHONY : external_libraries/gidpost/CMakeFiles/gidpost.dir/source/gidpostHash.c.o.provides

external_libraries/gidpost/CMakeFiles/gidpost.dir/source/gidpostHash.c.o.provides.build: external_libraries/gidpost/CMakeFiles/gidpost.dir/source/gidpostHash.c.o

external_libraries/gidpost/CMakeFiles/gidpost.dir/source/hashtab.c.o: external_libraries/gidpost/CMakeFiles/gidpost.dir/flags.make
external_libraries/gidpost/CMakeFiles/gidpost.dir/source/hashtab.c.o: ../external_libraries/gidpost/source/hashtab.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/julio/Kratos/debug_build/CMakeFiles $(CMAKE_PROGRESS_4)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object external_libraries/gidpost/CMakeFiles/gidpost.dir/source/hashtab.c.o"
	cd /home/julio/Kratos/debug_build/external_libraries/gidpost && /usr/bin/gcc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/gidpost.dir/source/hashtab.c.o   -c /home/julio/Kratos/external_libraries/gidpost/source/hashtab.c

external_libraries/gidpost/CMakeFiles/gidpost.dir/source/hashtab.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/gidpost.dir/source/hashtab.c.i"
	cd /home/julio/Kratos/debug_build/external_libraries/gidpost && /usr/bin/gcc  $(C_DEFINES) $(C_FLAGS) -E /home/julio/Kratos/external_libraries/gidpost/source/hashtab.c > CMakeFiles/gidpost.dir/source/hashtab.c.i

external_libraries/gidpost/CMakeFiles/gidpost.dir/source/hashtab.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/gidpost.dir/source/hashtab.c.s"
	cd /home/julio/Kratos/debug_build/external_libraries/gidpost && /usr/bin/gcc  $(C_DEFINES) $(C_FLAGS) -S /home/julio/Kratos/external_libraries/gidpost/source/hashtab.c -o CMakeFiles/gidpost.dir/source/hashtab.c.s

external_libraries/gidpost/CMakeFiles/gidpost.dir/source/hashtab.c.o.requires:
.PHONY : external_libraries/gidpost/CMakeFiles/gidpost.dir/source/hashtab.c.o.requires

external_libraries/gidpost/CMakeFiles/gidpost.dir/source/hashtab.c.o.provides: external_libraries/gidpost/CMakeFiles/gidpost.dir/source/hashtab.c.o.requires
	$(MAKE) -f external_libraries/gidpost/CMakeFiles/gidpost.dir/build.make external_libraries/gidpost/CMakeFiles/gidpost.dir/source/hashtab.c.o.provides.build
.PHONY : external_libraries/gidpost/CMakeFiles/gidpost.dir/source/hashtab.c.o.provides

external_libraries/gidpost/CMakeFiles/gidpost.dir/source/hashtab.c.o.provides.build: external_libraries/gidpost/CMakeFiles/gidpost.dir/source/hashtab.c.o

external_libraries/gidpost/CMakeFiles/gidpost.dir/source/recycle.c.o: external_libraries/gidpost/CMakeFiles/gidpost.dir/flags.make
external_libraries/gidpost/CMakeFiles/gidpost.dir/source/recycle.c.o: ../external_libraries/gidpost/source/recycle.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/julio/Kratos/debug_build/CMakeFiles $(CMAKE_PROGRESS_5)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object external_libraries/gidpost/CMakeFiles/gidpost.dir/source/recycle.c.o"
	cd /home/julio/Kratos/debug_build/external_libraries/gidpost && /usr/bin/gcc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/gidpost.dir/source/recycle.c.o   -c /home/julio/Kratos/external_libraries/gidpost/source/recycle.c

external_libraries/gidpost/CMakeFiles/gidpost.dir/source/recycle.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/gidpost.dir/source/recycle.c.i"
	cd /home/julio/Kratos/debug_build/external_libraries/gidpost && /usr/bin/gcc  $(C_DEFINES) $(C_FLAGS) -E /home/julio/Kratos/external_libraries/gidpost/source/recycle.c > CMakeFiles/gidpost.dir/source/recycle.c.i

external_libraries/gidpost/CMakeFiles/gidpost.dir/source/recycle.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/gidpost.dir/source/recycle.c.s"
	cd /home/julio/Kratos/debug_build/external_libraries/gidpost && /usr/bin/gcc  $(C_DEFINES) $(C_FLAGS) -S /home/julio/Kratos/external_libraries/gidpost/source/recycle.c -o CMakeFiles/gidpost.dir/source/recycle.c.s

external_libraries/gidpost/CMakeFiles/gidpost.dir/source/recycle.c.o.requires:
.PHONY : external_libraries/gidpost/CMakeFiles/gidpost.dir/source/recycle.c.o.requires

external_libraries/gidpost/CMakeFiles/gidpost.dir/source/recycle.c.o.provides: external_libraries/gidpost/CMakeFiles/gidpost.dir/source/recycle.c.o.requires
	$(MAKE) -f external_libraries/gidpost/CMakeFiles/gidpost.dir/build.make external_libraries/gidpost/CMakeFiles/gidpost.dir/source/recycle.c.o.provides.build
.PHONY : external_libraries/gidpost/CMakeFiles/gidpost.dir/source/recycle.c.o.provides

external_libraries/gidpost/CMakeFiles/gidpost.dir/source/recycle.c.o.provides.build: external_libraries/gidpost/CMakeFiles/gidpost.dir/source/recycle.c.o

external_libraries/gidpost/CMakeFiles/gidpost.dir/source/lookupa.c.o: external_libraries/gidpost/CMakeFiles/gidpost.dir/flags.make
external_libraries/gidpost/CMakeFiles/gidpost.dir/source/lookupa.c.o: ../external_libraries/gidpost/source/lookupa.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/julio/Kratos/debug_build/CMakeFiles $(CMAKE_PROGRESS_6)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object external_libraries/gidpost/CMakeFiles/gidpost.dir/source/lookupa.c.o"
	cd /home/julio/Kratos/debug_build/external_libraries/gidpost && /usr/bin/gcc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/gidpost.dir/source/lookupa.c.o   -c /home/julio/Kratos/external_libraries/gidpost/source/lookupa.c

external_libraries/gidpost/CMakeFiles/gidpost.dir/source/lookupa.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/gidpost.dir/source/lookupa.c.i"
	cd /home/julio/Kratos/debug_build/external_libraries/gidpost && /usr/bin/gcc  $(C_DEFINES) $(C_FLAGS) -E /home/julio/Kratos/external_libraries/gidpost/source/lookupa.c > CMakeFiles/gidpost.dir/source/lookupa.c.i

external_libraries/gidpost/CMakeFiles/gidpost.dir/source/lookupa.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/gidpost.dir/source/lookupa.c.s"
	cd /home/julio/Kratos/debug_build/external_libraries/gidpost && /usr/bin/gcc  $(C_DEFINES) $(C_FLAGS) -S /home/julio/Kratos/external_libraries/gidpost/source/lookupa.c -o CMakeFiles/gidpost.dir/source/lookupa.c.s

external_libraries/gidpost/CMakeFiles/gidpost.dir/source/lookupa.c.o.requires:
.PHONY : external_libraries/gidpost/CMakeFiles/gidpost.dir/source/lookupa.c.o.requires

external_libraries/gidpost/CMakeFiles/gidpost.dir/source/lookupa.c.o.provides: external_libraries/gidpost/CMakeFiles/gidpost.dir/source/lookupa.c.o.requires
	$(MAKE) -f external_libraries/gidpost/CMakeFiles/gidpost.dir/build.make external_libraries/gidpost/CMakeFiles/gidpost.dir/source/lookupa.c.o.provides.build
.PHONY : external_libraries/gidpost/CMakeFiles/gidpost.dir/source/lookupa.c.o.provides

external_libraries/gidpost/CMakeFiles/gidpost.dir/source/lookupa.c.o.provides.build: external_libraries/gidpost/CMakeFiles/gidpost.dir/source/lookupa.c.o

# Object files for target gidpost
gidpost_OBJECTS = \
"CMakeFiles/gidpost.dir/source/gidpost.c.o" \
"CMakeFiles/gidpost.dir/source/gidpostInt.c.o" \
"CMakeFiles/gidpost.dir/source/gidpostHash.c.o" \
"CMakeFiles/gidpost.dir/source/hashtab.c.o" \
"CMakeFiles/gidpost.dir/source/recycle.c.o" \
"CMakeFiles/gidpost.dir/source/lookupa.c.o"

# External object files for target gidpost
gidpost_EXTERNAL_OBJECTS =

external_libraries/gidpost/libgidpost.a: external_libraries/gidpost/CMakeFiles/gidpost.dir/source/gidpost.c.o
external_libraries/gidpost/libgidpost.a: external_libraries/gidpost/CMakeFiles/gidpost.dir/source/gidpostInt.c.o
external_libraries/gidpost/libgidpost.a: external_libraries/gidpost/CMakeFiles/gidpost.dir/source/gidpostHash.c.o
external_libraries/gidpost/libgidpost.a: external_libraries/gidpost/CMakeFiles/gidpost.dir/source/hashtab.c.o
external_libraries/gidpost/libgidpost.a: external_libraries/gidpost/CMakeFiles/gidpost.dir/source/recycle.c.o
external_libraries/gidpost/libgidpost.a: external_libraries/gidpost/CMakeFiles/gidpost.dir/source/lookupa.c.o
external_libraries/gidpost/libgidpost.a: external_libraries/gidpost/CMakeFiles/gidpost.dir/build.make
external_libraries/gidpost/libgidpost.a: external_libraries/gidpost/CMakeFiles/gidpost.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking C static library libgidpost.a"
	cd /home/julio/Kratos/debug_build/external_libraries/gidpost && $(CMAKE_COMMAND) -P CMakeFiles/gidpost.dir/cmake_clean_target.cmake
	cd /home/julio/Kratos/debug_build/external_libraries/gidpost && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/gidpost.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
external_libraries/gidpost/CMakeFiles/gidpost.dir/build: external_libraries/gidpost/libgidpost.a
.PHONY : external_libraries/gidpost/CMakeFiles/gidpost.dir/build

external_libraries/gidpost/CMakeFiles/gidpost.dir/requires: external_libraries/gidpost/CMakeFiles/gidpost.dir/source/gidpost.c.o.requires
external_libraries/gidpost/CMakeFiles/gidpost.dir/requires: external_libraries/gidpost/CMakeFiles/gidpost.dir/source/gidpostInt.c.o.requires
external_libraries/gidpost/CMakeFiles/gidpost.dir/requires: external_libraries/gidpost/CMakeFiles/gidpost.dir/source/gidpostHash.c.o.requires
external_libraries/gidpost/CMakeFiles/gidpost.dir/requires: external_libraries/gidpost/CMakeFiles/gidpost.dir/source/hashtab.c.o.requires
external_libraries/gidpost/CMakeFiles/gidpost.dir/requires: external_libraries/gidpost/CMakeFiles/gidpost.dir/source/recycle.c.o.requires
external_libraries/gidpost/CMakeFiles/gidpost.dir/requires: external_libraries/gidpost/CMakeFiles/gidpost.dir/source/lookupa.c.o.requires
.PHONY : external_libraries/gidpost/CMakeFiles/gidpost.dir/requires

external_libraries/gidpost/CMakeFiles/gidpost.dir/clean:
	cd /home/julio/Kratos/debug_build/external_libraries/gidpost && $(CMAKE_COMMAND) -P CMakeFiles/gidpost.dir/cmake_clean.cmake
.PHONY : external_libraries/gidpost/CMakeFiles/gidpost.dir/clean

external_libraries/gidpost/CMakeFiles/gidpost.dir/depend:
	cd /home/julio/Kratos/debug_build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/julio/Kratos /home/julio/Kratos/external_libraries/gidpost /home/julio/Kratos/debug_build /home/julio/Kratos/debug_build/external_libraries/gidpost /home/julio/Kratos/debug_build/external_libraries/gidpost/CMakeFiles/gidpost.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : external_libraries/gidpost/CMakeFiles/gidpost.dir/depend

