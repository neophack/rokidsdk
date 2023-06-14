cmake_minimum_required(VERSION 3.0)

project(libusb_control_transfer_demo)

set(CMAKE_CXX_STANDARD 11)
if(WIN32)
    set( "${CMAKE_CXX_FLAGS} -static")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static")
endif()
# Find libusb
find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBUSB REQUIRED libusb-1.0)

# Source files
set(SRC_FILES glass_control.cpp main.cpp)

# Include directories
include_directories(${LIBUSB_INCLUDE_DIRS})

message(${LIBUSB_INCLUDE_DIRS})
# Link directories
link_directories(${LIBUSB_LIBRARY_DIRS})

# Executable
add_executable(${PROJECT_NAME} ${SRC_FILES})

# Link libraries
target_link_libraries(${PROJECT_NAME} ${LIBUSB_LIBRARIES})


