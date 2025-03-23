#include "Polygon.hpp"

#include <limits>
#include <sstream>

#include <clipper.hpp>

#include "Setting/SettingsCache.hpp"

const Rect &Polygon::Bounds() const
{
    if (!m_bounds.has_value()) {
        Vector2f min, max;
        min.x = min.y = std::numeric_limits<float>::max();
        max.x = max.y = std::numeric_limits<float>::lowest();
        for (auto &vertex : Vertices) {
            if (vertex.x > max.x) {
                max.x = vertex.x;
            }
            if (vertex.y > max.y) {
                max.y = vertex.y;
            }
            if (vertex.x < min.x) {
                min.x = vertex.x;
            }
            if (vertex.y < min.y) {
                min.y = vertex.y;
            }
        }
        m_bounds.emplace(min.x, min.y, max.x - min.x, max.y - min.y);
    }
    return m_bounds.value();
}

float Polygon::Area() const
{
    if (!m_area.has_value()) {
        float area = 0.0f;
        size_t j = 1;
        for (size_t i = 0; i < Vertices.size(); j++, i++) {
            if (j == Vertices.size())
                j = 0;
            Vector2d vec1 = Vertices[i];
            Vector2d vec2 = Vertices[j];
            area += vec1.x * vec2.y - vec2.x * vec1.y;
        }
        area *= 0.5;
        m_area.emplace(area);
    }
    return m_area.value();
}

const Vector2f &Polygon::Centroid() const
{
    if (!m_centroid.has_value()) {
        Vector2f value;
        float area = 0.0f;
        size_t j = 1;
        for (size_t i = 0; i < Vertices.size(); j++, i++) {
            if (j == Vertices.size())
                j = 0;
            Vector2d vec1 = Vertices[i];
            Vector2d vec2 = Vertices[j];
            double cross = vec1.x * vec2.y - vec2.x * vec1.y;
            value += (vec1 + vec2) * (float)cross;
            area += cross;
        }
        area *= 0.5;
        m_area.emplace(area);
        value /= (float)(6.0 * area);
        m_centroid.emplace(value);
    }
    return m_centroid.value();
}

bool Polygon::Contains(const Vector2f &point) const
{
    if (!Bounds().Contains(point)) {
        return false;
    }
    bool flag = false;
    size_t j = Vertices.size() - 1;
    for (size_t i = 0; i < Vertices.size(); j = i++) {
        auto first = Vertices[j] - Vertices[i];
        auto second = point - Vertices[i];
        auto cross = first.Cross(second);
        if (cross == 0) {
            return true; // at edge;
        } else if (i == 0) {
            flag = cross > 0;
        } else if (flag != cross > 0) {
            return false;
        }
    }
    return true;
}

void Polygon::Intersect(const Polygon &clip)
{
    using namespace ClipperLib;

    Path subjs;
    for (auto &item : this->Vertices) {
        subjs.emplace_back((int)(item.x * 10000.0), (int)(item.y * 10000.0));
    }
    Path clips;
    for (auto &item : clip.Vertices) {
        clips.emplace_back((int)(item.x * 10000.0), (int)(item.y * 10000.0));
    }
    Clipper clipper;
    clipper.AddPath(subjs, PolyType::ptSubject, true);
    clipper.AddPath(clips, PolyType::ptClip, true);
    PolyTree polytree;
    Paths result;
    clipper.Execute(ctIntersection, polytree, PolyFillType::pftEvenOdd);
    ClipperLib::PolyTreeToPaths(polytree, result);
    this->Vertices.clear();
    if (!result.empty()) {
        auto &path = result[0];
        for (auto &item : path) {
            Vector2f point{(float)item.X, (float)item.Y};
            this->Vertices.emplace_back(point * 0.0001f);
        }
    } else {
        LogE("intersection result is empty.");
        std::stringstream sstm;
        sstm << "subj: ";
        for (auto &item : subjs) {
            sstm << item.X << "," << item.Y << ";";
        }
        sstm << "clip: ";
        for (auto &item : clips) {
            sstm << item.X << "," << item.Y << ";";
        }
        LogE("%s\n", sstm.str().c_str());
    }
}

bool Polygon::SimpleSharesEdge(const Polygon &rhs) const
{
    int same = 0;
    for (auto &vex1 : Vertices) {
        for (auto &vex2 : rhs.Vertices) {
            if (vex1.Distance(vex2) < 0.001f) {
                same++;
            }
        }
    }
    return same > 1;
}
