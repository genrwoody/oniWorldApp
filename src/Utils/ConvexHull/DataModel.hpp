#pragma once

#include <array>
#include <vector>
#include <stack>

struct ConvexFaceInternal;
struct DeferredFace;
struct FaceConnector;

struct ConvexFaceInternal {
    /// <summary>
    /// The face.
    /// </summary>
    ConvexFaceInternal *Face = nullptr;

    /// <summary>
    /// Gets or sets the adjacent face data.
    /// </summary>
    std::array<int, MaxDimension> AdjacentFaces = {0};

    /// <summary>
    /// The furthest vertex.
    /// </summary>
    int FurthestVertex = 0;

    /// <summary>
    /// Index of the face inside the pool.
    /// </summary>
    int Index = 0;

    /// <summary>
    /// Is it present in the list.
    /// </summary>
    bool InList = false;

    /// <summary>
    /// Is the normal flipped?
    /// </summary>
    bool IsNormalFlipped = false;

    /// <summary>
    /// Next node in the list.
    /// </summary>
    ConvexFaceInternal *Next = nullptr;

    /// <summary>
    /// Gets or sets the normal vector.
    /// </summary>
    std::array<double, MaxDimension> Normal = {0};

    /// <summary>
    /// Face plane constant element.
    /// </summary>
    double Offset = 0.0;

    // public int UnprocessedIndex;

    /// <summary>
    /// Prev node in the list.
    /// </summary>
    ConvexFaceInternal *Previous = nullptr;

    /// <summary>
    /// Used to traverse affected faces and create the Delaunay representation.
    /// </summary>
    int Tag = 0;

    /// <summary>
    /// Gets or sets the vertices.
    /// </summary>
    std::array<int, MaxDimension + 1> Vertices = {0};

    /// <summary>
    /// Gets or sets the vertices beyond.
    /// </summary>
    std::vector<int> VerticesBeyond;

    /// <summary>
    /// Initializes a new instance of the <see cref="ConvexFaceInternal" />
    /// class.
    /// </summary>
    /// <param name="dimension">The dimension.</param>
    /// <param name="index">The index.</param>
    /// <param name="beyondList">The beyond list.</param>
    ConvexFaceInternal(int index, std::vector<int> &beyondList)
    {
        Index = index;
        VerticesBeyond.swap(beyondList);
    }

    ConvexFaceInternal() = default;
};

struct DeferredFace {
    /// <summary>
    /// The faces.
    /// </summary>
    ConvexFaceInternal *Face, *Pivot, *OldFace;

    /// <summary>
    /// The indices.
    /// </summary>
    int FaceIndex, PivotIndex;
};

struct FaceConnector {
    /// <summary>
    /// The edge to be connected.
    /// </summary>
    int EdgeIndex = 0;

    /// <summary>
    /// The hash code computed from indices.
    /// </summary>
    size_t HashCode = 0;

    /// <summary>
    /// The face.
    /// </summary>
    ConvexFaceInternal *Face = nullptr;

    /// <summary>
    /// The vertex indices.
    /// </summary>
    std::array<int, MaxDimension - 1> Vertices = {0};

    /// <summary>
    /// Updates the connector.
    /// </summary>
    /// <param name="face">The face.</param>
    /// <param name="edgeIndex">Index of the edge.</param>
    /// <param name="dim">The dim.</param>
    void Update(ConvexFaceInternal *face, int edgeIndex, int dim);

    /// <summary>
    /// Can two faces be connected.
    /// </summary>
    /// <param name="a">a.</param>
    /// <param name="b">The b.</param>
    /// <returns><c>true</c> if XXXX, <c>false</c> otherwise.</returns>
    bool operator==(const FaceConnector &b) const
    {
        return HashCode == b.HashCode && Vertices == b.Vertices;
    }

    /// <summary>
    /// Connect two faces.
    /// </summary>
    /// <param name="a">a.</param>
    /// <param name="b">The b.</param>
    void Connect(FaceConnector &b)
    {
        Face->AdjacentFaces[EdgeIndex] = b.Face->Index;
        b.Face->AdjacentFaces[b.EdgeIndex] = Face->Index;
    }

    struct Hasher {
        size_t operator()(const FaceConnector &value) const
        {
            return value.HashCode;
        }
    };
};
