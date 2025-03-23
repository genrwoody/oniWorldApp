#include "MathHelper.hpp"

#include <cmath>

#include "DataModel.hpp"

bool MathHelper::CalculateFacePlane(ConvexFaceInternal &face,
                                    const double *center) const
{
    auto &vertices = face.Vertices;
    auto &normal = face.Normal;
    FindNormalVector(vertices.data(), normal.data());

    if (std::isnan(normal[0])) {
        return false;
    }

    double tmpCenter[MaxDimension] = {0};
    if (center == nullptr)
        center = tmpCenter;

    double offset = 0.0;
    double centerDistance = 0.0;
    auto fi = Dimension * vertices[0];
    for (auto i = 0; i < Dimension; i++) {
        auto n = normal[i];
        offset += n * PositionData[fi + i];
        centerDistance += n * center[i];
    }
    face.Offset = -offset;
    centerDistance -= offset;

    if (centerDistance > 0) {
        for (auto i = 0; i < Dimension; i++)
            normal[i] = -normal[i];
        face.Offset = offset;
        face.IsNormalFlipped = true;
    } else
        face.IsNormalFlipped = false;

    return true;
}

double MathHelper::GetVertexDistance(int v, const ConvexFaceInternal &f) const
{
    auto normal = f.Normal;
    auto x = v * Dimension;
    auto distance = f.Offset;
    for (auto i = 0; i < Dimension; i++)
        distance += normal[i] * PositionData[x + i];
    return distance;
}

void MathHelper::VectorBetweenVertices(int toIndex, int fromIndex,
                                       double *target) const
{
    int u = toIndex * Dimension, v = fromIndex * Dimension;
    for (auto i = 0; i < Dimension; i++) {
        target[i] = PositionData[u + i] - PositionData[v + i];
    }
}

void MathHelper::RandomOffsetToLift(int index, double maxHeight)
{
    int liftIndex = (index * Dimension) + Dimension - 1;
    PositionData[liftIndex] += 0.0001 * maxHeight * 0.2;
}

void MathHelper::FindNormalVector2D(const int *vertices, double *normal) const
{
    double ntX[MaxDimension];

    VectorBetweenVertices(vertices[1], vertices[0], ntX);

    auto nx = -ntX[1];
    auto ny = ntX[0];

    auto norm = std::sqrt(nx * nx + ny * ny);

    auto f = 1.0 / norm;
    normal[0] = f * nx;
    normal[1] = f * ny;
}

void MathHelper::FindNormalVector3D(const int *vertices, double *normal) const
{
    double ntX[MaxDimension];
    double ntY[MaxDimension];

    VectorBetweenVertices(vertices[1], vertices[0], ntX);
    VectorBetweenVertices(vertices[2], vertices[1], ntY);

    auto nx = ntX[1] * ntY[2] - ntX[2] * ntY[1];
    auto ny = ntX[2] * ntY[0] - ntX[0] * ntY[2];
    auto nz = ntX[0] * ntY[1] - ntX[1] * ntY[0];

    auto norm = std::sqrt(nx * nx + ny * ny + nz * nz);

    auto f = 1.0 / norm;
    normal[0] = f * nx;
    normal[1] = f * ny;
    normal[2] = f * nz;
}

void MathHelper::FindNormalVector4D(const int *vertices, double *normal) const
{
    double ntX[MaxDimension];
    double ntY[MaxDimension];
    double ntZ[MaxDimension];

    VectorBetweenVertices(vertices[1], vertices[0], ntX);
    VectorBetweenVertices(vertices[2], vertices[1], ntY);
    VectorBetweenVertices(vertices[3], vertices[2], ntZ);

    auto x = ntX;
    auto y = ntY;
    auto z = ntZ;

    // This was generated using Mathematica
    auto nx = x[3] * (y[2] * z[1] - y[1] * z[2]) +
              x[2] * (y[1] * z[3] - y[3] * z[1]) +
              x[1] * (y[3] * z[2] - y[2] * z[3]);
    auto ny = x[3] * (y[0] * z[2] - y[2] * z[0]) +
              x[2] * (y[3] * z[0] - y[0] * z[3]) +
              x[0] * (y[2] * z[3] - y[3] * z[2]);
    auto nz = x[3] * (y[1] * z[0] - y[0] * z[1]) +
              x[1] * (y[0] * z[3] - y[3] * z[0]) +
              x[0] * (y[3] * z[1] - y[1] * z[3]);
    auto nw = x[2] * (y[0] * z[1] - y[1] * z[0]) +
              x[1] * (y[2] * z[0] - y[0] * z[2]) +
              x[0] * (y[1] * z[2] - y[2] * z[1]);

    auto norm = std::sqrt(nx * nx + ny * ny + nz * nz + nw * nw);

    auto f = 1.0 / norm;
    normal[0] = f * nx;
    normal[1] = f * ny;
    normal[2] = f * nz;
    normal[3] = f * nw;
}

void MathHelper::FindNormalVectorND(const int *vertices, double *normal) const
{
    /* We need to solve the matrix A n = B where
     *  - A contains coordinates of vertices as columns
     *  - B is vector with all 1's. Really, it should be the distance of
     *      the plane from the origin, but - since we're not worried about
     * that here and we will normalize the normal anyway - all 1's suffices.
     */
    int iPiv[MaxDimension] = {0};
    double data[MaxDimension * MaxDimension] = {0};
    double nDNormalHelperVector[MaxDimension] = {0};
    double norm = 0.0;

    // Solve determinants by replacing x-th column by all 1.
    for (auto x = 0; x < Dimension; x++) {
        for (auto i = 0; i < Dimension; i++) {
            auto offset = vertices[i] * Dimension;
            for (auto j = 0; j < Dimension; j++) {
                // maybe I got the i/j mixed up here regarding the
                // representation Math.net uses...
                // ...but it does not matter since Det(A) =
                // Det(Transpose(A)).
                data[Dimension * i + j] =
                    j == x ? 1.0 : PositionData[offset + j];
            }
        }
        LUFactor(data, (int)Dimension, iPiv, nDNormalHelperVector);
        auto coord = 1.0;
        for (auto i = 0; i < Dimension; i++) {
            // the determinant sign changes on row swap.
            if (iPiv[i] != i)
                coord *= -data[Dimension * i + i];
            else
                coord *= data[Dimension * i + i];
        }
        normal[x] = coord;
        norm += coord * coord;
    }

    // Normalize the result
    auto f = 1.0 / std::sqrt(norm);
    for (auto i = 0; i < Dimension; i++)
        normal[i] *= f;
}

/// <summary>
/// Gets the simplex volume. Prior to having enough edge vectors, the method
/// pads the remaining with all "other numbers". So, yes, this method is not
/// really finding the volume. But a relative volume-like measure. It uses the
/// magnitude of the determinant as the volume stand-in following the
/// Cayley-Menger theorem.
/// </summary>
/// <param name="edgeVectors">The edge vectors.</param>
/// <param name="lastIndex">The last index.</param>
/// <param name="bigNumber">The big number.</param>
/// <returns>System.Double.</returns>
double
MathHelper::GetSimplexVolume(std::array<double, MaxDimension> *edgeVectors,
                             int lastIndex, double bigNumber)
{
    double A[MaxDimension * MaxDimension];
    int index = 0;
    for (int i = 0; i < Dimension; i++)
        for (int j = 0; j < Dimension; j++)
            if (i <= lastIndex)
                A[index++] = edgeVectors[i][j];
            else {
                A[index] = (std::pow(-1, index) * index) / bigNumber;
                index++;
            }
    // this last term is used for all the vertices in the comparison for the yet
    // determined vertices the idea is to come up with sets of numbers that are
    // orthogonal so that an non-zero value will result and to choose smallish
    // numbers since the choice of vectors will affect what the end volume is.
    // A better way (todo?) is to solve a smaller matrix. However, cases were
    // found in which the obvious smaller vector (the upper left) had too many
    // zeros. So, one would need to find the right subset. Indeed choosing a
    // subset biases the first dimensions of the others. Perhaps a larger volume
    // would be created from a different vertex if another subset of dimensions
    // were used.
    return std::abs(DeterminantDestructive(A));
}

/// <summary>
/// Determinants the destructive.
/// </summary>
/// <param name="A">a.</param>
/// <returns>System.Double.</returns>
double MathHelper::DeterminantDestructive(double *A)
{
    switch (Dimension) {
    case 0:
        return 0.0;
    case 1:
        return A[0];
    case 2:
        return A[0] * A[3] - A[1] * A[2];
    case 3:
        return A[0] * A[4] * A[8] + A[1] * A[5] * A[6] + A[2] * A[3] * A[7] -
               A[0] * A[5] * A[7] - A[1] * A[3] * A[8] - A[2] * A[4] * A[6];
    default: {
        int iPiv[MaxDimension];
        double helper[MaxDimension];
        LUFactor(A, Dimension, iPiv, helper);
        double det = 1.0;
        for (int i = 0; i < Dimension; i++) {
            det *= A[Dimension * i + i];
            if (iPiv[i] != i)
                det *= -1; // the determinant sign changes on row swap.
        }
        return det;
    }
    }
}

double MathHelper::VolumeOfSimplex(const int *vertexIndices) const
{
    // DebugPrintVertices(vertexIndices);
    //  this is the Cayley-Menger determinant, so a matrix is defined that
    //  is numDimensions+2
    int numRowCol = Dimension + 2;
    std::vector<double> A(numRowCol * numRowCol);
    for (int i = 1; i < numRowCol; i++) {
        A[i] = 1;
        A[i * numRowCol] = 1;
    }
    for (int i = 0; i <= Dimension; i++)
        for (int j = i + 1; j <= Dimension; j++) {
            double d[MaxDimension];
            VectorBetweenVertices(vertexIndices[i], vertexIndices[j], d);
            auto distanceSquared = 0.0;
            for (int k = 0; k < Dimension; k++)
                distanceSquared += d[k] * d[k];
            A[(i + 1) + (j + 1) * numRowCol] = distanceSquared;
            A[(j + 1) + (i + 1) * numRowCol] = distanceSquared;
        }
    int iPiv[MaxDimension + 2];
    double helper[MaxDimension + 2];
    // determinant(A, 2 + Dimension);  //, iPiv, helper);
    LUFactor(A.data(), (int)numRowCol, iPiv, helper);
    auto det = 1.0;
    for (int i = 0; i < numRowCol; i++) {
        det *= A[numRowCol * i + i];
        if (iPiv[i] != i)
            det *= -1; // the determinant sign changes on row swap.
    }
    int denom = (int)std::pow(2, Dimension);
    int m = 1;
    while (m++ < Dimension)
        denom *= m;
    det /= denom;
    if (Dimension % 2 == 0)
        det *= -1;
    return det;
}

void MathHelper::LUFactor(double *data, int order, int *ipiv, double *vecLUcolj)
{
    // Initialize the pivot matrix to the identity permutation.
    for (auto i = 0; i < order; i++) {
        ipiv[i] = i;
    }

    // Outer loop.
    for (auto j = 0; j < order; j++) {
        auto indexj = j * order;
        auto indexjj = indexj + j;

        // Make a copy of the j-th column to localize references.
        for (auto i = 0; i < order; i++) {
            vecLUcolj[i] = data[indexj + i];
        }

        // Apply previous transformations.
        for (auto i = 0; i < order; i++) {
            // Most of the time is spent in the following dot product.
            auto kmax = std::min(i, j);
            auto s = 0.0;
            for (auto k = 0; k < kmax; k++) {
                s += data[k * order + i] * vecLUcolj[k];
            }

            data[indexj + i] = vecLUcolj[i] -= s;
        }

        // Find pivot and exchange if necessary.
        auto p = j;
        for (auto i = j + 1; i < order; i++) {
            if (std::abs(vecLUcolj[i]) > std::abs(vecLUcolj[p])) {
                p = i;
            }
        }

        if (p != j) {
            for (auto k = 0; k < order; k++) {
                auto indexk = k * order;
                auto indexkp = indexk + p;
                auto indexkj = indexk + j;
                auto temp = data[indexkp];
                data[indexkp] = data[indexkj];
                data[indexkj] = temp;
            }

            ipiv[j] = p;
        }

        // Compute multipliers.
        if (j < order && data[indexjj] != 0.0) {
            for (auto i = j + 1; i < order; i++) {
                data[indexj + i] /= data[indexjj];
            }
        }
    }
}
