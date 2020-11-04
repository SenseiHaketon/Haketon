﻿#include "hkpch.h"
#include "Scene.h"
#include "Haketon/Renderer/Renderer2D.h"
#include "Components.h"
#include "Entity.h"

#include <glm/glm.hpp>

namespace Haketon
{
    Scene::Scene()
    {
    }

    Scene::~Scene()
    {
        
    }

    Entity Scene::CreateEntity(const std::string& name)
    {
        Entity entity = { m_Registry.create(), this };

        // TODO: Do I want every entity to have a transform component? If not, also add a check to SceneSerializer
        entity.AddComponent<TransformComponent>();
        // TODO: Maybe instead of tag component, save name in entity class? (Although we want to keep it small, so we can easily copy it)
        entity.AddComponent<TagComponent>(name);
        return entity;
    }

    void Scene::DestroyEntity(Entity entity)
    {
        m_Registry.destroy(entity);
    }

    void Scene::OnUpdate(Timestep ts)
    {
        m_Registry.view<NativeScriptComponent>().each([=](auto entity, NativeScriptComponent& nsc)
        {
           if(!nsc.Instance)
           {
               nsc.Instance = nsc.InstantiateScript();
               nsc.Instance->m_Entity = Entity{ entity, this };
               nsc.Instance->OnCreate();
           }

            nsc.Instance->OnUpdate(ts);
        });
    
        
        // Render 2D
        Camera* primaryCamera = nullptr;
        glm::mat4 cameraTransform;
        auto cameraCompGroup = m_Registry.group<CameraComponent>(entt::get<TransformComponent>); // TODO: WHY CANT I USE TWO GROUPS??
        for(auto entity : cameraCompGroup)
        {
            auto [transform, camera] = cameraCompGroup.get<TransformComponent, CameraComponent>(entity);
            if(camera.Primary)
            {
                primaryCamera = &camera.Camera;
                cameraTransform = transform.GetTransform();
                break;
            }
        }

        if(primaryCamera)
        {
            Renderer2D::BeginScene(*primaryCamera, cameraTransform);

            auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
            for(auto entity : group)
            {
                auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);

                Renderer2D::DrawQuad(transform.GetTransform(), sprite.Color);
            }

            Renderer2D::EndScene();
        }       
    }

    void Scene::OnViewportResize(uint32_t width, uint32_t height)
    {
        m_ViewportWidth = width;
        m_ViewportHeight = height;

        // Resize cameras
        auto view = m_Registry.view<CameraComponent>();
        for(auto entity : view)
        {
            auto& cameraComponent = view.get<CameraComponent>(entity);
            if(!cameraComponent.FixedAspectRatio)
                cameraComponent.Camera.SetViewportSize(width, height);
        }
    }

    template<typename T>
    void Scene::OnComponentAdded(Entity entity, T& component)
    {
        static_assert(false);
    }

    template<>
    void Scene::OnComponentAdded<TagComponent>(Entity entity, TagComponent& component)
    {        
    }
    
    template<>
    void Scene::OnComponentAdded<TransformComponent>(Entity entity, TransformComponent& component)
    {        
    }

    template<>
    void Scene::OnComponentAdded<CameraComponent>(Entity entity, CameraComponent& component)
    {
        component.Camera.SetViewportSize(m_ViewportWidth, m_ViewportHeight);
    }

    template<>
    void Scene::OnComponentAdded<SpriteRendererComponent>(Entity entity, SpriteRendererComponent& component)
    {        
    }

    template<>
    void Scene::OnComponentAdded<NativeScriptComponent>(Entity entity, NativeScriptComponent& component)
    {        
    }
}
