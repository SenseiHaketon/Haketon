﻿#pragma once

#include "Scene.h"

namespace Haketon
{
    class SceneSerializer
    {
    public:
        SceneSerializer(const Ref<Scene>& scene);

        void SerializeText(const std::string& filePath);
        void SerializeBinary(const std::string& filePath);

        bool DeserializeText(const std::string& filePath);
        bool DeserializeBinary(const std::string& filePath);
    private:
        Ref<Scene> m_Scene;
    };

}