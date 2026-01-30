cmake_minimum_required(VERSION 3.20)

# Just for prevent warning
project(MyCMakeProject)

set(ZENITH_SHARED false)

include(FindPkgConfig)

# Including the projects
add_subdirectory(ZenithEngine)

# Demo projects
add_subdirectory(Demos/TestApp)
add_subdirectory(Demos/FlappyBird)
