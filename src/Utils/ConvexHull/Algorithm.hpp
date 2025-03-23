#pragma once

#include <array>
#include <vector>
#include <stack>
#include <unordered_set>

#include "Constants.hpp"
#include "DataModel.hpp"
#include "MathHelper.hpp"

class ConvexHullAlgorithm
{
    typedef std::vector<int> IndexBuffer;

public:
    /// <summary>
    /// Corresponds to the dimension of the data.
    /// When the "lifted" hull is computed, Dimension is automatically
    /// incremented by one.
    /// </summary>
    int NumOfDimensions = 0;

    /// <summary>
    /// Are we on a paraboloid?
    /// </summary>
    bool IsLifted = false;

    /// <summary>
    /// Explained in ConvexHullComputationConfig.
    /// </summary>
    double PlaneDistanceTolerance = DefaultPlaneDistanceTolerance;

    /*
     * Representation of the input vertices.
     *
     * - In the algorithm, a vertex is represented by its index in the Vertices
     * array. This makes the algorithm a lot faster (up to 30%) than using
     * object reference everywhere.
     * - Positions are stored as a single array of values. Coordinates for
     * vertex with index i are stored at indices <i * Dimension, (i + 1) *
     * Dimension)
     * - VertexMarks are used by the algorithm to help identify a set of
     * vertices that is "above" (or "beyond") a specific face.
     */
    /// <summary>
    /// The positions
    /// </summary>
    double *Positions = nullptr;
    /// <summary>
    /// The vertex marks
    /// </summary>
    std::vector<bool> VertexVisited;

    int NumberOfVertices = 0;

    /*
     * The triangulation faces are represented in a single pool for objects that
     * are being reused. This allows for represent the faces as integers and
     * significantly speeds up many computations.
     * - AffectedFaceFlags are used to mark affected faces/
     */
    /// <summary>
    /// The face pool
    /// </summary>
    std::vector<ConvexFaceInternal> FacePool;
    /// <summary>
    /// The affected face flags
    /// </summary>
    std::vector<bool> AffectedFaceFlags;
    /// <summary>
    /// The free face indices
    /// </summary>
    std::stack<int> FreeFaceIndices;

    /// <summary>
    /// Used to track the size of the current hull in the Update/RollbackCenter
    /// functions.
    /// </summary>
    int ConvexHullSize = 0;

    /// <summary>
    /// A list of faces that that are not a part of the final convex hull and
    /// still need to be processed.
    /// The last
    /// </summary>
    ConvexFaceInternal *lastUnprocessedFace = nullptr;

    /// <summary>
    /// A list of faces that that are not a part of the final convex hull and
    /// still need to be processed.
    /// Get the first element.
    /// </summary>
    /// <value>The first.</value>
    ConvexFaceInternal *FirstUnprocessedFace = nullptr;

    /// <summary>
    /// A list of faces that form the convex hull.
    /// </summary>
    IndexBuffer ConvexFaces;

    /// <summary>
    /// The vertex that is currently being processed.
    /// </summary>
    int CurrentVertex = 0;

    /// <summary>
    /// A helper variable to determine the furthest vertex for a particular
    /// convex face.
    /// </summary>
    double MaxDistance = 0.0;

    /// <summary>
    /// A helper variable to help determine the index of the vertex that is
    /// furthest from the face that is currently being processed.
    /// </summary>
    int FurthestVertex = 0;

    /// <summary>
    /// The inside point is used to determine which side of the face is pointing
    /// inside and which is pointing outside. This may be relatively trivial for
    /// 3D, but it is unknown for higher dimensions. It is calculated as the
    /// average of the initial simplex points.
    /// </summary>
    std::array<double, MaxDimension> InsidePoint;

    /*
     * Helper arrays to store faces for adjacency update.
     * This is just to prevent unnecessary allocations.
     */
    /// <summary>
    /// The update buffer
    /// </summary>
    int UpdateBuffer[MaxDimension] = {0};
    /// <summary>
    /// The update indices
    /// </summary>
    int UpdateIndices[MaxDimension] = {0};

    /// <summary>
    /// Used to determine which faces need to be updated at each step of the
    /// algorithm.
    /// </summary>
    std::stack<int> TraverseStack;

    /// <summary>
    /// Used for VerticesBeyond for faces that are on the convex hull.
    /// </summary>
    IndexBuffer EmptyBuffer;

    /// <summary>
    /// Used to determine which vertices are "above" (or "beyond") a face
    /// </summary>
    IndexBuffer BeyondBuffer;

    /// <summary>
    /// Stores faces that are visible from the current vertex.
    /// </summary>
    IndexBuffer AffectedFaceBuffer;

    /// <summary>
    /// Stores faces that form a "cone" created by adding new vertex.
    /// </summary>
    std::vector<DeferredFace> ConeFaceBuffer;

    /// <summary>
    /// The empty buffer stack
    /// </summary>
    std::stack<IndexBuffer> EmptyBufferStack;

    /// <summary>
    /// Stores a list of "singular" (or "generate", "planar", etc.) vertices
    /// that cannot be part of the hull.
    /// </summary>
    std::unordered_set<int> SingularVertices;

    /// <summary>
    /// The connector table helps to determine the adjacency of convex faces.
    /// Hashing is used instead of pairwise comparison. This significantly
    /// speeds up the computations, especially for higher dimensions.
    /// </summary>
    std::unordered_set<FaceConnector, FaceConnector::Hasher> ConnectorTable;

    /// <summary>
    /// Helper class for handling math related stuff.
    /// </summary>
    MathHelper mathHelper;
    IndexBuffer boundingBoxPoints[MaxDimension];
    int indexOfDimensionWithLeastExtremes = 0;
    double minima[MaxDimension];
    double maxima[MaxDimension];

public:
    ConvexHullAlgorithm() = default;

    /// <summary>
    /// Initialize
    /// </summary>
    void Initialize(double *vertices, int count, int dimension)
    {
        Positions = vertices;
        NumberOfVertices = count;
        NumOfDimensions = dimension;

        FacePool.reserve(NumberOfVertices * (NumOfDimensions + 1) + 10);
        AffectedFaceFlags.resize(NumberOfVertices * (NumOfDimensions + 1) + 10);
        VertexVisited.resize(NumberOfVertices);
        mathHelper.Initialize(NumOfDimensions, Positions);
    }

    /// <summary>
    /// Gets/calculates the convex hull. This is
    /// </summary>
    void GetConvexHull();

    /// <summary>
    /// Gets the hull vertices.
    /// </summary>
    /// <typeparam name="TVertex">The type of the t vertex.</typeparam>
    /// <param name="data">The data.</param>
    /// <returns>TVertex[].</returns>
    int GetHullVertices();

    /// <summary>
    /// Removes up facing Tetrahedrons from the triangulation.
    /// </summary>
    void RemoveUpperFaces();

private:
    /// <summary>
    /// Finds the bounding box points.
    /// </summary>
    void FindBoundingBoxPoints();

    /// <summary>
    /// Shifts and scales the Positions to avoid future errors. This does not
    /// alter the original data.
    /// </summary>
    void ShiftAndScalePositions();

    /// <summary>
    /// Find the (dimension+1) initial points and create the simplexes.
    /// Creates the initial simplex of n+1 vertices by using points from the
    /// bounding box. Special care is taken to ensure that the vertices chosen
    /// do not result in a degenerate shape where vertices are collinear
    /// (co-planar, etc). This would technically be resolved when additional
    /// vertices are checked in the main loop, but: 1) a degenerate simplex
    /// would not eliminate any other vertices (thus no savings there), 2) the
    /// creation of the face normal is prone to error.
    /// </summary>
    void CreateInitialSimplex();

    IndexBuffer FindInitialPoints();

    /// <summary>
    /// Check if 2 faces are adjacent and if so, update their AdjacentFaces
    /// array.
    /// </summary>
    /// <param name="l">The l.</param>
    /// <param name="r">The r.</param>
    void UpdateAdjacency(ConvexFaceInternal &l, ConvexFaceInternal &r);

    /// <summary>
    /// Used in the "initialization" code.
    /// </summary>
    /// <param name="face">The face.</param>
    void FindBeyondVertices(ConvexFaceInternal &face);

    /// <summary>
    /// Tags all faces seen from the current vertex with 1.
    /// </summary>
    /// <param name="currentFace">The current face.</param>
    void TagAffectedFaces(ConvexFaceInternal &currentFace);

    /// <summary>
    /// Recursively traverse all the relevant faces.
    /// </summary>
    /// <param name="currentFace">The current face.</param>
    void TraverseAffectedFaces(int currentFace);

    /// <summary>
    /// Connect faces using a connector.
    /// </summary>
    /// <param name="connector">The connector.</param>
    void ConnectFace(FaceConnector &connector);

    /// <summary>
    /// Removes the faces "covered" by the current vertex and adds the newly
    /// created ones.
    /// </summary>
    /// <returns><c>true</c> if possible, <c>false</c> otherwise.</returns>
    bool CreateCone();

    /// <summary>
    /// Commits a cone and adds a vertex to the convex hull.
    /// </summary>
    void CommitCone();

    /// <summary>
    /// Check whether the vertex v is beyond the given face. If so, add it to
    /// beyondVertices.
    /// </summary>
    /// <param name="face">The face.</param>
    /// <param name="beyondVertices">The beyond vertices.</param>
    /// <param name="v">The v.</param>
    void IsBeyond(const ConvexFaceInternal &face, IndexBuffer &beyond, int v);

    /// <summary>
    /// Compares the values of two vertices. The return value (-1, 0 or +1) are
    /// found by first checking the first coordinate and then progressing
    /// through the rest. In this way {2, 8} will be a "-1" (less than) {3, 1}.
    /// </summary>
    /// <param name="u">The base vertex index, u.</param>
    /// <param name="v">The compared vertex index, v.</param>
    /// <returns>System.Int32.</returns>
    int LexCompare(int u, int v) const;

    /// <summary>
    /// Used by update faces.
    /// </summary>
    /// <param name="face">The face.</param>
    /// <param name="beyond">The beyond.</param>
    /// <param name="beyond1">The beyond1.</param>
    void FindBeyondVertices(ConvexFaceInternal &face, IndexBuffer &beyond,
                            IndexBuffer &beyond1);

    /// <summary>
    /// Finds the beyond vertices.
    /// </summary>
    /// <param name="face">The face.</param>
    /// <param name="beyond">The beyond.</param>
    void FindBeyondVertices(ConvexFaceInternal &face, IndexBuffer &beyond);

    /// <summary>
    /// Recalculates the centroid of the current hull.
    /// </summary>
    void UpdateCenter();

    /// <summary>
    /// Removes the last vertex from the center.
    /// </summary>
    void RollbackCenter();

    /// <summary>
    /// Handles singular vertex.
    /// </summary>
    void HandleSingular();

    /// <summary>
    /// Get a vertex coordinate. In order to reduce speed, all vertex
    /// coordinates have been placed in a single array.
    /// </summary>
    /// <param name="vIndex">The vertex index.</param>
    /// <param name="dimension">The index of the dimension.</param>
    /// <returns>System.Double.</returns>
    double GetCoordinate(int vIndex, int dimension) const
    {
        int index = vIndex * NumOfDimensions + dimension;
        return Positions[index];
    }

    /// <summary>
    /// Adds a face to the list.
    /// </summary>
    /// <param name="face">The face.</param>
    void AddToUnprocessedFaces(ConvexFaceInternal &face);

    /// <summary>
    /// Removes the element from the list.
    /// </summary>
    /// <param name="face">The face.</param>
    void RemoveFromUnprocessedFaces(ConvexFaceInternal &face);

    /// <summary>
    /// Return the face to the pool for later use.
    /// </summary>
    /// <param name="faceIndex">Index of the face.</param>
    void DepositFaceToPool(int faceIndex);

    /// <summary>
    /// Return index of an unused face or creates a new one.
    /// </summary>
    /// <returns>System.Int32.</returns>
    int GetFaceFromPool();

    /// <summary>
    /// Deposit the index buffer.
    /// </summary>
    /// <param name="buffer">The buffer.</param>
    void DepositVertexBuffer(IndexBuffer &buffer)
    {
        buffer.clear();
        EmptyBufferStack.push(std::move(buffer));
    }

    /// <summary>
    /// Get a store index buffer or create a new instance.
    /// </summary>
    /// <returns>IndexBuffer.</returns>
    IndexBuffer GetVertexBuffer()
    {
        if (EmptyBufferStack.empty()) {
            return EmptyBuffer;
        }
        IndexBuffer buffer = std::move(EmptyBufferStack.top());
        EmptyBufferStack.pop();
        return buffer;
    }
};
