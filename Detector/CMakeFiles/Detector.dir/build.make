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
include Detector/CMakeFiles/Detector.dir/depend.make

# Include the progress variables for this target.
include Detector/CMakeFiles/Detector.dir/progress.make

# Include the compile flags for this target's objects.
include Detector/CMakeFiles/Detector.dir/flags.make

Detector/DetectorSetupDictionary.cc: Detector/src/Atmosphere.h
Detector/DetectorSetupDictionary.cc: Detector/src/Array.h
Detector/DetectorSetupDictionary.cc: Detector/src/Station.h
Detector/DetectorSetupDictionary.cc: Detector/src/Channel.h
Detector/DetectorSetupDictionary.cc: Detector/src/ChannelGain.h
Detector/DetectorSetupDictionary.cc: Detector/src/ChannelMap.h
Detector/DetectorSetupDictionary.cc: Detector/src/Detector.h
Detector/DetectorSetupDictionary.cc: Detector/src/Antenna.h
Detector/DetectorSetupDictionary.cc: Detector/src/DetectorSetup.h
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/Users/ImenAlSamarai/Documents/mbr/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Generating DetectorSetupDictionary.cc, DetectorSetupDictionary.h"
	cd /Users/ImenAlSamarai/Documents/mbr/Detector && /Applications/root/bin/rootcint -f /Users/ImenAlSamarai/Documents/mbr/Detector/DetectorSetupDictionary.cc -c -I/Users/ImenAlSamarai/Documents/mbr/Detector/src -I/Users/ImenAlSamarai/Documents/mbr/Event/src -I/Users/ImenAlSamarai/Documents/mbr/Utilities/src -I/Users/ImenAlSamarai/Documents/mbr/Externals/tinyxml -I/Users/ImenAlSamarai/Documents/mbr/Externals/gtest/include -I/Users/ImenAlSamarai/Documents/mbr/Detector src/Atmosphere.h src/Array.h src/Station.h src/Channel.h src/ChannelGain.h src/ChannelMap.h src/Detector.h src/Antenna.h src/DetectorSetup.h src/DetectorSetupLinkDef.h

Detector/DetectorSetupDictionary.h: Detector/DetectorSetupDictionary.cc
	@$(CMAKE_COMMAND) -E touch_nocreate Detector/DetectorSetupDictionary.h

Detector/CMakeFiles/Detector.dir/src/Atmosphere.cc.o: Detector/CMakeFiles/Detector.dir/flags.make
Detector/CMakeFiles/Detector.dir/src/Atmosphere.cc.o: Detector/src/Atmosphere.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/ImenAlSamarai/Documents/mbr/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object Detector/CMakeFiles/Detector.dir/src/Atmosphere.cc.o"
	cd /Users/ImenAlSamarai/Documents/mbr/Detector && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Detector.dir/src/Atmosphere.cc.o -c /Users/ImenAlSamarai/Documents/mbr/Detector/src/Atmosphere.cc

Detector/CMakeFiles/Detector.dir/src/Atmosphere.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Detector.dir/src/Atmosphere.cc.i"
	cd /Users/ImenAlSamarai/Documents/mbr/Detector && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/ImenAlSamarai/Documents/mbr/Detector/src/Atmosphere.cc > CMakeFiles/Detector.dir/src/Atmosphere.cc.i

Detector/CMakeFiles/Detector.dir/src/Atmosphere.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Detector.dir/src/Atmosphere.cc.s"
	cd /Users/ImenAlSamarai/Documents/mbr/Detector && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/ImenAlSamarai/Documents/mbr/Detector/src/Atmosphere.cc -o CMakeFiles/Detector.dir/src/Atmosphere.cc.s

Detector/CMakeFiles/Detector.dir/src/Atmosphere.cc.o.requires:

.PHONY : Detector/CMakeFiles/Detector.dir/src/Atmosphere.cc.o.requires

Detector/CMakeFiles/Detector.dir/src/Atmosphere.cc.o.provides: Detector/CMakeFiles/Detector.dir/src/Atmosphere.cc.o.requires
	$(MAKE) -f Detector/CMakeFiles/Detector.dir/build.make Detector/CMakeFiles/Detector.dir/src/Atmosphere.cc.o.provides.build
.PHONY : Detector/CMakeFiles/Detector.dir/src/Atmosphere.cc.o.provides

Detector/CMakeFiles/Detector.dir/src/Atmosphere.cc.o.provides.build: Detector/CMakeFiles/Detector.dir/src/Atmosphere.cc.o


Detector/CMakeFiles/Detector.dir/src/Array.cc.o: Detector/CMakeFiles/Detector.dir/flags.make
Detector/CMakeFiles/Detector.dir/src/Array.cc.o: Detector/src/Array.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/ImenAlSamarai/Documents/mbr/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object Detector/CMakeFiles/Detector.dir/src/Array.cc.o"
	cd /Users/ImenAlSamarai/Documents/mbr/Detector && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Detector.dir/src/Array.cc.o -c /Users/ImenAlSamarai/Documents/mbr/Detector/src/Array.cc

Detector/CMakeFiles/Detector.dir/src/Array.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Detector.dir/src/Array.cc.i"
	cd /Users/ImenAlSamarai/Documents/mbr/Detector && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/ImenAlSamarai/Documents/mbr/Detector/src/Array.cc > CMakeFiles/Detector.dir/src/Array.cc.i

Detector/CMakeFiles/Detector.dir/src/Array.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Detector.dir/src/Array.cc.s"
	cd /Users/ImenAlSamarai/Documents/mbr/Detector && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/ImenAlSamarai/Documents/mbr/Detector/src/Array.cc -o CMakeFiles/Detector.dir/src/Array.cc.s

Detector/CMakeFiles/Detector.dir/src/Array.cc.o.requires:

.PHONY : Detector/CMakeFiles/Detector.dir/src/Array.cc.o.requires

Detector/CMakeFiles/Detector.dir/src/Array.cc.o.provides: Detector/CMakeFiles/Detector.dir/src/Array.cc.o.requires
	$(MAKE) -f Detector/CMakeFiles/Detector.dir/build.make Detector/CMakeFiles/Detector.dir/src/Array.cc.o.provides.build
.PHONY : Detector/CMakeFiles/Detector.dir/src/Array.cc.o.provides

Detector/CMakeFiles/Detector.dir/src/Array.cc.o.provides.build: Detector/CMakeFiles/Detector.dir/src/Array.cc.o


Detector/CMakeFiles/Detector.dir/src/Station.cc.o: Detector/CMakeFiles/Detector.dir/flags.make
Detector/CMakeFiles/Detector.dir/src/Station.cc.o: Detector/src/Station.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/ImenAlSamarai/Documents/mbr/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object Detector/CMakeFiles/Detector.dir/src/Station.cc.o"
	cd /Users/ImenAlSamarai/Documents/mbr/Detector && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Detector.dir/src/Station.cc.o -c /Users/ImenAlSamarai/Documents/mbr/Detector/src/Station.cc

Detector/CMakeFiles/Detector.dir/src/Station.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Detector.dir/src/Station.cc.i"
	cd /Users/ImenAlSamarai/Documents/mbr/Detector && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/ImenAlSamarai/Documents/mbr/Detector/src/Station.cc > CMakeFiles/Detector.dir/src/Station.cc.i

Detector/CMakeFiles/Detector.dir/src/Station.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Detector.dir/src/Station.cc.s"
	cd /Users/ImenAlSamarai/Documents/mbr/Detector && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/ImenAlSamarai/Documents/mbr/Detector/src/Station.cc -o CMakeFiles/Detector.dir/src/Station.cc.s

Detector/CMakeFiles/Detector.dir/src/Station.cc.o.requires:

.PHONY : Detector/CMakeFiles/Detector.dir/src/Station.cc.o.requires

Detector/CMakeFiles/Detector.dir/src/Station.cc.o.provides: Detector/CMakeFiles/Detector.dir/src/Station.cc.o.requires
	$(MAKE) -f Detector/CMakeFiles/Detector.dir/build.make Detector/CMakeFiles/Detector.dir/src/Station.cc.o.provides.build
.PHONY : Detector/CMakeFiles/Detector.dir/src/Station.cc.o.provides

Detector/CMakeFiles/Detector.dir/src/Station.cc.o.provides.build: Detector/CMakeFiles/Detector.dir/src/Station.cc.o


Detector/CMakeFiles/Detector.dir/src/Channel.cc.o: Detector/CMakeFiles/Detector.dir/flags.make
Detector/CMakeFiles/Detector.dir/src/Channel.cc.o: Detector/src/Channel.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/ImenAlSamarai/Documents/mbr/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object Detector/CMakeFiles/Detector.dir/src/Channel.cc.o"
	cd /Users/ImenAlSamarai/Documents/mbr/Detector && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Detector.dir/src/Channel.cc.o -c /Users/ImenAlSamarai/Documents/mbr/Detector/src/Channel.cc

Detector/CMakeFiles/Detector.dir/src/Channel.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Detector.dir/src/Channel.cc.i"
	cd /Users/ImenAlSamarai/Documents/mbr/Detector && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/ImenAlSamarai/Documents/mbr/Detector/src/Channel.cc > CMakeFiles/Detector.dir/src/Channel.cc.i

Detector/CMakeFiles/Detector.dir/src/Channel.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Detector.dir/src/Channel.cc.s"
	cd /Users/ImenAlSamarai/Documents/mbr/Detector && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/ImenAlSamarai/Documents/mbr/Detector/src/Channel.cc -o CMakeFiles/Detector.dir/src/Channel.cc.s

Detector/CMakeFiles/Detector.dir/src/Channel.cc.o.requires:

.PHONY : Detector/CMakeFiles/Detector.dir/src/Channel.cc.o.requires

Detector/CMakeFiles/Detector.dir/src/Channel.cc.o.provides: Detector/CMakeFiles/Detector.dir/src/Channel.cc.o.requires
	$(MAKE) -f Detector/CMakeFiles/Detector.dir/build.make Detector/CMakeFiles/Detector.dir/src/Channel.cc.o.provides.build
.PHONY : Detector/CMakeFiles/Detector.dir/src/Channel.cc.o.provides

Detector/CMakeFiles/Detector.dir/src/Channel.cc.o.provides.build: Detector/CMakeFiles/Detector.dir/src/Channel.cc.o


Detector/CMakeFiles/Detector.dir/src/ChannelGain.cc.o: Detector/CMakeFiles/Detector.dir/flags.make
Detector/CMakeFiles/Detector.dir/src/ChannelGain.cc.o: Detector/src/ChannelGain.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/ImenAlSamarai/Documents/mbr/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object Detector/CMakeFiles/Detector.dir/src/ChannelGain.cc.o"
	cd /Users/ImenAlSamarai/Documents/mbr/Detector && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Detector.dir/src/ChannelGain.cc.o -c /Users/ImenAlSamarai/Documents/mbr/Detector/src/ChannelGain.cc

Detector/CMakeFiles/Detector.dir/src/ChannelGain.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Detector.dir/src/ChannelGain.cc.i"
	cd /Users/ImenAlSamarai/Documents/mbr/Detector && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/ImenAlSamarai/Documents/mbr/Detector/src/ChannelGain.cc > CMakeFiles/Detector.dir/src/ChannelGain.cc.i

Detector/CMakeFiles/Detector.dir/src/ChannelGain.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Detector.dir/src/ChannelGain.cc.s"
	cd /Users/ImenAlSamarai/Documents/mbr/Detector && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/ImenAlSamarai/Documents/mbr/Detector/src/ChannelGain.cc -o CMakeFiles/Detector.dir/src/ChannelGain.cc.s

Detector/CMakeFiles/Detector.dir/src/ChannelGain.cc.o.requires:

.PHONY : Detector/CMakeFiles/Detector.dir/src/ChannelGain.cc.o.requires

Detector/CMakeFiles/Detector.dir/src/ChannelGain.cc.o.provides: Detector/CMakeFiles/Detector.dir/src/ChannelGain.cc.o.requires
	$(MAKE) -f Detector/CMakeFiles/Detector.dir/build.make Detector/CMakeFiles/Detector.dir/src/ChannelGain.cc.o.provides.build
.PHONY : Detector/CMakeFiles/Detector.dir/src/ChannelGain.cc.o.provides

Detector/CMakeFiles/Detector.dir/src/ChannelGain.cc.o.provides.build: Detector/CMakeFiles/Detector.dir/src/ChannelGain.cc.o


Detector/CMakeFiles/Detector.dir/src/ChannelMap.cc.o: Detector/CMakeFiles/Detector.dir/flags.make
Detector/CMakeFiles/Detector.dir/src/ChannelMap.cc.o: Detector/src/ChannelMap.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/ImenAlSamarai/Documents/mbr/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object Detector/CMakeFiles/Detector.dir/src/ChannelMap.cc.o"
	cd /Users/ImenAlSamarai/Documents/mbr/Detector && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Detector.dir/src/ChannelMap.cc.o -c /Users/ImenAlSamarai/Documents/mbr/Detector/src/ChannelMap.cc

Detector/CMakeFiles/Detector.dir/src/ChannelMap.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Detector.dir/src/ChannelMap.cc.i"
	cd /Users/ImenAlSamarai/Documents/mbr/Detector && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/ImenAlSamarai/Documents/mbr/Detector/src/ChannelMap.cc > CMakeFiles/Detector.dir/src/ChannelMap.cc.i

Detector/CMakeFiles/Detector.dir/src/ChannelMap.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Detector.dir/src/ChannelMap.cc.s"
	cd /Users/ImenAlSamarai/Documents/mbr/Detector && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/ImenAlSamarai/Documents/mbr/Detector/src/ChannelMap.cc -o CMakeFiles/Detector.dir/src/ChannelMap.cc.s

Detector/CMakeFiles/Detector.dir/src/ChannelMap.cc.o.requires:

.PHONY : Detector/CMakeFiles/Detector.dir/src/ChannelMap.cc.o.requires

Detector/CMakeFiles/Detector.dir/src/ChannelMap.cc.o.provides: Detector/CMakeFiles/Detector.dir/src/ChannelMap.cc.o.requires
	$(MAKE) -f Detector/CMakeFiles/Detector.dir/build.make Detector/CMakeFiles/Detector.dir/src/ChannelMap.cc.o.provides.build
.PHONY : Detector/CMakeFiles/Detector.dir/src/ChannelMap.cc.o.provides

Detector/CMakeFiles/Detector.dir/src/ChannelMap.cc.o.provides.build: Detector/CMakeFiles/Detector.dir/src/ChannelMap.cc.o


Detector/CMakeFiles/Detector.dir/src/Detector.cc.o: Detector/CMakeFiles/Detector.dir/flags.make
Detector/CMakeFiles/Detector.dir/src/Detector.cc.o: Detector/src/Detector.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/ImenAlSamarai/Documents/mbr/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building CXX object Detector/CMakeFiles/Detector.dir/src/Detector.cc.o"
	cd /Users/ImenAlSamarai/Documents/mbr/Detector && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Detector.dir/src/Detector.cc.o -c /Users/ImenAlSamarai/Documents/mbr/Detector/src/Detector.cc

Detector/CMakeFiles/Detector.dir/src/Detector.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Detector.dir/src/Detector.cc.i"
	cd /Users/ImenAlSamarai/Documents/mbr/Detector && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/ImenAlSamarai/Documents/mbr/Detector/src/Detector.cc > CMakeFiles/Detector.dir/src/Detector.cc.i

Detector/CMakeFiles/Detector.dir/src/Detector.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Detector.dir/src/Detector.cc.s"
	cd /Users/ImenAlSamarai/Documents/mbr/Detector && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/ImenAlSamarai/Documents/mbr/Detector/src/Detector.cc -o CMakeFiles/Detector.dir/src/Detector.cc.s

Detector/CMakeFiles/Detector.dir/src/Detector.cc.o.requires:

.PHONY : Detector/CMakeFiles/Detector.dir/src/Detector.cc.o.requires

Detector/CMakeFiles/Detector.dir/src/Detector.cc.o.provides: Detector/CMakeFiles/Detector.dir/src/Detector.cc.o.requires
	$(MAKE) -f Detector/CMakeFiles/Detector.dir/build.make Detector/CMakeFiles/Detector.dir/src/Detector.cc.o.provides.build
.PHONY : Detector/CMakeFiles/Detector.dir/src/Detector.cc.o.provides

Detector/CMakeFiles/Detector.dir/src/Detector.cc.o.provides.build: Detector/CMakeFiles/Detector.dir/src/Detector.cc.o


Detector/CMakeFiles/Detector.dir/src/Antenna.cc.o: Detector/CMakeFiles/Detector.dir/flags.make
Detector/CMakeFiles/Detector.dir/src/Antenna.cc.o: Detector/src/Antenna.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/ImenAlSamarai/Documents/mbr/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Building CXX object Detector/CMakeFiles/Detector.dir/src/Antenna.cc.o"
	cd /Users/ImenAlSamarai/Documents/mbr/Detector && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Detector.dir/src/Antenna.cc.o -c /Users/ImenAlSamarai/Documents/mbr/Detector/src/Antenna.cc

Detector/CMakeFiles/Detector.dir/src/Antenna.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Detector.dir/src/Antenna.cc.i"
	cd /Users/ImenAlSamarai/Documents/mbr/Detector && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/ImenAlSamarai/Documents/mbr/Detector/src/Antenna.cc > CMakeFiles/Detector.dir/src/Antenna.cc.i

Detector/CMakeFiles/Detector.dir/src/Antenna.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Detector.dir/src/Antenna.cc.s"
	cd /Users/ImenAlSamarai/Documents/mbr/Detector && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/ImenAlSamarai/Documents/mbr/Detector/src/Antenna.cc -o CMakeFiles/Detector.dir/src/Antenna.cc.s

Detector/CMakeFiles/Detector.dir/src/Antenna.cc.o.requires:

.PHONY : Detector/CMakeFiles/Detector.dir/src/Antenna.cc.o.requires

Detector/CMakeFiles/Detector.dir/src/Antenna.cc.o.provides: Detector/CMakeFiles/Detector.dir/src/Antenna.cc.o.requires
	$(MAKE) -f Detector/CMakeFiles/Detector.dir/build.make Detector/CMakeFiles/Detector.dir/src/Antenna.cc.o.provides.build
.PHONY : Detector/CMakeFiles/Detector.dir/src/Antenna.cc.o.provides

Detector/CMakeFiles/Detector.dir/src/Antenna.cc.o.provides.build: Detector/CMakeFiles/Detector.dir/src/Antenna.cc.o


Detector/CMakeFiles/Detector.dir/src/DetectorSetup.cc.o: Detector/CMakeFiles/Detector.dir/flags.make
Detector/CMakeFiles/Detector.dir/src/DetectorSetup.cc.o: Detector/src/DetectorSetup.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/ImenAlSamarai/Documents/mbr/CMakeFiles --progress-num=$(CMAKE_PROGRESS_10) "Building CXX object Detector/CMakeFiles/Detector.dir/src/DetectorSetup.cc.o"
	cd /Users/ImenAlSamarai/Documents/mbr/Detector && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Detector.dir/src/DetectorSetup.cc.o -c /Users/ImenAlSamarai/Documents/mbr/Detector/src/DetectorSetup.cc

Detector/CMakeFiles/Detector.dir/src/DetectorSetup.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Detector.dir/src/DetectorSetup.cc.i"
	cd /Users/ImenAlSamarai/Documents/mbr/Detector && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/ImenAlSamarai/Documents/mbr/Detector/src/DetectorSetup.cc > CMakeFiles/Detector.dir/src/DetectorSetup.cc.i

Detector/CMakeFiles/Detector.dir/src/DetectorSetup.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Detector.dir/src/DetectorSetup.cc.s"
	cd /Users/ImenAlSamarai/Documents/mbr/Detector && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/ImenAlSamarai/Documents/mbr/Detector/src/DetectorSetup.cc -o CMakeFiles/Detector.dir/src/DetectorSetup.cc.s

Detector/CMakeFiles/Detector.dir/src/DetectorSetup.cc.o.requires:

.PHONY : Detector/CMakeFiles/Detector.dir/src/DetectorSetup.cc.o.requires

Detector/CMakeFiles/Detector.dir/src/DetectorSetup.cc.o.provides: Detector/CMakeFiles/Detector.dir/src/DetectorSetup.cc.o.requires
	$(MAKE) -f Detector/CMakeFiles/Detector.dir/build.make Detector/CMakeFiles/Detector.dir/src/DetectorSetup.cc.o.provides.build
.PHONY : Detector/CMakeFiles/Detector.dir/src/DetectorSetup.cc.o.provides

Detector/CMakeFiles/Detector.dir/src/DetectorSetup.cc.o.provides.build: Detector/CMakeFiles/Detector.dir/src/DetectorSetup.cc.o


Detector/CMakeFiles/Detector.dir/DetectorSetupDictionary.cc.o: Detector/CMakeFiles/Detector.dir/flags.make
Detector/CMakeFiles/Detector.dir/DetectorSetupDictionary.cc.o: Detector/DetectorSetupDictionary.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/ImenAlSamarai/Documents/mbr/CMakeFiles --progress-num=$(CMAKE_PROGRESS_11) "Building CXX object Detector/CMakeFiles/Detector.dir/DetectorSetupDictionary.cc.o"
	cd /Users/ImenAlSamarai/Documents/mbr/Detector && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Detector.dir/DetectorSetupDictionary.cc.o -c /Users/ImenAlSamarai/Documents/mbr/Detector/DetectorSetupDictionary.cc

Detector/CMakeFiles/Detector.dir/DetectorSetupDictionary.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Detector.dir/DetectorSetupDictionary.cc.i"
	cd /Users/ImenAlSamarai/Documents/mbr/Detector && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/ImenAlSamarai/Documents/mbr/Detector/DetectorSetupDictionary.cc > CMakeFiles/Detector.dir/DetectorSetupDictionary.cc.i

Detector/CMakeFiles/Detector.dir/DetectorSetupDictionary.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Detector.dir/DetectorSetupDictionary.cc.s"
	cd /Users/ImenAlSamarai/Documents/mbr/Detector && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/ImenAlSamarai/Documents/mbr/Detector/DetectorSetupDictionary.cc -o CMakeFiles/Detector.dir/DetectorSetupDictionary.cc.s

Detector/CMakeFiles/Detector.dir/DetectorSetupDictionary.cc.o.requires:

.PHONY : Detector/CMakeFiles/Detector.dir/DetectorSetupDictionary.cc.o.requires

Detector/CMakeFiles/Detector.dir/DetectorSetupDictionary.cc.o.provides: Detector/CMakeFiles/Detector.dir/DetectorSetupDictionary.cc.o.requires
	$(MAKE) -f Detector/CMakeFiles/Detector.dir/build.make Detector/CMakeFiles/Detector.dir/DetectorSetupDictionary.cc.o.provides.build
.PHONY : Detector/CMakeFiles/Detector.dir/DetectorSetupDictionary.cc.o.provides

Detector/CMakeFiles/Detector.dir/DetectorSetupDictionary.cc.o.provides.build: Detector/CMakeFiles/Detector.dir/DetectorSetupDictionary.cc.o


# Object files for target Detector
Detector_OBJECTS = \
"CMakeFiles/Detector.dir/src/Atmosphere.cc.o" \
"CMakeFiles/Detector.dir/src/Array.cc.o" \
"CMakeFiles/Detector.dir/src/Station.cc.o" \
"CMakeFiles/Detector.dir/src/Channel.cc.o" \
"CMakeFiles/Detector.dir/src/ChannelGain.cc.o" \
"CMakeFiles/Detector.dir/src/ChannelMap.cc.o" \
"CMakeFiles/Detector.dir/src/Detector.cc.o" \
"CMakeFiles/Detector.dir/src/Antenna.cc.o" \
"CMakeFiles/Detector.dir/src/DetectorSetup.cc.o" \
"CMakeFiles/Detector.dir/DetectorSetupDictionary.cc.o"

# External object files for target Detector
Detector_EXTERNAL_OBJECTS =

Detector/libDetector.dylib: Detector/CMakeFiles/Detector.dir/src/Atmosphere.cc.o
Detector/libDetector.dylib: Detector/CMakeFiles/Detector.dir/src/Array.cc.o
Detector/libDetector.dylib: Detector/CMakeFiles/Detector.dir/src/Station.cc.o
Detector/libDetector.dylib: Detector/CMakeFiles/Detector.dir/src/Channel.cc.o
Detector/libDetector.dylib: Detector/CMakeFiles/Detector.dir/src/ChannelGain.cc.o
Detector/libDetector.dylib: Detector/CMakeFiles/Detector.dir/src/ChannelMap.cc.o
Detector/libDetector.dylib: Detector/CMakeFiles/Detector.dir/src/Detector.cc.o
Detector/libDetector.dylib: Detector/CMakeFiles/Detector.dir/src/Antenna.cc.o
Detector/libDetector.dylib: Detector/CMakeFiles/Detector.dir/src/DetectorSetup.cc.o
Detector/libDetector.dylib: Detector/CMakeFiles/Detector.dir/DetectorSetupDictionary.cc.o
Detector/libDetector.dylib: Detector/CMakeFiles/Detector.dir/build.make
Detector/libDetector.dylib: Event/libEvent.dylib
Detector/libDetector.dylib: Externals/tinyxml/libtinyxml.dylib
Detector/libDetector.dylib: /Applications/root/lib/libCore.so
Detector/libDetector.dylib: /Applications/root/lib/libCint.so
Detector/libDetector.dylib: /Applications/root/lib/libRIO.so
Detector/libDetector.dylib: /Applications/root/lib/libNet.so
Detector/libDetector.dylib: /Applications/root/lib/libHist.so
Detector/libDetector.dylib: /Applications/root/lib/libGraf.so
Detector/libDetector.dylib: /Applications/root/lib/libGraf3d.so
Detector/libDetector.dylib: /Applications/root/lib/libGpad.so
Detector/libDetector.dylib: /Applications/root/lib/libTree.so
Detector/libDetector.dylib: /Applications/root/lib/libRint.so
Detector/libDetector.dylib: /Applications/root/lib/libPostscript.so
Detector/libDetector.dylib: /Applications/root/lib/libMatrix.so
Detector/libDetector.dylib: /Applications/root/lib/libPhysics.so
Detector/libDetector.dylib: /Applications/root/lib/libMathCore.so
Detector/libDetector.dylib: /Applications/root/lib/libThread.so
Detector/libDetector.dylib: /usr/lib/libdl.dylib
Detector/libDetector.dylib: /Applications/root/lib/libMinuit.so
Detector/libDetector.dylib: /Applications/root/lib/libMathMore.so
Detector/libDetector.dylib: Utilities/libUtil.dylib
Detector/libDetector.dylib: /Applications/root/lib/libCore.so
Detector/libDetector.dylib: /Applications/root/lib/libCint.so
Detector/libDetector.dylib: /Applications/root/lib/libRIO.so
Detector/libDetector.dylib: /Applications/root/lib/libNet.so
Detector/libDetector.dylib: /Applications/root/lib/libHist.so
Detector/libDetector.dylib: /Applications/root/lib/libGraf.so
Detector/libDetector.dylib: /Applications/root/lib/libGraf3d.so
Detector/libDetector.dylib: /Applications/root/lib/libGpad.so
Detector/libDetector.dylib: /Applications/root/lib/libTree.so
Detector/libDetector.dylib: /Applications/root/lib/libRint.so
Detector/libDetector.dylib: /Applications/root/lib/libPostscript.so
Detector/libDetector.dylib: /Applications/root/lib/libMatrix.so
Detector/libDetector.dylib: /Applications/root/lib/libPhysics.so
Detector/libDetector.dylib: /Applications/root/lib/libMathCore.so
Detector/libDetector.dylib: /Applications/root/lib/libThread.so
Detector/libDetector.dylib: /usr/lib/libdl.dylib
Detector/libDetector.dylib: /Applications/root/lib/libMinuit.so
Detector/libDetector.dylib: /Applications/root/lib/libMathMore.so
Detector/libDetector.dylib: Detector/CMakeFiles/Detector.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/ImenAlSamarai/Documents/mbr/CMakeFiles --progress-num=$(CMAKE_PROGRESS_12) "Linking CXX shared library libDetector.dylib"
	cd /Users/ImenAlSamarai/Documents/mbr/Detector && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Detector.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
Detector/CMakeFiles/Detector.dir/build: Detector/libDetector.dylib

.PHONY : Detector/CMakeFiles/Detector.dir/build

Detector/CMakeFiles/Detector.dir/requires: Detector/CMakeFiles/Detector.dir/src/Atmosphere.cc.o.requires
Detector/CMakeFiles/Detector.dir/requires: Detector/CMakeFiles/Detector.dir/src/Array.cc.o.requires
Detector/CMakeFiles/Detector.dir/requires: Detector/CMakeFiles/Detector.dir/src/Station.cc.o.requires
Detector/CMakeFiles/Detector.dir/requires: Detector/CMakeFiles/Detector.dir/src/Channel.cc.o.requires
Detector/CMakeFiles/Detector.dir/requires: Detector/CMakeFiles/Detector.dir/src/ChannelGain.cc.o.requires
Detector/CMakeFiles/Detector.dir/requires: Detector/CMakeFiles/Detector.dir/src/ChannelMap.cc.o.requires
Detector/CMakeFiles/Detector.dir/requires: Detector/CMakeFiles/Detector.dir/src/Detector.cc.o.requires
Detector/CMakeFiles/Detector.dir/requires: Detector/CMakeFiles/Detector.dir/src/Antenna.cc.o.requires
Detector/CMakeFiles/Detector.dir/requires: Detector/CMakeFiles/Detector.dir/src/DetectorSetup.cc.o.requires
Detector/CMakeFiles/Detector.dir/requires: Detector/CMakeFiles/Detector.dir/DetectorSetupDictionary.cc.o.requires

.PHONY : Detector/CMakeFiles/Detector.dir/requires

Detector/CMakeFiles/Detector.dir/clean:
	cd /Users/ImenAlSamarai/Documents/mbr/Detector && $(CMAKE_COMMAND) -P CMakeFiles/Detector.dir/cmake_clean.cmake
.PHONY : Detector/CMakeFiles/Detector.dir/clean

Detector/CMakeFiles/Detector.dir/depend: Detector/DetectorSetupDictionary.cc
Detector/CMakeFiles/Detector.dir/depend: Detector/DetectorSetupDictionary.h
	cd /Users/ImenAlSamarai/Documents/mbr && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/ImenAlSamarai/Documents/mbr /Users/ImenAlSamarai/Documents/mbr/Detector /Users/ImenAlSamarai/Documents/mbr /Users/ImenAlSamarai/Documents/mbr/Detector /Users/ImenAlSamarai/Documents/mbr/Detector/CMakeFiles/Detector.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : Detector/CMakeFiles/Detector.dir/depend

