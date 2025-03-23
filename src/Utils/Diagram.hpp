#pragma once

#include <string>

#include "Polygon.hpp"
#include "Halfedge.hpp"

class Diagram
{
private:
    float m_weightSum = 0.0f;
    Polygon &m_bounds;
    std::vector<Site> &m_sites;

public:
    Diagram(Polygon &bounds, std::vector<Site> &sites);

    bool ComputeNode();
    bool ComputeNodePD();

private:
    bool MakeVD(const Rect &bounds);
    bool ComputeVD();
    bool ComputePD();
    void UpdateWeights();
    bool ComputePowerDiagram();
};
