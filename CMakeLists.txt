cmake_minimum_required (VERSION 2.6) 

# Project
project(stripper)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake/modules)
find_package(OpenBabel2 REQUIRED)

# Version
set (${PROJECT_NAME}_VERSION_MAJOR 1)
set (${PROJECT_NAME}_VERSION_MINOR 0)
set (${PROJECT_NAME}_VERSION_PATCH 0)
configure_file ("${PROJECT_SOURCE_DIR}/stripperConfig.h.in" "${PROJECT_SOURCE_DIR}/include/stripperConfig.h")

# Release type
set (CMAKE_BUILD_TYPE Release)

# Sieve code
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

# Include files from stripper project
include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/include/scaffolds)
include_directories(${OPENBABEL2_INCLUDE_DIR})

# Executables
add_executable(${PROJECT_NAME} ${sources})
target_link_libraries(${PROJECT_NAME} ${OPENBABEL2_LIBRARIES})
install(TARGETS ${PROJECT_NAME} DESTINATION bin)

# Testing  
enable_testing()

macro(do_test n i s r)
	add_test (StripperTest_${n} stripper --noLog --in ${i} --scaffolds ${s})
	set_tests_properties (StripperTest_${n}
	PROPERTIES PASS_REGULAR_EXPRESSION ${r})
endmacro(do_test)

add_test (StripperTest_1 stripper)
set_tests_properties (StripperTest_1
PROPERTIES PASS_REGULAR_EXPRESSION "ERROR: Command line option '--in' is missing.")

add_test (StripperTest_2 stripper -h)
set_tests_properties (StripperTest_2
PROPERTIES PASS_REGULAR_EXPRESSION "STRIPPER is a tool to extract predefined scaffolds from input molecules")

add_test (StripperTest_3 stripper -v)
set_tests_properties (StripperTest_3
PROPERTIES PASS_REGULAR_EXPRESSION "STRIPPER v1.0.0")

do_test(4 ${PROJECT_SOURCE_DIR}/test/test_1.smi ${PROJECT_SOURCE_DIR}/test/scaffold-murcko1.txt "C1CCC[(]CC1[)]C1CCCC1CCC1CC2C1CCC2")
do_test(5 ${PROJECT_SOURCE_DIR}/test/test_1.smi ${PROJECT_SOURCE_DIR}/test/scaffold-murcko2.txt "C1CCC[(]CC1[)]C1CCCC1C1CC2C1CCC2")
do_test(6 ${PROJECT_SOURCE_DIR}/test/test_1.smi ${PROJECT_SOURCE_DIR}/test/scaffold-oprea1.txt "C1CC1C1CC1C1C2C1C2")
do_test(7 ${PROJECT_SOURCE_DIR}/test/test_1.smi ${PROJECT_SOURCE_DIR}/test/scaffold-oprea2.txt "C[(]C1CC1C1CC1[)]CC1C2C1C2")
do_test(8 ${PROJECT_SOURCE_DIR}/test/test_1.smi ${PROJECT_SOURCE_DIR}/test/scaffold-oprea3.txt "C1CC1C1OC1OC1OC2C1C2")
do_test(9 ${PROJECT_SOURCE_DIR}/test/test_1.smi ${PROJECT_SOURCE_DIR}/test/scaffold-ringswithlinkers1.txt "c1ccc[(]cc1[)]c1nocc1CCC1CN2C1SCC2")
do_test(10 ${PROJECT_SOURCE_DIR}/test/test_1.smi ${PROJECT_SOURCE_DIR}/test/scaffold-ringswithlinkers2.txt "O=C1N2CCSC2C1CC[(]=O[)]c1conc1c1ccccc1")
do_test(11 ${PROJECT_SOURCE_DIR}/test/test_1.smi ${PROJECT_SOURCE_DIR}/test/scaffold-schuffenhauer1.txt "C1CNC1=O")
do_test(12 ${PROJECT_SOURCE_DIR}/test/test_1.smi ${PROJECT_SOURCE_DIR}/test/scaffold-schuffenhauer2.txt "O=C1CC2N1CCS2")
do_test(13 ${PROJECT_SOURCE_DIR}/test/test_1.smi ${PROJECT_SOURCE_DIR}/test/scaffold-schuffenhauer3.txt "O=C1N2CCSC2C1CC[(]=O[)]c1conc1")
do_test(14 ${PROJECT_SOURCE_DIR}/test/test_1.smi ${PROJECT_SOURCE_DIR}/test/scaffold-schuffenhauer4.txt "O=C1N2CCSC2C1CC[(]=O[)]c1conc1c1ccccc1")
do_test(15 ${PROJECT_SOURCE_DIR}/test/test_1.smi ${PROJECT_SOURCE_DIR}/test/scaffold-schuffenhauer5.txt "O=C1N2CCSC2C1CC[(]=O[)]c1conc1c1ccccc1")

# Packaging source tar.gz
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

