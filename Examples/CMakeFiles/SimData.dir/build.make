# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.4

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
CMAKE_COMMAND = /Applications/CMake.app/Contents/bin/cmake

# The command to remove a file.
RM = /Applications/CMake.app/Contents/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/ImenAlSamarai/Documents/mbr

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/ImenAlSamarai/Documents/mbr

# Include any dependencies generated for this target.
include Examples/CMakeFiles/SimData.dir/depend.make

# Include the progress variables for this target.
include Examples/CMakeFiles/SimData.dir/progress.make

# Include the compile flags for this target's objects.
include Examples/CMakeFiles/SimData.dir/flags.make

Examples/CMakeFiles/SimData.dir/SimData.cc.o: Examples/CMakeFiles/SimData.dir/flags.make
Examples/CMakeFiles/SimData.dir/SimData.cc.o: Examples/SimData.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/ImenAlSamarai/Documents/mbr/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object Examples/CMakeFiles/SimData.dir/SimData.cc.o"
	cd /Users/ImenAlSamarai/Documents/mbr/Examples && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/SimData.dir/SimData.cc.o -c /Users/ImenAlSamarai/Documents/mbr/Examples/SimData.cc

Examples/CMakeFiles/SimData.dir/SimData.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/SimData.dir/SimData.cc.i"
	cd /Users/ImenAlSamarai/Documents/mbr/Examples && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/ImenAlSamarai/Documents/mbr/Examples/SimData.cc > CMakeFiles/SimData.dir/SimData.cc.i

Examples/CMakeFiles/SimData.dir/SimData.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/SimData.dir/SimData.cc.s"
	cd /Users/ImenAlSamarai/Documents/mbr/Examples && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/ImenAlSamarai/Documents/mbr/Examples/SimData.cc -o CMakeFiles/SimData.dir/SimData.cc.s

Examples/CMakeFiles/SimData.dir/SimData.cc.o.requires:

.PHONY : Examples/CMakeFiles/SimData.dir/SimData.cc.o.requires

Examples/CMakeFiles/SimData.dir/SimData.cc.o.provides: Examples/CMakeFiles/SimData.dir/SimData.cc.o.requires
	$(MAKE) -f Examples/CMakeFiles/SimData.dir/build.make Examples/CMakeFiles/SimData.dir/SimData.cc.o.provides.build
.PHONY : Examples/CMakeFiles/SimData.dir/SimData.cc.o.provides

Examples/CMakeFiles/SimData.dir/SimData.cc.o.provides.build: Examples/CMakeFiles/SimData.dir/SimData.cc.o


# Object files for target SimData
SimData_OBJECTS = \
"CMakeFiles/SimData.dir/SimData.cc.o"

# External object files for target SimData
SimData_EXTERNAL_OBJECTS =

Examples/SimData: Examples/CMakeFiles/SimData.dir/SimData.cc.o
Examples/SimData: Examples/CMakeFiles/SimData.dir/build.make
Examples/SimData: Simulation/libSim.dylib
Examples/SimData: /Applications/root/lib/libCore.so
Examples/SimData: /Applications/root/lib/libCint.so
Examples/SimData: /Applications/root/lib/libRIO.so
Examples/SimData: /Applications/root/lib/libNet.so
Examples/SimData: /Applications/root/lib/libHist.so
Examples/SimData: /Applications/root/lib/libGraf.so
Examples/SimData: /Applications/root/lib/libGraf3d.so
Examples/SimData: /Applications/root/lib/libGpad.so
Examples/SimData: /Applications/root/lib/libTree.so
Examples/SimData: /Applications/root/lib/libRint.so
Examples/SimData: /Applications/root/lib/libPostscript.so
Examples/SimData: /Applications/root/lib/libMatrix.so
Examples/SimData: /Applications/root/lib/libPhysics.so
Examples/SimData: /Applications/root/lib/libMathCore.so
Examples/SimData: /Applications/root/lib/libThread.so
Examples/SimData: /usr/lib/libdl.dylib
Examples/SimData: /Applications/root/lib/libMinuit.so
Examples/SimData: /Applications/root/lib/libMathMore.so
Examples/SimData: Detector/libDetector.dylib
Examples/SimData: Event/libEvent.dylib
Examples/SimData: Utilities/libUtil.dylib
Examples/SimData: Externals/tinyxml/libtinyxml.dylib
Examples/SimData: /Applications/root/lib/libGui.so
Examples/SimData: /Applications/root/lib/libCore.so
Examples/SimData: /Applications/root/lib/libCint.so
Examples/SimData: /Applications/root/lib/libRIO.so
Examples/SimData: /Applications/root/lib/libNet.so
Examples/SimData: /Applications/root/lib/libHist.so
Examples/SimData: /Applications/root/lib/libGraf.so
Examples/SimData: /Applications/root/lib/libGraf3d.so
Examples/SimData: /Applications/root/lib/libGpad.so
Examples/SimData: /Applications/root/lib/libTree.so
Examples/SimData: /Applications/root/lib/libRint.so
Examples/SimData: /Applications/root/lib/libPostscript.so
Examples/SimData: /Applications/root/lib/libMatrix.so
Examples/SimData: /Applications/root/lib/libPhysics.so
Examples/SimData: /Applications/root/lib/libMathCore.so
Examples/SimData: /Applications/root/lib/libThread.so
Examples/SimData: /usr/lib/libdl.dylib
Examples/SimData: /Applications/root/lib/libMinuit.so
Examples/SimData: /Applications/root/lib/libMathMore.so
Examples/SimData: /Applications/root/lib/libXMLIO.so
Examples/SimData: /usr/local/lib/libfftw3.a
Examples/SimData: Examples/CMakeFiles/SimData.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/ImenAlSamarai/Documents/mbr/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable SimData"
	cd /Users/ImenAlSamarai/Documents/mbr/Examples && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/SimData.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
Examples/CMakeFiles/SimData.dir/build: Examples/SimData

.PHONY : Examples/CMakeFiles/SimData.dir/build

Examples/CMakeFiles/SimData.dir/requires: Examples/CMakeFiles/SimData.dir/SimData.cc.o.requires

.PHONY : Examples/CMakeFiles/SimData.dir/requires

Examples/CMakeFiles/SimData.dir/clean:
	cd /Users/ImenAlSamarai/Documents/mbr/Examples && $(CMAKE_COMMAND) -P CMakeFiles/SimData.dir/cmake_clean.cmake
.PHONY : Examples/CMakeFiles/SimData.dir/clean

Examples/CMakeFiles/SimData.dir/depend:
	cd /Users/ImenAlSamarai/Documents/mbr && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/ImenAlSamarai/Documents/mbr /Users/ImenAlSamarai/Documents/mbr/Examples /Users/ImenAlSamarai/Documents/mbr /Users/ImenAlSamarai/Documents/mbr/Examples /Users/ImenAlSamarai/Documents/mbr/Examples/CMakeFiles/SimData.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : Examples/CMakeFiles/SimData.dir/depend

