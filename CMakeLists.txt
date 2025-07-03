cmake_minimum_required(VERSION 3.20)

set(ZENITH_SHARED true)

# Just for prevent warning
project(MyCMakeProject)

# Including the projects
add_subdirectory(ZenithEngine)

# Demo projects
add_subdirectory(Demos/TestApp)
#add_subdirectory(Demos/FlappyBird)
