#include "Halfedge.hpp"

#include <deque>

void Edge::ClipVertices(const Rect &bounds)
{
    float xMin = bounds.x;
    float yMin = bounds.y;
    float xMax = bounds.x + bounds.width;
    float yMax = bounds.y + bounds.height;
    Vector2f *vertex;
    Vector2f *vertex2;
    if (a == 1.0f && b >= 0.0f) {
        vertex = rightVertex;
        vertex2 = leftVertex;
    } else {
        vertex = leftVertex;
        vertex2 = rightVertex;
    }
    float num;
    float num2;
    float num3;
    float num4;
    if (a == 1.0f) {
        num = yMin;
        if (vertex != nullptr && vertex->y > yMin) {
            num = vertex->y;
        }
        if (num > yMax) {
            return;
        }
        num2 = c - (double)b * num;
        num3 = yMax;
        if (vertex2 != nullptr && vertex2->y < yMax) {
            num3 = vertex2->y;
        }
        if (num3 < yMin) {
            return;
        }
        num4 = c - (double)b * num3;
        if ((num2 > xMax && num4 > xMax) || (num2 < xMin && num4 < xMin)) {
            return;
        }
        if (num2 > xMax) {
            num2 = xMax;
            num = ((double)c - num2) / b;
        } else if (num2 < xMin) {
            num2 = xMin;
            num = ((double)c - num2) / b;
        }
        if (num4 > xMax) {
            num4 = xMax;
            num3 = ((double)c - num4) / b;
        } else if (num4 < xMin) {
            num4 = xMin;
            num3 = ((double)c - num4) / b;
        }
    } else {
        num2 = xMin;
        if (vertex != nullptr && vertex->x > xMin) {
            num2 = vertex->x;
        }
        if (num2 > xMax) {
            return;
        }
        num = c - (double)a * num2;
        num4 = xMax;
        if (vertex2 != nullptr && vertex2->x < xMax) {
            num4 = vertex2->x;
        }
        if (num4 < xMin) {
            return;
        }
        num3 = c - (double)a * num4;
        if ((num > yMax && num3 > yMax) || (num < yMin && num3 < yMin)) {
            return;
        }
        if (num > yMax) {
            num = yMax;
            num2 = ((double)c - num) / a;
        } else if (num < yMin) {
            num = yMin;
            num2 = ((double)c - num) / a;
        }
        if (num3 > yMax) {
            num3 = yMax;
            num4 = ((double)c - num3) / a;
        } else if (num3 < yMin) {
            num3 = yMin;
            num4 = ((double)c - num3) / a;
        }
    }
    visible = true;
    if (vertex == leftVertex) {
        clippedEnds[(int)Side::LEFT] = Vector2f(num2, num);
        clippedEnds[(int)Side::RIGHT] = Vector2f(num4, num3);
    } else {
        clippedEnds[(int)Side::RIGHT] = Vector2f(num2, num);
        clippedEnds[(int)Side::LEFT] = Vector2f(num4, num3);
    }
}

bool Halfedge::IsLeftOf(const Site &site) const
{
    auto coord = edge->rightSite;
    bool flag = site.x > coord->x;
    if (flag && leftRight == Side::LEFT) {
        return true;
    }
    if (!flag && leftRight == Side::RIGHT) {
        return false;
    }
    bool flag3;
    if (edge->a == 1.0f) {
        float num = site.y - coord->y;
        float num2 = site.x - coord->x;
        bool flag2 = false;
        if ((!flag && edge->b < 0.0f) || (flag && edge->b >= 0.0f)) {
            flag3 = num >= edge->b * num2;
            flag2 = flag3;
        } else {
            flag3 = site.x + site.y * edge->b > edge->c;
            if (edge->b < 0.0f) {
                flag3 = !flag3;
            }
            if (!flag3) {
                flag2 = true;
            }
        }
        if (!flag2) {
            float num3 = coord->x - edge->leftSite->x;
            float num4 = edge->b * (num2 * num2 - num * num);
            float num5 = 1.0f + 2.0f * num2 / num3 + (edge->b * edge->b);
            flag3 = num4 < num3 * num * num5;
            if (edge->b < 0.0f) {
                flag3 = !flag3;
            }
        }
    } else {
        float num4 = edge->c - edge->a * site.x;
        float num5 = site.y - num4;
        float num6 = site.x - coord->x;
        float num7 = num4 - coord->y;
        flag3 = num5 * num5 > num6 * num6 + num7 * num7;
    }
    if (leftRight != Side::LEFT) {
        return !flag3;
    }
    return flag3;
}

bool Halfedge::Intersect(const Halfedge &halfedge1, Vector2f &result) const
{
    auto &halfedge0 = *this;

    auto edge = halfedge0.edge;
    auto edge2 = halfedge1.edge;
    if (edge == nullptr || edge2 == nullptr) {
        return false;
    }
    if (edge->rightSite == edge2->rightSite) {
        return false;
    }
    float num = (double)edge->a * edge2->b - (double)edge->b * edge2->a;
    if (-1E-10f < num && num < 1E-10f) {
        return false;
    }
    float x = ((double)edge->c * edge2->b - (double)edge2->c * edge->b) / num;
    float y = ((double)edge2->c * edge->a - (double)edge->c * edge2->a) / num;
    auto halfedge2 = &halfedge1;
    auto edge3 = edge2;
    if ((*edge->rightSite) < (*edge2->rightSite)) {
        halfedge2 = &halfedge0;
        edge3 = edge;
    }
    bool flag = x >= edge3->rightSite->x;
    if ((flag && halfedge2->leftRight == Side::LEFT) ||
        (!flag && halfedge2->leftRight == Side::RIGHT)) {
        return false;
    }
    result.x = x;
    result.y = y;
    return true;
}
