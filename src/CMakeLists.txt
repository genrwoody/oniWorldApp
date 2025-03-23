set(SETTING_HEADER_FILES
    "Setting/ClusterLayout.hpp"
    "Setting/DefaultSettings.hpp"
    "Setting/SampleDescriber.hpp"
    "Setting/SimHashes.hpp"
    "Setting/TemplateContainer.hpp"
    "Setting/World.hpp"
    "Setting/WorldGenClasses.hpp"
)

add_custom_command(OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/JsonDeserializeGen.hpp"
    COMMAND ${PYTHON_CMD} "${CMAKE_CURRENT_SOURCE_DIR}/JsonDeserializeGen.py"
                          "${CMAKE_CURRENT_SOURCE_DIR}/Setting"
    DEPENDS ${SETTING_HEADER_FILES}
    WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
    VERBATIM
)

add_custom_command(OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/data/data.zip"
    COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_CURRENT_BINARY_DIR}/data"
    COMMAND ${CMAKE_COMMAND} -E echo "keep" > "${CMAKE_CURRENT_BINARY_DIR}/data/!keep"
    COMMAND ${CMAKE_COMMAND} -E tar cf "${CMAKE_CURRENT_BINARY_DIR}/data/data.zip"
                --format=zip dlc templates worldgen Asubworlds.json
    WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/asset"
    VERBATIM
)

add_custom_target(generate
    DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/JsonDeserializeGen.hpp"
            "${CMAKE_CURRENT_BINARY_DIR}/data/data.zip"
)

add_executable(oniWorldApp
    ${SETTING_HEADER_FILES}
    "Setting/ComposableDictionary.hpp"
    "Setting/JsonDeserialize.hpp"
    "Setting/SettingsCache.hpp"
    "Setting/WorldGenClasses.cpp"
    "Setting/SettingsCache.cpp"
    "Utils/ConvexHull/Constants.hpp"
    "Utils/ConvexHull/Algorithm.hpp"
    "Utils/ConvexHull/Algorithm.cpp"
    "Utils/ConvexHull/DataModel.hpp"
    "Utils/ConvexHull/MathHelper.hpp"
    "Utils/ConvexHull/MathHelper.cpp"
    "Utils/ConvexHull.hpp"
    "Utils/ConvexHull.cpp"
    "Utils/KRandom.hpp"
    "Utils/KRandom.cpp"
    "Utils/PointGenerator.hpp"
    "Utils/PointGenerator.cpp"
    "Utils/Vector2f.hpp"
    "Utils/Polygon.hpp"
    "Utils/Polygon.cpp"
    "Utils/Halfedge.hpp"
    "Utils/Halfedge.cpp"
    "Utils/Voronoi.hpp"
    "Utils/Voronoi.cpp"
    "Utils/Diagram.hpp"
    "Utils/Diagram.cpp"
    "WorldGen.hpp"
    "WorldGen.cpp"
    "TemplateSpawning.cpp"
    "main.cpp"
)
if(EMSCRIPTEN)
target_link_options(oniWorldApp
    PUBLIC "-sALLOW_MEMORY_GROWTH" "-sTOTAL_MEMORY=33554432"
    PUBLIC "--preload-file" "${CMAKE_CURRENT_BINARY_DIR}/data@/"
    PUBLIC "--js-library" "${CMAKE_CURRENT_SOURCE_DIR}/getresult.js")
elseif(MSVC)
target_compile_options(oniWorldApp PUBLIC "/source-charset:utf-8" "/execution-charset:GBK")
endif()
target_include_directories(oniWorldApp
    PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}"
    PUBLIC "${CMAKE_CURRENT_BINARY_DIR}"
    PUBLIC "${CMAKE_SOURCE_DIR}/3rdparty/clipper"
    PUBLIC "${CMAKE_SOURCE_DIR}/3rdparty/jsoncpp"
    PUBLIC "${CMAKE_SOURCE_DIR}/3rdparty/miniz"
)
target_link_libraries(oniWorldApp PUBLIC clipper jsoncpp miniz)

add_dependencies(oniWorldApp generate)

configure_file("config.h.in" "config.h")
