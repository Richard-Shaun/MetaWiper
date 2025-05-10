cmake_minimum_required(VERSION 3.15)
project(meta_wiper VERSION 0.1.0)

# set variables for version, unify the version number in the whole project
set(META_WIPER_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set(META_WIPER_VERSION_MINOR ${PROJECT_VERSION_MINOR})
set(META_WIPER_VERSION_PATCH ${PROJECT_VERSION_PATCH})
set(META_WIPER_VERSION ${PROJECT_VERSION})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory(core)
add_subdirectory(tests)
add_subdirectory(gui)
