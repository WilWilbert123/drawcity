cmake_minimum_required(VERSION 3.22)

add_library(living_canvas_gdext SHARED
    ${CMAKE_CURRENT_SOURCE_DIR}/register_types.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/drawing/drawing_bridge.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/animation/animation_bridge.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/physics/physics_bridge.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/recognition/recognition_bridge.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/storage/storage_bridge.cpp
)

target_include_directories(living_canvas_gdext
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/engine/include
)

target_link_libraries(living_canvas_gdext
    PRIVATE
        living_canvas_engine
        godot::cpp
)

set_target_properties(living_canvas_gdext PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
    POSITION_INDEPENDENT_CODE ON
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin"
)

if(ANDROID)
    set_target_properties(living_canvas_gdext PROPERTIES
        OUTPUT_NAME "living_canvas_gdext.android.${LIVING_CANVAS_BUILD_FLAVOR}.arm64"
    )
elseif(APPLE)
    set_target_properties(living_canvas_gdext PROPERTIES
        OUTPUT_NAME "living_canvas_gdext.macos.${LIVING_CANVAS_BUILD_FLAVOR}.arm64"
    )
elseif(UNIX)
    set_target_properties(living_canvas_gdext PROPERTIES
        OUTPUT_NAME "living_canvas_gdext.linux.${LIVING_CANVAS_BUILD_FLAVOR}.x86_64"
    )
elseif(WIN32)
    set_target_properties(living_canvas_gdext PROPERTIES
        OUTPUT_NAME "living_canvas_gdext.windows.${LIVING_CANVAS_BUILD_FLAVOR}.x86_64"
    )
endif()

