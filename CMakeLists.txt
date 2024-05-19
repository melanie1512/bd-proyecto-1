cmake_minimum_required(VERSION 3.27)
project(proyecto1)

set(CMAKE_CXX_STANDARD 14)

include_directories(tecnicas/AVL)

# Añadir fuentes
add_executable(proyecto1
        tecnicas/AVL/main.cpp
        tecnicas/AVL/AVL.cpp
)

# Incluir FetchContent
include(FetchContent)

# Descargar nlohmann/json
FetchContent_Declare(
        json
        GIT_REPOSITORY https://github.com/nlohmann/json.git
        GIT_TAG v3.10.5
)
FetchContent_MakeAvailable(json)

# Descargar cpp-httplib
FetchContent_Declare(
        httplib
        GIT_REPOSITORY https://github.com/yhirose/cpp-httplib.git
        GIT_TAG v0.9.9
)
FetchContent_MakeAvailable(httplib)

# Linkear las bibliotecas descargadas
target_link_libraries(proyecto1 PRIVATE httplib::httplib nlohmann_json::nlohmann_json)
