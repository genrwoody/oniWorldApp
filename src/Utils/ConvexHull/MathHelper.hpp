#pragma once

#include <vector>

#include "Constants.hpp"

struct ConvexFaceInternal;

class MathHelper
{
private:
    /// <summary>
    /// The dimension
    /// </summary>
    size_t Dimension = 0;

    /// <summary>
    /// The position data
    /// </summary>
    double *PositionData = nullptr;

public:
    MathHelper() = default;

    /// <summary>
    /// Initializes a new instance of the <see cref="MathHelper"/> class.
    /// </summary>
    /// <param name="dimension">The dimension.</param>
    /// <param name="positions">The positions.</param>
    void Initialize(int dimension, double *positions)
    {
        Dimension = dimension;
        PositionData = positions;
    }

    /// <summary>
    /// Calculates the normal and offset of the hyper-plane given by the face's
    /// vertices.
    /// </summary>
    /// <param name="face">The face.</param>
    /// <param name="center">The center.</param>
    /// <returns><c>true</c> if XXXX, <c>false</c> otherwise.</returns>
    bool CalculateFacePlane(ConvexFaceInternal &face,
                            const double *center) const;

    /// <summary>
    /// Check if the vertex is "visible" from the face.
    /// The vertex is "over face" if the return value is &gt;
    /// Constants.PlaneDistanceTolerance.
    /// </summary>
    /// <param name="v">The v.</param>
    /// <param name="f">The f.</param>
    /// <returns>The vertex is "over face" if the result is positive.</returns>
    double GetVertexDistance(int v, const ConvexFaceInternal &f) const;

    /// <summary>
    /// Returns the vector the between vertices.
    /// </summary>
    /// <param name="fromIndex">From index.</param>
    /// <param name="toIndex">To index.</param>
    /// <param name="target">The target.</param>
    /// <returns></returns>
    void VectorBetweenVertices(int toIndex, int fromIndex,
                               double *target) const;

    void RandomOffsetToLift(int index, double maxHeight);

    /// <summary>
    /// Finds normal vector of a hyper-plane given by vertices.
    /// Stores the results to normalData.
    /// </summary>
    /// <param name="vertices">The vertices.</param>
    /// <param name="normalData">The normal data.</param>
    void FindNormalVector(const int *vertices, double *normalData) const
    {
        switch (Dimension) {
        case 2:
            FindNormalVector2D(vertices, normalData);
            break;
        case 3:
            FindNormalVector3D(vertices, normalData);
            break;
        case 4:
            FindNormalVector4D(vertices, normalData);
            break;
        default:
            FindNormalVectorND(vertices, normalData);
            break;
        }
    }

    /// <summary>
    /// Finds 2D normal vector.
    /// </summary>
    /// <param name="vertices">The vertices.</param>
    /// <param name="normal">The normal.</param>
    void FindNormalVector2D(const int *vertices, double *normal) const;

    /// <summary>
    /// Finds 3D normal vector.
    /// </summary>
    /// <param name="vertices">The vertices.</param>
    /// <param name="normal">The normal.</param>
    void FindNormalVector3D(const int *vertices, double *normal) const;

    /// <summary>
    /// Finds 4D normal vector.
    /// </summary>
    /// <param name="vertices">The vertices.</param>
    /// <param name="normal">The normal.</param>
    void FindNormalVector4D(const int *vertices, double *normal) const;

    /// <summary>
    /// Finds the normal vector nd.
    /// </summary>
    /// <param name="vertices">The vertices.</param>
    /// <param name="normal">The normal.</param>
    void FindNormalVectorND(const int *vertices, double *normal) const;

    /// <summary>
    /// Determinants the volume of the simplex.
    /// </summary>
    /// <param name="A">a.</param>
    /// <returns>System.Double.</returns>
    double VolumeOfSimplex(const int *vertexIndices) const;

    /// <summary>
    /// Gets the simplex volume. Prior to having enough edge vectors, the method
    /// pads the remaining with all "other numbers". So, yes, this method is not
    /// really finding the volume. But a relative volume-like measure. It uses
    /// the magnitude of the determinant as the volume stand-in following the
    /// Cayley-Menger theorem.
    /// </summary>
    /// <param name="edgeVectors">The edge vectors.</param>
    /// <param name="lastIndex">The last index.</param>
    /// <param name="bigNumber">The big number.</param>
    /// <returns>System.Double.</returns>
    double GetSimplexVolume(std::array<double, MaxDimension> *edgeVectors,
                            int lastIndex, double bigNumber);

    /// <summary>
    /// Determinants the destructive.
    /// </summary>
    /// <param name="A">a.</param>
    /// <returns>System.Double.</returns>
    double DeterminantDestructive(double *A);

    // Modified from Math.NET
    // Copyright (c) 2009-2013 Math.NET
    /// <summary>
    /// Lus the factor.
    /// </summary>
    /// <param name="data">The data.</param>
    /// <param name="order">The order.</param>
    /// <param name="ipiv">The ipiv.</param>
    /// <param name="vecLUcolj">The vec l ucolj.</param>
    static void LUFactor(double *data, int order, int *ipiv, double *vecLUcolj);
};
