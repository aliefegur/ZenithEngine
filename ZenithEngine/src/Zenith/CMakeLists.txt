target_sources(${PROJECT_NAME} PRIVATE API.h Core.h)

add_subdirectory(App)
add_subdirectory(Audio)
add_subdirectory(Input)
add_subdirectory(Platform)
add_subdirectory(Renderer)
add_subdirectory(Utils)
