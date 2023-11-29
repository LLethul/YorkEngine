cmake_minimum_required (VERSION 3.8)

project ("York")
file(GLOB_RECURSE SOURCES src/*.cpp)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1) # works
find_package(Vulkan REQUIRED)

include_directories(${Vulkan_INCLUDE_DIRS})
include_directories(include/)
include_directories(C:/VulkanSDK/1.3.268.0/Include)
link_directories(lib)

add_executable (York ${SOURCES})

target_link_libraries(York glfw3 ${Vulkan_LIBRARIES})
message(${Vulkan_INCLUDE_DIRS})

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET York PROPERTY CXX_STANDARD 20)
endif()