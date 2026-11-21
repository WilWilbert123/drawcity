cmake_minimum_required(VERSION 3.22)

project(LivingCanvas
    VERSION 1.0.0
    DESCRIPTION "LIVING CANVAS (Draw City) — Offline Android 2D Drawing, Animation, Physics & World Engine"
    LANGUAGES C CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

if(CMAKE_BUILD_TYPE STREQUAL "Release")
    set(LIVING_CANVAS_BUILD_FLAVOR "release")
    set(GODOTCPP_TARGET "template_release" CACHE STRING "" FORCE)
else()
    set(LIVING_CANVAS_BUILD_FLAVOR "debug")
    set(GODOTCPP_TARGET "template_debug" CACHE STRING "" FORCE)
endif()

set(GODOTCPP_API_VERSION "4.7" CACHE STRING "Target Godot 4.7 GDExtension API" FORCE)
set(GODOTCPP_DISABLE_EXCEPTIONS ON CACHE BOOL "" FORCE)

# 1. SQLite 3 Support (Uses bundled amalgamation if present or system SQLite3 on desktop)
set(SQLITE_AMALGAMATION_DIR "/tmp/sqlite-amalgamation-3500100" CACHE PATH "Path to SQLite3 C amalgamation")
if(ANDROID OR NOT EXISTS "/opt/homebrew/opt/sqlite/include/sqlite3.h")
    if(EXISTS "${SQLITE_AMALGAMATION_DIR}/sqlite3.c")
        message(STATUS "Using SQLite3 C amalgamation from ${SQLITE_AMALGAMATION_DIR}")
        add_library(living_canvas_sqlite3 STATIC "${SQLITE_AMALGAMATION_DIR}/sqlite3.c")
        target_include_directories(living_canvas_sqlite3 PUBLIC "${SQLITE_AMALGAMATION_DIR}")
        target_compile_definitions(living_canvas_sqlite3 PRIVATE
            SQLITE_THREADSAFE=1
            SQLITE_ENABLE_FTS5
            SQLITE_ENABLE_JSON1
            SQLITE_DEFAULT_FOREIGN_KEYS=1
        )
        if(UNIX AND NOT ANDROID)
            target_link_libraries(living_canvas_sqlite3 PUBLIC pthread dl)
        elseif(ANDROID)
            target_link_libraries(living_canvas_sqlite3 PUBLIC log dl)
        endif()
    else()
        message(FATAL_ERROR "SQLite3 C amalgamation not found at ${SQLITE_AMALGAMATION_DIR}")
    endif()
else()
    message(STATUS "Using Homebrew/System SQLite3 at /opt/homebrew/opt/sqlite")
    add_library(living_canvas_sqlite3 INTERFACE)
    target_include_directories(living_canvas_sqlite3 INTERFACE "/opt/homebrew/opt/sqlite/include")
    target_link_libraries(living_canvas_sqlite3 INTERFACE "/opt/homebrew/opt/sqlite/lib/libsqlite3.dylib")
endif()

# 2. Core C++17 LIVING CANVAS Engine Static Library
add_library(living_canvas_engine STATIC
    engine/src/core/engine_config.cpp
    engine/src/core/event_system.cpp
    engine/src/core/engine_manager.cpp
    engine/src/drawing/stroke.cpp
    engine/src/drawing/brush_engine.cpp
    engine/src/drawing/drawing_engine.cpp
    engine/src/animation/keyframe.cpp
    engine/src/animation/animation_clip.cpp
    engine/src/animation/animation_engine.cpp
    engine/src/rigging/joint.cpp
    engine/src/rigging/bone.cpp
    engine/src/rigging/rig_manager.cpp
    engine/src/physics/movement_model.cpp
    engine/src/physics/custom_physics.cpp
    engine/src/recognition/shape_recognizer.cpp
    engine/src/recognition/object_classifier.cpp
    engine/src/interaction/object_relationship.cpp
    engine/src/interaction/interaction_manager.cpp
    engine/src/world/world_manager.cpp
    engine/src/world/world_simulator.cpp
    engine/src/objects/living_object.cpp
    engine/src/objects/character.cpp
    engine/src/objects/animal.cpp
    engine/src/objects/vehicle.cpp
    engine/src/storage/database_manager.cpp
    engine/src/storage/project_repository.cpp
    engine/src/serialization/project_serializer.cpp
    engine/src/history/history_manager.cpp
    engine/src/platform/platform_services.cpp
)

target_include_directories(living_canvas_engine
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/engine/include
)

target_link_libraries(living_canvas_engine
    PUBLIC
        living_canvas_sqlite3
)

# 3. Godot-CPP & GDExtension Bridge
set(GODOT_CPP_DIR "/tmp/godot-cpp-cache" CACHE PATH "Path to godot-cpp source repository")
if(EXISTS "${GODOT_CPP_DIR}/CMakeLists.txt")
    add_subdirectory("${GODOT_CPP_DIR}" "${CMAKE_BINARY_DIR}/godot-cpp-build" EXCLUDE_FROM_ALL)
    add_subdirectory(godot_extension)
else()
    message(WARNING "godot-cpp not found at ${GODOT_CPP_DIR}; building standalone C++17 engine only.")
endif()

