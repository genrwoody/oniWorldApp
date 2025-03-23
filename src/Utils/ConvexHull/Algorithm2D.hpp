#pragma once

#include "../Diagram.hpp"

class ConvexHull2DAlgorithm
{
public:
    /// <summary>
    /// For 2D only: Returns the result in counter-clockwise order starting with
    /// the element with the lowest X value. If there are multiple vertices with
    /// the same minimum X, then the one with the lowest Y is chosen.
    /// </summary>
    /// <typeparam name="TVertex">The type of the vertex.</typeparam>
    /// <param name="points">The points.</param>
    /// <param name="tolerance">The tolerance.</param>
    /// <returns>List&lt;TVertex&gt;.</returns>
    /// <exception cref="ArgumentException">Cannot define the 2D convex hull for
    /// less than two points.</exception>
    static std::vector<Vector2d *> Create(std::vector<Vector2d> &points,
                                          double tolerance = 1e-10);

private:
    static std::vector<Vector2d *> FindIntermediatePointsForLongSkinny(
        std::vector<Vector2d> &points, int numPoints, int usedIndex1,
        int usedIndex2, std::vector<int> &newUsedIndices);

    // this function adds the new point to the sorted array. The reason it is
    // complicated is that if it errors - it is because there are two points at
    // the same distance along. So, we then check if the new point or the
    // existing one on the list should stay. Simply keep the one that is
    // furthest from the edge vector.
    static bool AddToListAlong(std::vector<Vector2d *> &sortedPoints,
                               std::vector<double> &sortedKeys, int &size,
                               Vector2d &newPoint, double newPointX,
                               double newPointY, double basePointX,
                               double basePointY, double edgeVectorX,
                               double edgeVectorY, double tolerance);

    // This binary search is modified/simplified from Array.BinarySearch
    // (https://referencesource.microsoft.com/mscorlib/a.html#b92d187c91d4c9a9)
    static int BinarySearch(double *data, int length, double value);
};
