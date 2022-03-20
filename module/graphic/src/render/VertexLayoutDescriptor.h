#ifndef BONGOMOUNTAIN_VERTEXLAYOUTDESCRIPTOR_H
#define BONGOMOUNTAIN_VERTEXLAYOUTDESCRIPTOR_H

#include <array>
#include <vector>
#include <stdexcept>
#include <string_view>

namespace render
{
    enum class ShaderDataType { None = 0, Float, Float2, Float3, Float4, Mat3, Mat4, Int, Int2, Int3, Int4, Bool };

    constexpr uint32_t shaderDataTypeSize(ShaderDataType type)
    {
        switch(type)
        {
        case ShaderDataType::Float:
            return 4;
        case ShaderDataType::Float2:
            return 4 * 2;
        case ShaderDataType::Float3:
            return 4 * 3;
        case ShaderDataType::Float4:
            return 4 * 4;
        case ShaderDataType::Mat3:
            return 4 * 3 * 3;
        case ShaderDataType::Mat4:
            return 4 * 4 * 4;
        case ShaderDataType::Int:
            return 4;
        case ShaderDataType::Int2:
            return 4 * 2;
        case ShaderDataType::Int3:
            return 4 * 3;
        case ShaderDataType::Int4:
            return 4 * 4;
        case ShaderDataType::Bool:
            return 1;
        }
        throw std::range_error{ "Unknown ShaderDataType!" };
    }
    constexpr uint32_t shaderComponentCount(ShaderDataType type)
    {
        switch(type)
        {
        case ShaderDataType::Float:
            return 1;
        case ShaderDataType::Float2:
            return 2;
        case ShaderDataType::Float3:
            return 3;
        case ShaderDataType::Float4:
            return 4;
        case ShaderDataType::Mat3:
            return 3 * 3;// 3* float3
        case ShaderDataType::Mat4:
            return 4 * 4;// 4* float4
        case ShaderDataType::Int:
            return 1;
        case ShaderDataType::Int2:
            return 2;
        case ShaderDataType::Int3:
            return 3;
        case ShaderDataType::Int4:
            return 4;
        case ShaderDataType::Bool:
            return 1;
        default:
            throw std::range_error{ "Unknown ShaderDataType!" };
        }
    }

    struct AttributeLayout
    {
        std::string_view name;
        ShaderDataType   type;
        uint32_t         size;
        size_t           offset;
        bool             normalized;
        constexpr AttributeLayout(std::string_view aName, ShaderDataType aType, bool aNormalized = false)
            : name(aName)
            , type(aType)
            , size(shaderDataTypeSize(aType))
            , offset(0)
            , normalized(aNormalized)
        {}
    };

    struct VertexLayoutDescriptor : std::vector<AttributeLayout>
    {
        inline explicit VertexLayoutDescriptor(std::vector<AttributeLayout> aElements)
            : std::vector<AttributeLayout>{ std::move(aElements) }
            , stride{ 0 }
        {
            for(AttributeLayout &e : *this)
            {
                e.offset = stride;
                stride += e.size;
            }
        }

        std::size_t stride;
    };

    template<std::size_t n>
    struct LiteralVertexLayoutDescriptor : std::array<AttributeLayout, n>
    {
        constexpr explicit LiteralVertexLayoutDescriptor(std::array<AttributeLayout, n> aElements)
            : std::array<AttributeLayout, n>{ aElements }
            , stride{ 0 }
        {
            for(AttributeLayout &e : *this)
            {
                e.offset = stride;
                stride += e.size;
            }
        }

        std::size_t stride;
    };

    template<typename... EntriesT>
    consteval LiteralVertexLayoutDescriptor<sizeof...(EntriesT)> make_layout(EntriesT... entries)
    {
        return LiteralVertexLayoutDescriptor<sizeof...(EntriesT)>{ { entries... } };
    }
}// namespace render

#endif// BONGOMOUNTAIN_VERTEXLAYOUTDESCRIPTOR_H
