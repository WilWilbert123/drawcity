#include "register_types.hpp"

#include <gdextension_interface.h>
#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/godot.hpp>

#include "animation/animation_bridge.hpp"
#include "drawing/drawing_bridge.hpp"
#include "living_canvas/core/engine_manager.hpp"
#include "physics/physics_bridge.hpp"
#include "recognition/recognition_bridge.hpp"
#include "storage/storage_bridge.hpp"

void initialize_living_canvas_module(godot::ModuleInitializationLevel p_level) {
    if (p_level != godot::MODULE_INITIALIZATION_LEVEL_SCENE) {
        return;
    }

    living_canvas::core::EngineManager::instance().initialize();

    godot::ClassDB::register_class<living_canvas::gdext::DrawingBridge>();
    godot::ClassDB::register_class<living_canvas::gdext::AnimationBridge>();
    godot::ClassDB::register_class<living_canvas::gdext::PhysicsBridge>();
    godot::ClassDB::register_class<living_canvas::gdext::RecognitionBridge>();
    godot::ClassDB::register_class<living_canvas::gdext::StorageBridge>();
}

void uninitialize_living_canvas_module(godot::ModuleInitializationLevel p_level) {
    if (p_level != godot::MODULE_INITIALIZATION_LEVEL_SCENE) {
        return;
    }
    living_canvas::core::EngineManager::instance().shutdown();
}

extern "C" {
GDExtensionBool GDE_EXPORT living_canvas_library_init(
    GDExtensionInterfaceGetProcAddress p_get_proc_address,
    const GDExtensionClassLibraryPtr p_library,
    GDExtensionInitialization* r_initialization) {
    godot::GDExtensionBinding::InitObject init_obj(p_get_proc_address, p_library, r_initialization);

    init_obj.register_initializer(initialize_living_canvas_module);
    init_obj.register_terminator(uninitialize_living_canvas_module);
    init_obj.set_minimum_library_initialization_level(godot::MODULE_INITIALIZATION_LEVEL_SCENE);

    return init_obj.init();
}
}

