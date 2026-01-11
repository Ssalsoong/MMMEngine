#pragma once
#include <cstdint>
#include <type_traits>

namespace MMMEngine
{
    enum class AssetType : uint32_t
    {
        Unknown = 0,
        Texture2D = 1,
        Mesh = 2,
        Material = 3,
        Shader = 4,
        Audio = 5,
        AnimationClip = 6,
    };

    template<typename T>
    struct AssetTypeOf
    {
        static constexpr AssetType Value = AssetType::Unknown;
    };
}