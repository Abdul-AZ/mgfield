add_subdirectory(QFontIcon)
add_subdirectory(reactphysics3d)

message(STATUS ${SOURCES})
set(SOURCES
    ${SOURCES}
    ${CMAKE_CURRENT_SOURCE_DIR}/eng_format/eng_format.cpp thirdparty/eng_format/eng_format.hpp

    ${CMAKE_CURRENT_SOURCE_DIR}/tinygltf/json.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/tinygltf/stb_image.h
    ${CMAKE_CURRENT_SOURCE_DIR}/tinygltf/stb_image_write.h
    ${CMAKE_CURRENT_SOURCE_DIR}/tinygltf/tiny_gltf.cpp thirdparty/tinygltf/tiny_gltf.h

    ${CMAKE_CURRENT_SOURCE_DIR}/magic_enum/magic_enum.hpp

    PARENT_SCOPE
)


set(LIBS
    ${LIBS}
    QFontIcon
    reactphysics3d
    PARENT_SCOPE
)

set(INCLUDE_DIRS
    ${CMAKE_CURRENT_SOURCE_DIR}/
    ${CMAKE_CURRENT_SOURCE_DIR}/reactphysics3d/include
    ${CMAKE_CURRENT_SOURCE_DIR}/magic_enum/
    PARENT_SCOPE
)

# Testing a new method of organizing sources

# Link libheatmap
add_library(libheatmap STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/libheatmap/heatmap.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/libheatmap/heatmap.hpp
)
target_link_libraries(mgfield PRIVATE libheatmap)

# Link githash
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/GitHash/)
target_link_libraries(mgfield PRIVATE "${GITHASH_LIBRARIES}")

