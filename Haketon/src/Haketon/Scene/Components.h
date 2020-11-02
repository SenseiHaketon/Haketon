﻿#pragma once

#include <glm/glm.hpp>


#include "Haketon/Scene/SceneCamera.h"

namespace Haketon
{
    struct TagComponent
    {
        std::string Tag;

        TagComponent() = default;
        TagComponent(const TagComponent&) = default;
        TagComponent(const std::string tag)
            : Tag(tag) {}
    };
    
    struct TransformComponent
    {
        glm::mat4 Transform { 1.0f };

        TransformComponent() = default;
        TransformComponent(const TransformComponent&) = default;
        TransformComponent(const glm::mat4& transform)
            : Transform(transform) {}

        operator glm::mat4&() { return Transform; }
        operator const glm::mat4&() const { return Transform; }
    };

    struct SpriteRendererComponent
    {
    public:
        glm::vec4 Color { 1.0f, 1.0f, 1.0f, 1.0f };

        SpriteRendererComponent() = default;
        SpriteRendererComponent(const SpriteRendererComponent&) = default;
        SpriteRendererComponent(const glm::vec4& color)
            : Color(color) {}
    };

    struct CameraComponent
    {
        CameraComponent() = default;
        CameraComponent(const CameraComponent&) = default;
        
        SceneCamera Camera;
        bool Primary = false; // TODO: move this to scene
        bool FixedAspectRatio = false;
    };
}