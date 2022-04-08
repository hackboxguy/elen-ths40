cmake_minimum_required(VERSION 2.8.9)
set(COMPONENT_NAME elen-ths40)  #set the name of the binary

project(${COMPONENT_NAME})
file(GLOB SOURCES "*.cpp")
list (APPEND SOURCES ${SOURCES2})

# Add #include path of all dependent components
include_directories(${CMAKE_CURRENT_BINARY_DIR})
add_executable(${COMPONENT_NAME} ${SOURCES})

install(TARGETS ${COMPONENT_NAME} DESTINATION sbin)

# Specify component's dependency.
target_link_libraries(${COMPONENT_NAME} pthread z rt)

#how to build i2c-iow on linux(make sure cmake is installed)
#cmake -H. -BOutput -DCMAKE_INSTALL_PREFIX=~/tmp/elen-ths40/
#cmake --build Output -- install -j$(nproc)
