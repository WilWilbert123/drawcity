#include "living_canvas/world/world_simulator.hpp"

#include <cmath>

namespace living_canvas::world {

void WorldSimulator::reset() {
    running_ = false;
    elapsed_sec_ = 0.0f;
}

void WorldSimulator::step(float dt, WorldManager& world, living_canvas::physics::CustomPhysics& physics,
                          const living_canvas::interaction::InteractionManager& interactions) {
    if (!running_ || dt <= 0.0f) {
        return;
    }
    elapsed_sec_ += dt;
    auto& env = world.environment();
    env.time_of_day_hours = std::fmod(env.time_of_day_hours + dt * 0.05f, 24.0f);

    for (const auto& obj : world.objects()) {
        auto& body = physics.ensure_body(obj.id(), obj.x(), obj.y());
        if (obj.category() == "cloud") {
            body.x += env.wind_speed_px_s * dt;
            if (body.x > 2200.0f) {
                body.x = -200.0f;
            }
        }
    }

    // Synchronize mounted characters or held objects with their parent object
    for (const auto& rel : interactions.relationships()) {
        if (!rel.active) {
            continue;
        }
        const auto* target_body = physics.find_body(rel.target_object_id);
        if (!target_body) {
            continue;
        }
        if (rel.type == living_canvas::interaction::RelationshipType::CharacterDrivesVehicle) {
            auto& char_body = physics.ensure_body(rel.source_object_id);
            char_body.x = target_body->x + rel.anchor_offset_x;
            char_body.y = target_body->y + rel.anchor_offset_y;
            char_body.vx = target_body->vx;
            char_body.vy = target_body->vy;
        } else if (rel.type == living_canvas::interaction::RelationshipType::CharacterHoldsObject) {
            const auto* char_body = physics.find_body(rel.source_object_id);
            if (char_body) {
                auto& held_body = physics.ensure_body(rel.target_object_id);
                held_body.x = char_body->x + rel.anchor_offset_x;
                held_body.y = char_body->y + rel.anchor_offset_y;
            }
        }
    }

    // Write updated physics coordinates back to world objects
    for (const auto& obj : world.objects()) {
        if (const auto* body = physics.find_body(obj.id())) {
            if (auto* mut_obj = world.find_mutable_object(obj.id())) {
                mut_obj->set_position(body->x, body->y);
                mut_obj->set_rotation_rad(body->rotation_rad);
            }
        }
    }
}

} // namespace living_canvas::world

