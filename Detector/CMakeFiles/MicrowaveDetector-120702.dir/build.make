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

# Utility rule file for MicrowaveDetector-120702.

# Include the progress variables for this target.
include Detector/CMakeFiles/MicrowaveDetector-120702.dir/progress.make

Detector/MicrowaveDetector-120702.xml: Detector/xml/MicrowaveDetector-120702.xml.in
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/Users/ImenAlSamarai/Documents/mbr/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Generating MicrowaveDetector-120702.xml"
	cd /Users/ImenAlSamarai/Documents/mbr/Detector && sed -e 's+@XMLCONFIGDIR@+'/Users/ImenAlSamarai/Documents/mbr'/config+' /Users/ImenAlSamarai/Documents/mbr/Detector/xml/MicrowaveDetector-120702.xml.in > /Users/ImenAlSamarai/Documents/mbr/Detector/MicrowaveDetector-120702.xml

MicrowaveDetector-120702: Detector/MicrowaveDetector-120702.xml
MicrowaveDetector-120702: Detector/CMakeFiles/MicrowaveDetector-120702.dir/build.make

.PHONY : MicrowaveDetector-120702

# Rule to build all files generated by this target.
Detector/CMakeFiles/MicrowaveDetector-120702.dir/build: MicrowaveDetector-120702

.PHONY : Detector/CMakeFiles/MicrowaveDetector-120702.dir/build

Detector/CMakeFiles/MicrowaveDetector-120702.dir/clean:
	cd /Users/ImenAlSamarai/Documents/mbr/Detector && $(CMAKE_COMMAND) -P CMakeFiles/MicrowaveDetector-120702.dir/cmake_clean.cmake
.PHONY : Detector/CMakeFiles/MicrowaveDetector-120702.dir/clean

Detector/CMakeFiles/MicrowaveDetector-120702.dir/depend:
	cd /Users/ImenAlSamarai/Documents/mbr && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/ImenAlSamarai/Documents/mbr /Users/ImenAlSamarai/Documents/mbr/Detector /Users/ImenAlSamarai/Documents/mbr /Users/ImenAlSamarai/Documents/mbr/Detector /Users/ImenAlSamarai/Documents/mbr/Detector/CMakeFiles/MicrowaveDetector-120702.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : Detector/CMakeFiles/MicrowaveDetector-120702.dir/depend

