# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.5

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
CMAKE_SOURCE_DIR = /home/eric/Desktop/pclSender/libfreenect2

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/eric/Desktop/pclSender/libfreenect2

# Include any dependencies generated for this target.
include CMakeFiles/Test.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/Test.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/Test.dir/flags.make

CMakeFiles/Test.dir/Test.cpp.o: CMakeFiles/Test.dir/flags.make
CMakeFiles/Test.dir/Test.cpp.o: Test.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/eric/Desktop/pclSender/libfreenect2/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/Test.dir/Test.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Test.dir/Test.cpp.o -c /home/eric/Desktop/pclSender/libfreenect2/Test.cpp

CMakeFiles/Test.dir/Test.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Test.dir/Test.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/eric/Desktop/pclSender/libfreenect2/Test.cpp > CMakeFiles/Test.dir/Test.cpp.i

CMakeFiles/Test.dir/Test.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Test.dir/Test.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/eric/Desktop/pclSender/libfreenect2/Test.cpp -o CMakeFiles/Test.dir/Test.cpp.s

CMakeFiles/Test.dir/Test.cpp.o.requires:

.PHONY : CMakeFiles/Test.dir/Test.cpp.o.requires

CMakeFiles/Test.dir/Test.cpp.o.provides: CMakeFiles/Test.dir/Test.cpp.o.requires
	$(MAKE) -f CMakeFiles/Test.dir/build.make CMakeFiles/Test.dir/Test.cpp.o.provides.build
.PHONY : CMakeFiles/Test.dir/Test.cpp.o.provides

CMakeFiles/Test.dir/Test.cpp.o.provides.build: CMakeFiles/Test.dir/Test.cpp.o


CMakeFiles/Test.dir/viewer.cpp.o: CMakeFiles/Test.dir/flags.make
CMakeFiles/Test.dir/viewer.cpp.o: viewer.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/eric/Desktop/pclSender/libfreenect2/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/Test.dir/viewer.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Test.dir/viewer.cpp.o -c /home/eric/Desktop/pclSender/libfreenect2/viewer.cpp

CMakeFiles/Test.dir/viewer.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Test.dir/viewer.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/eric/Desktop/pclSender/libfreenect2/viewer.cpp > CMakeFiles/Test.dir/viewer.cpp.i

CMakeFiles/Test.dir/viewer.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Test.dir/viewer.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/eric/Desktop/pclSender/libfreenect2/viewer.cpp -o CMakeFiles/Test.dir/viewer.cpp.s

CMakeFiles/Test.dir/viewer.cpp.o.requires:

.PHONY : CMakeFiles/Test.dir/viewer.cpp.o.requires

CMakeFiles/Test.dir/viewer.cpp.o.provides: CMakeFiles/Test.dir/viewer.cpp.o.requires
	$(MAKE) -f CMakeFiles/Test.dir/build.make CMakeFiles/Test.dir/viewer.cpp.o.provides.build
.PHONY : CMakeFiles/Test.dir/viewer.cpp.o.provides

CMakeFiles/Test.dir/viewer.cpp.o.provides.build: CMakeFiles/Test.dir/viewer.cpp.o


CMakeFiles/Test.dir/home/eric/libfreenect2/src/flextGL.cpp.o: CMakeFiles/Test.dir/flags.make
CMakeFiles/Test.dir/home/eric/libfreenect2/src/flextGL.cpp.o: /home/eric/libfreenect2/src/flextGL.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/eric/Desktop/pclSender/libfreenect2/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/Test.dir/home/eric/libfreenect2/src/flextGL.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Test.dir/home/eric/libfreenect2/src/flextGL.cpp.o -c /home/eric/libfreenect2/src/flextGL.cpp

CMakeFiles/Test.dir/home/eric/libfreenect2/src/flextGL.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Test.dir/home/eric/libfreenect2/src/flextGL.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/eric/libfreenect2/src/flextGL.cpp > CMakeFiles/Test.dir/home/eric/libfreenect2/src/flextGL.cpp.i

CMakeFiles/Test.dir/home/eric/libfreenect2/src/flextGL.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Test.dir/home/eric/libfreenect2/src/flextGL.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/eric/libfreenect2/src/flextGL.cpp -o CMakeFiles/Test.dir/home/eric/libfreenect2/src/flextGL.cpp.s

CMakeFiles/Test.dir/home/eric/libfreenect2/src/flextGL.cpp.o.requires:

.PHONY : CMakeFiles/Test.dir/home/eric/libfreenect2/src/flextGL.cpp.o.requires

CMakeFiles/Test.dir/home/eric/libfreenect2/src/flextGL.cpp.o.provides: CMakeFiles/Test.dir/home/eric/libfreenect2/src/flextGL.cpp.o.requires
	$(MAKE) -f CMakeFiles/Test.dir/build.make CMakeFiles/Test.dir/home/eric/libfreenect2/src/flextGL.cpp.o.provides.build
.PHONY : CMakeFiles/Test.dir/home/eric/libfreenect2/src/flextGL.cpp.o.provides

CMakeFiles/Test.dir/home/eric/libfreenect2/src/flextGL.cpp.o.provides.build: CMakeFiles/Test.dir/home/eric/libfreenect2/src/flextGL.cpp.o


# Object files for target Test
Test_OBJECTS = \
"CMakeFiles/Test.dir/Test.cpp.o" \
"CMakeFiles/Test.dir/viewer.cpp.o" \
"CMakeFiles/Test.dir/home/eric/libfreenect2/src/flextGL.cpp.o"

# External object files for target Test
Test_EXTERNAL_OBJECTS =

Test: CMakeFiles/Test.dir/Test.cpp.o
Test: CMakeFiles/Test.dir/viewer.cpp.o
Test: CMakeFiles/Test.dir/home/eric/libfreenect2/src/flextGL.cpp.o
Test: CMakeFiles/Test.dir/build.make
Test: /home/eric/freenect2/lib/libfreenect2.so
Test: /usr/lib/x86_64-linux-gnu/libglfw.so
Test: /usr/lib/x86_64-linux-gnu/libGL.so
Test: /usr/local/lib/libopencv_highgui.so.3.4.0
Test: /usr/local/lib/libopencv_videoio.so.3.4.0
Test: /usr/local/lib/libopencv_imgcodecs.so.3.4.0
Test: /usr/local/lib/libopencv_imgproc.so.3.4.0
Test: /usr/local/lib/libopencv_core.so.3.4.0
Test: /usr/local/lib/libopencv_cudev.so.3.4.0
Test: CMakeFiles/Test.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/eric/Desktop/pclSender/libfreenect2/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking CXX executable Test"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Test.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/Test.dir/build: Test

.PHONY : CMakeFiles/Test.dir/build

CMakeFiles/Test.dir/requires: CMakeFiles/Test.dir/Test.cpp.o.requires
CMakeFiles/Test.dir/requires: CMakeFiles/Test.dir/viewer.cpp.o.requires
CMakeFiles/Test.dir/requires: CMakeFiles/Test.dir/home/eric/libfreenect2/src/flextGL.cpp.o.requires

.PHONY : CMakeFiles/Test.dir/requires

CMakeFiles/Test.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/Test.dir/cmake_clean.cmake
.PHONY : CMakeFiles/Test.dir/clean

CMakeFiles/Test.dir/depend:
	cd /home/eric/Desktop/pclSender/libfreenect2 && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/eric/Desktop/pclSender/libfreenect2 /home/eric/Desktop/pclSender/libfreenect2 /home/eric/Desktop/pclSender/libfreenect2 /home/eric/Desktop/pclSender/libfreenect2 /home/eric/Desktop/pclSender/libfreenect2/CMakeFiles/Test.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/Test.dir/depend
