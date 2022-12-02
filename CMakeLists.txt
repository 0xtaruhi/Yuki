cmake_minimum_required(VERSION 3.16)

project(Yuki LANGUAGES CXX VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# set the default build type to Release
if (NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release
    CACHE STRING "Choose the type of build, options are: Debug Release RelWithDebInfo MinSizeRel."
    FORCE)
endif()

# set flags for different build types
set(CMAKE_CXX_FLAGS_DEBUG "-g -D_DEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")

# set cmake module path
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules/")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

find_package(SFML 2.5.1 
  COMPONENTS
    graphics audio window network system REQUIRED
)
find_package(TMXLITE REQUIRED)

include_directories(
  ${SFML_INCLUDE_DIR}
  ${TMXLITE_INCLUDE_DIR}
  ${CMAKE_SOURCE_DIR}/include
)

set (PROJECT_SRC
  src/Main.cc
  src/Yuki.cc
  src/Map.cc
  src/Soldier.cc
)

if (WIN32)
  add_executable(${PROJECT_NAME} WIN32 ${PROJECT_SRC})
else()
  add_executable(${PROJECT_NAME} ${PROJECT_SRC})
endif()

target_link_libraries(${PROJECT_NAME}
  ${TMXLITE_LIBRARIES}
  ${SFML_LIBRARIES}
)

install(TARGETS ${PROJECT_NAME} DESTINATION bin)
install(DIRECTORY assets DESTINATION bin)