# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

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
CMAKE_SOURCE_DIR = /home/zmai/文档/code_program/git_ku/Real_Time_Embedded_Team_Project/Lidar/src/Qt/Lida_service

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/zmai/文档/code_program/git_ku/Real_Time_Embedded_Team_Project/Lidar/src/Qt/Lida_service

# Utility rule file for Lida_service_autogen.

# Include any custom commands dependencies for this target.
include CMakeFiles/Lida_service_autogen.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/Lida_service_autogen.dir/progress.make

CMakeFiles/Lida_service_autogen:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/zmai/文档/code_program/git_ku/Real_Time_Embedded_Team_Project/Lidar/src/Qt/Lida_service/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Automatic MOC and UIC for target Lida_service"
	/usr/bin/cmake -E cmake_autogen /home/zmai/文档/code_program/git_ku/Real_Time_Embedded_Team_Project/Lidar/src/Qt/Lida_service/CMakeFiles/Lida_service_autogen.dir/AutogenInfo.json ""

Lida_service_autogen: CMakeFiles/Lida_service_autogen
Lida_service_autogen: CMakeFiles/Lida_service_autogen.dir/build.make
.PHONY : Lida_service_autogen

# Rule to build all files generated by this target.
CMakeFiles/Lida_service_autogen.dir/build: Lida_service_autogen
.PHONY : CMakeFiles/Lida_service_autogen.dir/build

CMakeFiles/Lida_service_autogen.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/Lida_service_autogen.dir/cmake_clean.cmake
.PHONY : CMakeFiles/Lida_service_autogen.dir/clean

CMakeFiles/Lida_service_autogen.dir/depend:
	cd /home/zmai/文档/code_program/git_ku/Real_Time_Embedded_Team_Project/Lidar/src/Qt/Lida_service && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/zmai/文档/code_program/git_ku/Real_Time_Embedded_Team_Project/Lidar/src/Qt/Lida_service /home/zmai/文档/code_program/git_ku/Real_Time_Embedded_Team_Project/Lidar/src/Qt/Lida_service /home/zmai/文档/code_program/git_ku/Real_Time_Embedded_Team_Project/Lidar/src/Qt/Lida_service /home/zmai/文档/code_program/git_ku/Real_Time_Embedded_Team_Project/Lidar/src/Qt/Lida_service /home/zmai/文档/code_program/git_ku/Real_Time_Embedded_Team_Project/Lidar/src/Qt/Lida_service/CMakeFiles/Lida_service_autogen.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/Lida_service_autogen.dir/depend

