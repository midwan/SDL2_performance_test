cmake_minimum_required(VERSION 3.16)
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH})

project(sdl2_performance_test)

set(CMAKE_CXX_STANDARD 14)

# Set build type to "Release" if user did not specify any build type yet
# Other possible values: Debug, Release, RelWithDebInfo and MinSizeRel
if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif (NOT CMAKE_BUILD_TYPE)

include(FindHelper)
find_package(SDL2 CONFIG REQUIRED)
find_package(SDL2_image MODULE REQUIRED)

include_directories(${SDL2_INCLUDE_DIR} ${SDL2_IMAGE_INCLUDE_DIR} include)

add_executable(sdl2_pixelformat_test
    sdl2_pixelformat_test.cpp)

add_executable(sdl2_renderer_test
        sdl2_renderer_test.cpp)

target_link_libraries(sdl2_pixelformat_test ${SDL2_LIBRARIES})
target_link_libraries(sdl2_renderer_test ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES})