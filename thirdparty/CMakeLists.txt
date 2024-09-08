add_subdirectory(QFontIcon)
add_subdirectory(reactphysics3d)

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
    ${CMAKE_CURRENT_SOURCE_DIR}/reactphysics3d/include
    ${CMAKE_CURRENT_SOURCE_DIR}/magic_enum/
    PARENT_SCOPE
)
