#pragma once

#include "Mesh.hpp"
#include "Traits.hpp"
#include "Util.hpp"
#include "Vec2.hpp"
#include "define.hpp"

NS_BEGIN

template<typename T> requires std::is_arithmetic_v<T>
struct line
{
    template<typename C>
    inline operator line<C>() const
    {
        return line<C>{(vec2d<C>) A, (vec2d<C>) B};
    }

    vec2d<T> A, B;
};

using fline = line<float>;
using iline = line<int>;
using uline = line<unsigned int>;
using dline = line<double>;

struct GLLine: public line<float>
{
    inline GLLine()
    {
        A = {0.0, 0.0};
        B = {0.0, 0.0};
    }

    template<typename C>
    explicit inline GLLine(const line<C> &other)
        : line(other)
    {
        mesh.setAttributeDescriptors({
                                         AttributeType::Vec2 // position
                                     });
        mesh.setRenderMode(RenderMode::Lines);
        mesh.insertVertices({{A.x, A.y}, {B.x, B.y}});
        mesh.insertIndices({0, 1});
        if (!mesh.upload()) {
            log::error("Failed uploading GLLine mesh");
        }
    }

    explicit inline GLLine(fvec2d A, fvec2d B)
        : GLLine(fline{A, B})
    {}

    Mesh mesh;
};

template<typename T>
struct IsShape<line<T>>
{
    static const bool enable = true;
    static const ShapeType type = ShapeType::Line;
};

NS_END
