cmake_minimum_required (VERSION 2.6) 

#
# Project
project(stripper) 
#
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake/modules)
find_package(OpenBabel2 REQUIRED)
         

#
# Version 
#
set (${PROJECT_NAME}_VERSION_MAJOR 1)
set (${PROJECT_NAME}_VERSION_MINOR 0)
set (${PROJECT_NAME}_VERSION_PATCH 0)
configure_file ("${PROJECT_SOURCE_DIR}/stripperConfig.h.in" "${PROJECT_SOURCE_DIR}/include/stripperConfig.h")
 
  
#
# Release type  
#
set (CMAKE_BUILD_TYPE Release)
 

#
# Source code  
#
set (StripperDir ${PROJECT_SOURCE_DIR}/src)
set (sources	${StripperDir}/main.cpp
				${StripperDir}/counter.cpp
				${StripperDir}/calculator.cpp
				${StripperDir}/options.cpp
				${StripperDir}/parseCommandLine.cpp
				${StripperDir}/parseScaffoldDefinitions.cpp
				${StripperDir}/scaffolds/murcko_1.cpp
				${StripperDir}/scaffolds/murcko_2.cpp
				${StripperDir}/scaffolds/oprea.cpp
				${StripperDir}/scaffolds/oprea_1.cpp
				${StripperDir}/scaffolds/oprea_2.cpp
				${StripperDir}/scaffolds/oprea_3.cpp
				${StripperDir}/scaffolds/rings_with_linkers_1.cpp
				${StripperDir}/scaffolds/rings_with_linkers_2.cpp
				${StripperDir}/scaffolds/scaffold.cpp
				${StripperDir}/scaffolds/schuffenhauer.cpp
				${StripperDir}/scaffolds/schuffenhauer_1.cpp
				${StripperDir}/scaffolds/schuffenhauer_2.cpp
				${StripperDir}/scaffolds/schuffenhauer_3.cpp
				${StripperDir}/scaffolds/schuffenhauer_4.cpp
				${StripperDir}/scaffolds/schuffenhauer_5.cpp
)
 

#
# Include files
#
include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/include/scaffolds)
include_directories(${OPENBABEL2_INCLUDE_DIR})
 

#
# Executables    
#
add_executable(${PROJECT_NAME} ${sources})
target_link_libraries(${PROJECT_NAME} ${OPENBABEL2_LIBRARIES})
install(TARGETS ${PROJECT_NAME} DESTINATION bin)
           

#
# Packaging source tar.gz
#
set(CPACK_PACKAGE_NAME ${PROJECT_NAME})
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Stripper - a tool to extract scaffolds from organics molecules")
set(CPACK_PACKAGE_VENDOR "Silicos NV")
set(CPACK_PACKAGE_VERSION_MAJOR @stripper_VERSION_MAJOR@)
set(CPACK_PACKAGE_VERSION_MINOR @stripper_VERSION_MINOR@)
set(CPACK_PACKAGE_VERSION_PATCH @stripper_VERSION_PATCH@)

set(CPACK_RESOURCE_FILE_LICENSE "COPYING.txt")
set(CPACK_RESOURCE_FILE_README  "README.txt")
set(CPACK_RESOURCE_FILE_INSTALL "INSTALL.txt")

set(CPACK_SOURCE_GENERATOR "TGZ")
set(CPACK_SOURCE_IGNORE_FILES 
	".svn;/CMakeFiles/;.DS_Store;/_CPack_Packages/;CMakeCache.txt;stripper.tmproj;Makefile;cmake_install.cmake;project.tmproj;
	stripper-1.0.0-Darwin.dmg;stripper-1.0.0-Darwin.sh;stripper-1.0.0-Darwin.tar.gz"
)
set(CPACK_SOURCE_PACKAGE_FILE_NAME "${PROJECT_NAME}-${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH}")

include(CPack)


#
# Testing
#
enable_testing()
include(${CMAKE_MODULE_PATH}/IncludeAllTests.cmake)
