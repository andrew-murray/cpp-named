PROJECT(cpp_named)
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

INCLUDE(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
CONAN_BASIC_SETUP()

INCLUDE_DIRECTORIES("hana/include/")

# Just comment or uncomment the FindPackage line to use it or not
FIND_PACKAGE(Boost 1.60.0)
IF(${Boost_FOUND})
	INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})
ENDIF()

file(GLOB cpp_named_src ${CMAKE_SOURCE_DIR} *.h *.hxx *.hpp *.ipp *.cpp *.cxx *.cc)

ADD_EXECUTABLE(cpp_named_example ${cpp_named_src})

# TARGET_LINK_LIBRARIES(cpp_named_example  ${Boost_LIBRARIES})
TARGET_LINK_LIBRARIES(cpp_named_example ${CONAN_LIBS})
