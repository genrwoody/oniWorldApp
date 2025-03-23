#pragma once

#include <algorithm>
#include <map>
#include <queue>
#include <stack>
#include <set>

#include "Diagram.hpp"

class Voronoi
{
    struct HalfedgeGreater {
        bool operator()(const Halfedge *lhs, const Halfedge *rhs) const
        {
            return (*lhs) < (*rhs);
        }
    };

private:
    std::deque<Edge> edgePool;
    std::deque<Vector2f> vertexPool;
    std::deque<Halfedge> halfedgePool;
    std::set<Halfedge *, HalfedgeGreater> halfedgePriorityQueue;
    std::vector<Halfedge *> edgeList;

    std::vector<Site *> m_sites;
    std::vector<std::vector<Edge *>> sitesEdges;

    Rect m_plotBounds;
    Rect m_sitesBounds;

    Halfedge *leftEnd = nullptr;
    Halfedge *rightEnd = nullptr;
    Site *fortunesAlgorithm_bottomMostSite = nullptr;

public:
    Voronoi() = default;

    void Create(std::vector<Site> &points, const Rect &plotBounds);

    void BuildRegion(Site &site);
    void FindNeighborSites(Site &site);

private:
    Rect GetSitesBounds();

    std::deque<Edge *> ReorderEdges(int index, std::deque<Side> &sides);

    Edge *CreateBisectingEdge(Site &site0, Site &site1);

    Halfedge *EdgeListGetHash(int num);
    Halfedge *EdgeListLeftNeighbor(Site &site);
    void ConnectHalfedges(Halfedge *left, Halfedge *right);
    void DisconnectHalfedges(Halfedge *node);

    Vector2f *IntersectHalfedges(Halfedge *first, Halfedge *second);

    void FortunesAlgorithm();

    Site *FortunesAlgorithm_leftRegion(Halfedge &he);
    Site *FortunesAlgorithm_rightRegion(Halfedge &he);

    static int CompareByYThenX(const Site &s1, const Vector2f &s2);
};
