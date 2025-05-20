#pragma once

#include <cstdint>
#include <vector>
#include <optional>

#include "Vector2f.hpp"

struct Rect {
    float x{};
    float y{};
    float width{};
    float height{};

    Rect() = default;

    Rect(float x_, float y_, float width_, float height_)
        : x(x_)
        , y(y_)
        , width(width_)
        , height(height_)
    {
    }

    Vector2f MinCorner() const { return {x, y}; }
    Vector2f MaxCorner() const { return {x + width, y + height}; }
    Vector2f Center() const { return {x + width / 2.0f, y + height / 2.0f}; }

    bool Contains(const Vector2f &point) const
    {
        return point.x >= x && point.x < x + width && point.y >= y &&
               point.y < y + height;
    }

    bool Overlaps(const Rect &other) const
    {
        return other.x < x + width && x < other.x + other.width &&
               other.y < y + height && y < other.y + other.height;
    }

    Rect Inflate(float dx, float dy) const
    {
        return {x - dx, y - dy, width + dx * 2.0f, height + dy * 2.0f};
    }
};

class Polygon
{
public:
    std::vector<Vector2f> Vertices;

private:
    mutable std::optional<float> m_area;
    mutable std::optional<Vector2f> m_centroid;
    mutable std::optional<Rect> m_bounds;

public:
    Polygon() = default;
    explicit Polygon(const Rect &rect)
        : Vertices{
              {rect.x, rect.y},
              {rect.x + rect.width, rect.y},
              {rect.x + rect.width, rect.y + rect.height},
              {rect.x, rect.y + rect.height},
          }
    {
    }

    void Clear()
    {
        Vertices.clear();
        m_area.reset();
        m_centroid.reset();
        m_bounds.reset();
    }
    float Area() const;
    const Vector2f &Centroid() const;
    const Rect &Bounds() const;
    bool Contains(const Vector2f &point) const;
    bool Contains(float x, float y) const
    {
        Vector2f point{x, y};
        return Contains(point);
    }
    void Intersect(const Polygon &clip);
    bool SimpleSharesEdge(const Polygon &rhs) const;
};
