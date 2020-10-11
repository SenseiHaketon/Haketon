﻿#pragma once

#include <string>

#include "Haketon/Core/Core.h"

namespace Haketon
{
    class Texture
    {
    public:
        virtual ~Texture() = default;
        
        virtual uint32_t GetWidth() const = 0;
        virtual uint32_t GetHeight() const = 0;

        virtual void Bind(uint32_t slot = 0) const = 0;
    };

    class Texture2D : public Texture
    {
    public:
        static Ref<Texture2D> Create(const std::string& path = "../Haketon/assets/textures/T_UVChecker_512px.png");
    };
}