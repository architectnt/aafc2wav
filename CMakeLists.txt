cmake_minimum_required(VERSION 3.11)
project(aafc2wav VERSION 1 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

add_executable(aafc2wav
	include/aafc.h
	include/fileutils.h
	include/libaafcfunc.h
	src/aafc2wav.cpp
)

include_directories(${aafc2wav_SOURCE_DIR}/include)
include_directories(${PROJECT_BINARY_DIR}/include)

install(TARGETS aafc2wav
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
	ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
	RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)