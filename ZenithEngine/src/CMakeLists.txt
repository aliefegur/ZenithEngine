target_sources(${PROJECT_NAME} PRIVATE zenithpch.h ZenithEngine.h)
target_precompile_headers(${PROJECT_NAME} PRIVATE zenithpch.h)
add_subdirectory(Zenith)
