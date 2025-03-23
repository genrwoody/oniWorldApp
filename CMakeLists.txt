cmake_minimum_required (VERSION 3.20)

project("CMakeApp")

set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>")
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_program(PYTHON_CMD "python" REQUIRED)

add_library(clipper STATIC
    "3rdparty/clipper/clipper.cpp"
)
target_include_directories(clipper
    PUBLIC "${CMAKE_SOURCE_DIR}/3rdparty/clipper"
)

add_library(jsoncpp STATIC
    "3rdparty/jsoncpp/json_reader.cpp"
    "3rdparty/jsoncpp/json_writer.cpp"
    "3rdparty/jsoncpp/json_value.cpp"
)
target_include_directories(jsoncpp
    PUBLIC "${CMAKE_SOURCE_DIR}/3rdparty/jsoncpp"
)

add_library(miniz STATIC
    "3rdparty/miniz/miniz.c"
    "3rdparty/miniz/miniz_tdef.c"
    "3rdparty/miniz/miniz_tinfl.c"
    "3rdparty/miniz/miniz_zip.c"
)
target_compile_definitions(miniz PUBLIC MINIZ_NO_TIME)
target_include_directories(miniz
    PUBLIC "${CMAKE_CURRENT_BINARY_DIR}"
    PUBLIC "${CMAKE_SOURCE_DIR}/3rdparty/miniz"
)
set_target_properties(miniz
    PROPERTIES INTERFACE_COMPILE_DEFINITIONS MINIZ_STATIC_DEFINE)

add_subdirectory(src)
