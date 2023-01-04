cmake_minimum_required(VERSION 3.16)

project(Yuki LANGUAGES CXX VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)
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

# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -Werror")

# set cmake module path
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(SFML_STATIC_LIBRARIES TRUE)
set(SFML_DIR deps/SFML-2.5.1/lib/cmake/SFML)

find_package(SFML 2.5.1 COMPONENTS graphics audio REQUIRED)

set(SFML_LIBRARIES sfml-graphics sfml-window sfml-system)

include_directories(
  ${SFML_INCLUDE_DIR}
  ${CMAKE_SOURCE_DIR}/include
)

set (PROJECT_SRC
  src/Main.cpp
  src/Sodier.cpp
  src/Button.cpp
  src/InputProcessEngine.cpp
  src/Elementum.cpp
  src/StatusBar.cpp
  src/Menu.cpp
  src/YukiScene.cpp
  src/StartupScene.cpp
  src/MainScene.cpp
  src/Tile.cpp
  src/Map.cpp
  src/MilitaryBase.cpp
  src/FloatingBubble.cpp
  src/InfoBar.cpp
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
