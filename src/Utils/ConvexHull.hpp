#pragma once

#include <ranges>

#include "ConvexHull/Algorithm.hpp"
#include "ConvexHull/Algorithm2D.hpp"
#include "Diagram.hpp"

struct ConvexFace {
    /// <summary>
    /// Adjacency. Array of length "dimension".
    /// If F = Adjacency[i] then the vertices shared with F are Vertices[j]
    /// where j != i. In the context of triangulation, can be null (indicates
    /// the cell is at boundary).
    /// </summary>
    /// <value>The adjacency.</value>
    ConvexFace *Adjacency[3] = {0};

    /// <summary>
    /// The vertices stored in clockwise order for dimensions 2 - 4, in higher
    /// dimensions the order is arbitrary. Unless I accidentally switch some
    /// index somewhere in which case the order is CCW. Either way, it is
    /// consistent. 3D Normal = (V[1] - V[0]) x (V[2] - V[1]).
    /// </summary>
    /// <value>The vertices.</value>
    Site *Vertices[3] = {0};

    /// <summary>
    /// The normal vector of the face. Null if used in triangulation.
    /// </summary>
    /// <value>The normal.</value>
    double *Normal = nullptr;

    std::optional<Vector2f> dualPoint;
    Vector2f GetDualPoint();

    std::optional<Vector2f> circumcenter;
    Vector2f GetCircumcenter();
};

class ConvexHull
{
private:
    ConvexHullAlgorithm chAlgo;
    std::vector<double> Positions;

public:
    std::vector<Site *> Points;
    std::vector<ConvexFace> Faces;
    std::vector<ConvexFace *> Faces2D;

public:
    ConvexHull() = default;

    void Create2D(std::vector<Vector2f> &vertices);

    void Create(std::vector<Site> &vertices);

    void CreateDelaunay(std::vector<Site> &vertices);

private:
    void GetHullVertices(std::vector<Site> &vertices);

    /// <summary>
    /// Finds the convex hull and creates the TFace objects.
    /// </summary>
    /// <typeparam name="TFace">The type of the t face.</typeparam>
    /// <typeparam name="TVertex">The type of the t vertex.</typeparam>
    /// <returns>TFace[].</returns>
    template<typename TVertex>
    void GetConvexFaces(std::vector<TVertex> &vertices, bool lift, int dim);
    void Return2DResultInOrder(std::vector<Vector2f> &data);
};
