#include "Algorithm.hpp"

#include <algorithm>
#include <numeric>
#include <ranges>

#include "DataModel.hpp"
#include "MathHelper.hpp"

#include "../KRandom.hpp"

void FaceConnector::Update(ConvexFaceInternal *face, int edgeIndex, int dim)
{
    Face = face;
    EdgeIndex = edgeIndex;

    size_t hashCode = 32;

    auto &vs = face->Vertices;
    int i, c = 0;
    for (i = 0; i < edgeIndex; i++) {
        Vertices[c++] = vs[i];
        hashCode += 31 * hashCode + vs[i];
    }
    for (i = edgeIndex + 1; i < dim; i++) {
        Vertices[c++] = vs[i];
        hashCode += 31 * hashCode + vs[i];
    }

    HashCode = hashCode;
}

void ConvexHullAlgorithm::DepositFaceToPool(int faceIndex)
{
    auto &face = FacePool[faceIndex];
    auto &af = face.AdjacentFaces;
    for (int i = 0; i < NumOfDimensions; i++) {
        af[i] = -1;
    }
    FreeFaceIndices.push(faceIndex);
}

int ConvexHullAlgorithm::GetFaceFromPool()
{
    if (FreeFaceIndices.size() > 0) {
        int index = FreeFaceIndices.top();
        FreeFaceIndices.pop();
        return index;
    }
    int index = (int)FacePool.size();
    IndexBuffer buffer = GetVertexBuffer();
    FacePool.emplace_back(index, buffer);
    if (FacePool.size() > AffectedFaceFlags.size()) {
        AffectedFaceFlags.resize(AffectedFaceFlags.size() * 2);
    }
    return index;
}

void ConvexHullAlgorithm::AddToUnprocessedFaces(ConvexFaceInternal &face)
{
    auto &First = FirstUnprocessedFace;
    auto &last = lastUnprocessedFace;
    if (face.InList) {
        if (First->VerticesBeyond.size() < face.VerticesBeyond.size()) {
            RemoveFromUnprocessedFaces(face);
            face.InList = true;
            First->Previous = &face;
            face.Next = First;
            First = &face;
        }
        return;
    }

    face.InList = true;

    if (First != nullptr &&
        First->VerticesBeyond.size() < face.VerticesBeyond.size()) {
        First->Previous = &face;
        face.Next = First;
        First = &face;
    } else {
        if (last != nullptr) {
            last->Next = &face;
        }
        face.Previous = last;
        last = &face;
        if (First == nullptr) {
            First = &face;
        }
    }
}

void ConvexHullAlgorithm::RemoveFromUnprocessedFaces(ConvexFaceInternal &face)
{
    if (!face.InList)
        return;

    face.InList = false;

    if (face.Previous != nullptr) {
        face.Previous->Next = face.Next;
    } else if (/*first == face*/ face.Previous == nullptr) {
        FirstUnprocessedFace = face.Next;
    }

    if (face.Next != nullptr) {
        face.Next->Previous = face.Previous;
    } else if (/*last == face*/ face.Next == nullptr) {
        lastUnprocessedFace = face.Previous;
    }

    face.Next = nullptr;
    face.Previous = nullptr;
}

void ConvexHullAlgorithm::GetConvexHull()
{
    // next the bounding box extremes are found. This is used to shift,
    // scale and find the starting simplex.
    FindBoundingBoxPoints();
    // the positions are shifted to avoid divide by zero problems
    // and if Delaunay or Voronoi, then the parabola terms are scaled back
    // to match the size of the other coords
    ShiftAndScalePositions();
    // Find the (dimension+1) initial points and create the simplexes.
    CreateInitialSimplex();

    // Now, the main loop. These initial faces of a simplex are replaced and
    // expanded outwards to make the convex hull and faces.
    while (FirstUnprocessedFace != nullptr) {
        auto &currentFace = *FirstUnprocessedFace;
        CurrentVertex = currentFace.FurthestVertex;

        UpdateCenter();
        // The affected faces get tagged
        TagAffectedFaces(currentFace);

        // Create the cone from the currentVertex and the affected faces
        // horizon.
        if (!SingularVertices.contains(CurrentVertex) && CreateCone())
            CommitCone();
        else
            HandleSingular();

        // Need to reset the tags
        auto count = (int)AffectedFaceBuffer.size();
        for (auto i = 0; i < count; i++)
            AffectedFaceFlags[AffectedFaceBuffer[i]] = false;
    }
}

void ConvexHullAlgorithm::FindBoundingBoxPoints()
{
    indexOfDimensionWithLeastExtremes = -1;
    int minNumExtremes = std::numeric_limits<int>::max();
    for (int i = 0; i < NumOfDimensions; i++) {
        IndexBuffer minIndices;
        IndexBuffer maxIndices;
        double min = std::numeric_limits<double>::max();
        double max = std::numeric_limits<double>::lowest();
        for (int j = 0; j < NumberOfVertices; j++) {
            auto v = GetCoordinate(j, i);
            auto difference = min - v;
            if (difference >= PlaneDistanceTolerance) {
                // you found a better solution than before, clear out the list
                // and store new value
                min = v;
                minIndices.clear();
                minIndices.push_back(j);
            } else if (difference > 0) {
                // you found a solution slightly better than before, clear out
                // those that are no longer on the list and store new value
                min = v;
                auto ret = std::ranges::remove_if(
                    minIndices, [this, i, min](int index) {
                        return min - GetCoordinate(index, i) >
                               PlaneDistanceTolerance;
                    });
                minIndices.erase(ret.begin(), ret.end());
                minIndices.push_back(j);
            } else if (difference > -PlaneDistanceTolerance) {
                // same or almost as good as current limit, so store it
                minIndices.push_back(j);
            }
            difference = v - max;
            if (difference >= PlaneDistanceTolerance) {
                // you found a better solution than before, clear out the list
                // and store new value
                max = v;
                maxIndices.clear();
                maxIndices.push_back(j);
            } else if (difference > 0) {
                // you found a solution slightly better than before, clear out
                // those that are no longer on the list and store new value
                max = v;
                auto ret = std::ranges::remove_if(
                    maxIndices, [this, i, min](int index) {
                        return min - GetCoordinate(index, i) >
                               PlaneDistanceTolerance;
                    });
                maxIndices.erase(ret.begin(), ret.end());
                maxIndices.push_back(j);
            } else if (difference > -PlaneDistanceTolerance) {
                // same or almost as good as current limit, so store it
                maxIndices.push_back(j);
            }
        }
        minima[i] = min;
        maxima[i] = max;
        minIndices.insert(minIndices.end(), maxIndices.begin(),
                          maxIndices.end());
        if ((int)minIndices.size() < minNumExtremes) {
            minNumExtremes = minIndices.size();
            indexOfDimensionWithLeastExtremes = i;
        }
        boundingBoxPoints[i] = minIndices;
    }
}

void ConvexHullAlgorithm::ShiftAndScalePositions()
{
    int positionsLength = NumberOfVertices * NumOfDimensions;

    if (IsLifted) {
        int origNumDim = NumOfDimensions - 1;
        double min = 0.0, max = 0.0;
        for (int i = 0; i < NumOfDimensions; ++i) {
            min += std::abs(minima[i]);
            max += std::abs(maxima[i]);
        }
        auto parabolaScale = 2 / (min + max - std::abs(maxima[origNumDim]) -
                                  std::abs(minima[origNumDim]));
        // the parabola scale is 1 / average of the sum of the other dimensions.
        // multiplying this by the parabola will scale it back to be on near
        // similar size to the other dimensions. Without this, the term is much
        // larger than the others, which causes problems for roundoff error and
        // finding the normal of faces.
        // change the extreme values as well
        minima[origNumDim] *= parabolaScale;
        maxima[origNumDim] *= parabolaScale;
        // it is done here because
        for (int i = origNumDim; i < positionsLength; i += NumOfDimensions)
            Positions[i] *= parabolaScale;
    }
    double shiftAmount[MaxDimension] = {0};
    for (int i = 0; i < NumOfDimensions; i++)
        // now the entire model is shifted to all positive numbers...plus
        // some more. why? 1) to avoid dealing with a point at the origin
        // {0,0,...,0} which causes problems
        //    for future normal finding
        // 2) note that weird shift that is used (max - min - min). This is
        // to avoid scaling
        //    issues. this shift means that the minima in a dimension will
        //    always be a positive number (no points at zero), and the
        //    minima [in a given dimension] will always be half of the
        //    maxima. 'Half' is much preferred to 'thousands of times' Think
        //    of the first term as the range (max - min), then the second
        //    term avoids cases where there are both positive and negative
        //    numbers.
        if (maxima[i] == minima[i])
            shiftAmount[i] = 0.0;
        else
            shiftAmount[i] = (maxima[i] - minima[i]) - minima[i];
    for (int i = 0; i < positionsLength; i++)
        Positions[i] += shiftAmount[i % NumOfDimensions];
}

void ConvexHullAlgorithm::CreateInitialSimplex()
{
    auto initialPoints = FindInitialPoints();
    // region Create the first faces from(dimension + 1) vertices.

    int faces[MaxDimension + 1] = {0};

    for (int i = 0; i < NumOfDimensions + 1; i++) {
        std::array<int, MaxDimension + 1> vertices = {0};
        for (int j = 0, k = 0; j <= NumOfDimensions; j++) {
            if (i != j)
                vertices[k++] = initialPoints[j];
        }
        std::sort(vertices.begin(), vertices.begin() + NumOfDimensions);
        auto &newFace = FacePool[GetFaceFromPool()];
        newFace.Vertices = vertices;
        mathHelper.CalculateFacePlane(newFace, InsidePoint.data());
        faces[i] = newFace.Index;
    }
    // update the adjacency (check all pairs of faces)
    for (int i = 0; i < NumOfDimensions; i++)
        for (int j = i + 1; j < NumOfDimensions + 1; j++)
            UpdateAdjacency(FacePool[faces[i]], FacePool[faces[j]]);
    // region Init the vertex beyond buffers.

    for (int i = 0; i <= NumOfDimensions; ++i) {
        auto &face = FacePool[faces[i]];
        FindBeyondVertices(face);
        if (face.VerticesBeyond.empty())
            ConvexFaces.push_back(face.Index); // The face is on the hull
        else
            AddToUnprocessedFaces(face);
    }
    // Set all vertices to false (unvisited).
    for (auto &vertex : initialPoints)
        VertexVisited[vertex] = false;
}

std::vector<int> ConvexHullAlgorithm::FindInitialPoints()
{
    double bigNumber = 0.0;
    for (int i = 0; i < NumOfDimensions; ++i) {
        bigNumber += maxima[i];
    }
    bigNumber *= NumOfDimensions * NumberOfVertices;
    // the first two points are taken from the dimension that had the fewest
    // extremes well, in most cases there will only be 2 in all dimensions: one
    // min and one max but a lot of engineering part shapes are nice and square
    // and can have hundreds of parallel vertices at the extremes
    auto &bbPoints = boundingBoxPoints[indexOfDimensionWithLeastExtremes];
    int vertex1 = bbPoints.front(); // these are min and max vertices along
    int vertex2 = bbPoints.back();  // the dimension that had the fewest points
    bbPoints.erase(bbPoints.begin());
    bbPoints.pop_back();
    IndexBuffer initialPoints{vertex1, vertex2};
    VertexVisited[vertex1] = VertexVisited[vertex2] = true;
    CurrentVertex = vertex1;
    UpdateCenter();
    CurrentVertex = vertex2;
    UpdateCenter();
    std::array<double, MaxDimension> edgeVectors[MaxDimension];
    mathHelper.VectorBetweenVertices(vertex2, vertex1, edgeVectors[0].data());
    // now the remaining vertices are just combined in one big list
    IndexBuffer extremes;
    for (int i = 0; i < NumOfDimensions; ++i) {
        auto &points = boundingBoxPoints[i];
        extremes.insert(extremes.end(), points.begin(), points.end());
    }
    // otherwise find the remaining points by maximizing the initial simplex
    // volume
    int index = 1;
    while (index < NumOfDimensions && !extremes.empty()) {
        int bestVertex = -1;
        std::array<double, MaxDimension> bestEdgeVector;
        double maxVolume = DefaultPlaneDistanceTolerance;
        for (int i = extremes.size() - 1; i >= 0; i--) {
            // count backwards in order to remove potential duplicates
            int vIndex = extremes[i];
            if (std::ranges::contains(initialPoints, vIndex))
                extremes.erase(extremes.begin() + i);
            else {
                mathHelper.VectorBetweenVertices(vIndex, vertex1,
                                                 edgeVectors[index].data());
                auto volume =
                    mathHelper.GetSimplexVolume(edgeVectors, index, bigNumber);
                if (maxVolume < volume) {
                    maxVolume = volume;
                    bestVertex = vIndex;
                    bestEdgeVector = edgeVectors[index];
                }
            }
        }
        auto ret = std::ranges::remove(extremes, bestVertex);
        extremes.erase(ret.begin(), ret.end());
        if (bestVertex == -1)
            break;
        initialPoints.push_back(bestVertex);
        edgeVectors[index++] = bestEdgeVector;
        CurrentVertex = bestVertex;
        UpdateCenter();
    }
    // hmm, there are not enough points on the bounding box to make a simplex.
    // It is rare but entirely possibly. As an extreme, the bounding box can be
    // made in n dimensions from only 2 unique points. When we can't find enough
    // unique points, we start again with ALL the vertices. The following is a
    // near replica of the code above, but instead of extremes, we consider
    // "allVertices".
    if ((int)initialPoints.size() <= NumOfDimensions && !IsLifted) {
        std::vector<int> allVertices(NumberOfVertices);
        std::iota(allVertices.begin(), allVertices.end(), 0);
        while (index < NumOfDimensions && !allVertices.empty()) {
            int bestVertex = -1;
            std::array<double, MaxDimension> bestEdgeVector;
            double maxVolume = 0.0;
            for (int i = allVertices.size() - 1; i >= 0; i--) {
                // count backwards in order to remove potential duplicates
                int vIndex = allVertices[i];
                if (std::ranges::contains(initialPoints, vIndex))
                    allVertices.erase(allVertices.begin() + i);
                else {
                    mathHelper.VectorBetweenVertices(vIndex, vertex1,
                                                     edgeVectors[index].data());
                    double volume = mathHelper.GetSimplexVolume(
                        edgeVectors, index, bigNumber);
                    if (maxVolume < volume) {
                        maxVolume = volume;
                        bestVertex = vIndex;
                        bestEdgeVector = edgeVectors[index];
                    }
                }
            }
            auto ret = std::ranges::remove(extremes, bestVertex);
            extremes.erase(ret.begin(), ret.end());
            if (bestVertex == -1)
                break;
            initialPoints.push_back(bestVertex);
            edgeVectors[index++] = bestEdgeVector;
            CurrentVertex = bestVertex;
            UpdateCenter();
        }
    }
    if ((int)initialPoints.size() <= NumOfDimensions && IsLifted) {
        /*
        std::vector<int> allVertices(NumberOfVertices);
        std::iota(allVertices.begin(), allVertices.end(), 0);
        while (index < NumOfDimensions && !allVertices.empty()) {
            int bestVertex = -1;
            std::array<double, MaxDimension> bestEdgeVector;
            double maxVolume = 0.0;
            for (int i = allVertices.size() - 1; i >= 0; i--) {
                // count backwards in order to remove potential duplicates
                int vIndex = allVertices[i];
                if (std::ranges::contains(initialPoints, vIndex))
                    allVertices.erase(allVertices.begin() + i);
                else {
                    mathHelper.RandomOffsetToLift(vIndex, maxima.Last() -
                                                              minima.Last());
                    edgeVectors[index] =
                        mathHelper.VectorBetweenVertices(vIndex, vertex1);
                    var volume = mathHelper.GetSimplexVolume(edgeVectors, index,
                                                             bigNumber);
                    if (maxVolume < volume) {
                        maxVolume = volume;
                        bestVertex = vIndex;
                        bestEdgeVector = edgeVectors[index];
                    }
                }
            }
            allVertices.Remove(bestVertex);
            if (bestVertex == -1)
                break;
            initialPoints.Add(bestVertex);
            edgeVectors[index++] = bestEdgeVector;
            CurrentVertex = bestVertex;
            UpdateCenter();
        }
        */
    }
    return initialPoints;
}

void ConvexHullAlgorithm::UpdateAdjacency(ConvexFaceInternal &l,
                                          ConvexFaceInternal &r)
{
    auto lv = l.Vertices;
    auto rv = r.Vertices;
    int i;

    // reset marks on the 1st face
    for (i = 0; i < NumOfDimensions; i++)
        VertexVisited[lv[i]] = false;

    // mark all vertices on the 2nd face
    for (i = 0; i < NumOfDimensions; i++)
        VertexVisited[rv[i]] = true;

    // find the 1st false index
    for (i = 0; i < NumOfDimensions; i++)
        if (!VertexVisited[lv[i]])
            break;

    // no vertex was marked
    if (i == NumOfDimensions)
        return;

    // check if only 1 vertex wasn't marked
    for (auto j = i + 1; j < NumOfDimensions; j++)
        if (!VertexVisited[lv[j]])
            return;

    // if we are here, the two faces share an edge
    l.AdjacentFaces[i] = r.Index;

    // update the adj. face on the other face - find the vertex that remains
    // marked
    for (i = 0; i < NumOfDimensions; i++)
        VertexVisited[lv[i]] = false;
    for (i = 0; i < NumOfDimensions; i++) {
        if (VertexVisited[rv[i]])
            break;
    }
    r.AdjacentFaces[i] = l.Index;
}

void ConvexHullAlgorithm::FindBeyondVertices(ConvexFaceInternal &face)
{
    auto &beyondVertices = face.VerticesBeyond;
    MaxDistance = std::numeric_limits<double>::lowest();
    FurthestVertex = 0;
    for (auto i = 0; i < NumberOfVertices; i++) {
        if (VertexVisited[i])
            continue;
        IsBeyond(face, beyondVertices, i);
    }

    face.FurthestVertex = FurthestVertex;
}

void ConvexHullAlgorithm::TagAffectedFaces(ConvexFaceInternal &currentFace)
{
    AffectedFaceBuffer.clear();
    AffectedFaceBuffer.push_back(currentFace.Index);
    TraverseAffectedFaces(currentFace.Index);
}

void ConvexHullAlgorithm::TraverseAffectedFaces(int currentFace)
{
    std::stack<int> empty;
    TraverseStack.swap(empty);
    TraverseStack.push(currentFace);
    AffectedFaceFlags[currentFace] = true;

    while (TraverseStack.size() > 0) {
        auto topIndex = TraverseStack.top();
        TraverseStack.pop();
        auto &top = FacePool[topIndex];
        for (auto i = 0; i < NumOfDimensions; i++) {
            auto adjFace = top.AdjacentFaces[i];
            if (AffectedFaceFlags[adjFace])
                continue;
            auto distance =
                mathHelper.GetVertexDistance(CurrentVertex, FacePool[adjFace]);
            if (distance >= PlaneDistanceTolerance) {
                AffectedFaceBuffer.push_back(adjFace);
                AffectedFaceFlags[adjFace] = true;
                TraverseStack.push(adjFace);
            }
        }
    }
}

void ConvexHullAlgorithm::ConnectFace(FaceConnector &connector)
{
    auto current = ConnectorTable.extract(connector);
    if (current) {
        current.value().Connect(connector);
    } else {
        ConnectorTable.emplace(connector);
    }
}

bool ConvexHullAlgorithm::CreateCone()
{
    auto currentVertexIndex = CurrentVertex;
    ConeFaceBuffer.clear();

    for (auto fIndex = 0; fIndex < (int)AffectedFaceBuffer.size(); fIndex++) {
        auto oldFaceIndex = AffectedFaceBuffer[fIndex];
        auto &oldFace = FacePool[oldFaceIndex];

        // Find the faces that need to be updated
        auto updateCount = 0;
        for (auto i = 0; i < NumOfDimensions; i++) {
            auto af = oldFace.AdjacentFaces[i];
            // Tag == false when oldFaces does not contain af
            if (!AffectedFaceFlags[af]) {
                UpdateBuffer[updateCount] = af;
                UpdateIndices[updateCount] = i;
                ++updateCount;
            }
        }

        for (auto i = 0; i < updateCount; i++) {
            auto &adjacentFace = FacePool[UpdateBuffer[i]];

            auto oldFaceAdjacentIndex = 0;
            auto adjFaceAdjacency = adjacentFace.AdjacentFaces;
            for (auto j = 0; j < NumOfDimensions; j++) {
                if (oldFaceIndex == adjFaceAdjacency[j]) {
                    oldFaceAdjacentIndex = j;
                    break;
                }
            }

            int forbidden = UpdateIndices[i];
            // Index of the face that corresponds to this adjacent face
            auto &newFace = FacePool[GetFaceFromPool()];
            auto &vertices = newFace.Vertices;
            for (auto j = 0; j < NumOfDimensions; j++)
                vertices[j] = oldFace.Vertices[j];
            auto oldVertexIndex = vertices[forbidden];

            int orderedPivotIndex;

            // correct the ordering
            if (currentVertexIndex < oldVertexIndex) {
                orderedPivotIndex = 0;
                for (auto j = forbidden; j > 0; j--) {
                    if (vertices[j - 1] > currentVertexIndex)
                        vertices[j] = vertices[j - 1];
                    else {
                        orderedPivotIndex = j;
                        break;
                    }
                }
            } else {
                orderedPivotIndex = NumOfDimensions - 1;
                for (auto j = forbidden + 1; j < NumOfDimensions; j++) {
                    if (vertices[j] < currentVertexIndex)
                        vertices[j - 1] = vertices[j];
                    else {
                        orderedPivotIndex = j - 1;
                        break;
                    }
                }
            }

            vertices[orderedPivotIndex] = CurrentVertex;

            if (!mathHelper.CalculateFacePlane(newFace, InsidePoint.data())) {
                return false;
            }
            ConeFaceBuffer.emplace_back(&newFace, &adjacentFace, &oldFace,
                                        (int)orderedPivotIndex,
                                        oldFaceAdjacentIndex);
        }
    }

    return true;
}

void ConvexHullAlgorithm::CommitCone()
{
    // Fill the adjacency.
    for (auto i = 0; i < (int)ConeFaceBuffer.size(); i++) {
        auto &face = ConeFaceBuffer[i];

        auto &newFace = *face.Face;
        auto &adjacentFace = *face.Pivot;
        auto &oldFace = *face.OldFace;
        auto orderedPivotIndex = face.FaceIndex;

        newFace.AdjacentFaces[orderedPivotIndex] = adjacentFace.Index;
        adjacentFace.AdjacentFaces[face.PivotIndex] = newFace.Index;

        // let there be a connection.
        for (auto j = 0; j < NumOfDimensions; j++) {
            if (j == orderedPivotIndex)
                continue;
            FaceConnector connector;
            connector.Update(&newFace, j, NumOfDimensions);
            ConnectFace(connector);
        }

        // the id adjacent face on the hull? If so, we can use simple method
        // to find beyond vertices.
        if (adjacentFace.VerticesBeyond.empty())
            FindBeyondVertices(newFace, oldFace.VerticesBeyond);
        // it is slightly more effective if the face with the lower number
        // of beyond vertices comes first.
        else if (adjacentFace.VerticesBeyond.size() <
                 oldFace.VerticesBeyond.size())
            FindBeyondVertices(newFace, adjacentFace.VerticesBeyond,
                               oldFace.VerticesBeyond);
        else
            FindBeyondVertices(newFace, oldFace.VerticesBeyond,
                               adjacentFace.VerticesBeyond);

        // This face will definitely lie on the hull
        if (newFace.VerticesBeyond.empty()) {
            ConvexFaces.push_back(newFace.Index);
            RemoveFromUnprocessedFaces(newFace);
            DepositVertexBuffer(newFace.VerticesBeyond);
        } else {
            // Add the face to the list
            AddToUnprocessedFaces(newFace);
        }
    }

    // Recycle the affected faces.
    for (auto fIndex = 0; fIndex < (int)AffectedFaceBuffer.size(); fIndex++) {
        auto face = AffectedFaceBuffer[fIndex];
        RemoveFromUnprocessedFaces(FacePool[face]);
        DepositFaceToPool(face);
    }
}

void ConvexHullAlgorithm::IsBeyond(const ConvexFaceInternal &face,
                                   IndexBuffer &beyond, int v)
{
    auto distance = mathHelper.GetVertexDistance(v, face);
    if (distance >= PlaneDistanceTolerance) {
        if (distance > MaxDistance) {
            // If it's within the tolerance distance, use the lex. larger point
            if (distance - MaxDistance < PlaneDistanceTolerance) {
                // todo: why is this LexCompare necessary. Would seem to favor x
                // over y over z (etc.)?
                if (LexCompare(v, FurthestVertex) > 0) {
                    MaxDistance = distance;
                    FurthestVertex = v;
                }
            } else {
                MaxDistance = distance;
                FurthestVertex = v;
            }
        }
        beyond.push_back(v);
    }
}

int ConvexHullAlgorithm::LexCompare(int u, int v) const
{
    u *= NumOfDimensions;
    v *= NumOfDimensions;
    for (int i = 0; i < NumOfDimensions; i++) {
        double x = Positions[u + i], y = Positions[v + i];
        auto comp = x < y ? -1 : (x == y ? 0 : 1);
        if (comp != 0)
            return comp;
    }
    return 0;
}

void ConvexHullAlgorithm::FindBeyondVertices(ConvexFaceInternal &face,
                                             IndexBuffer &beyond,
                                             IndexBuffer &beyond1)
{
    auto &beyondVertices = BeyondBuffer;

    MaxDistance = std::numeric_limits<double>::lowest();
    FurthestVertex = 0;
    int v;

    for (auto i = 0; i < (int)beyond1.size(); i++)
        VertexVisited[beyond1[i]] = true;
    VertexVisited[CurrentVertex] = false;
    for (auto i = 0; i < (int)beyond.size(); i++) {
        v = beyond[i];
        if (v == CurrentVertex)
            continue;
        VertexVisited[v] = false;
        IsBeyond(face, beyondVertices, v);
    }

    for (auto i = 0; i < (int)beyond1.size(); i++) {
        v = beyond1[i];
        if (VertexVisited[v])
            IsBeyond(face, beyondVertices, v);
    }

    face.FurthestVertex = FurthestVertex;

    // Pull the old switch a roo (switch the face beyond buffers)
    face.VerticesBeyond.swap(beyondVertices);
    BeyondBuffer.clear();
}

void ConvexHullAlgorithm::FindBeyondVertices(ConvexFaceInternal &face,
                                             IndexBuffer &beyond)
{
    auto &beyondVertices = BeyondBuffer;

    MaxDistance = std::numeric_limits<double>::lowest();
    FurthestVertex = 0;

    for (auto i = 0; i < (int)beyond.size(); i++) {
        auto v = beyond[i];
        if (v == CurrentVertex)
            continue;
        IsBeyond(face, beyondVertices, v);
    }

    face.FurthestVertex = FurthestVertex;

    // Pull the old switch a roo (switch the face beyond buffers)
    face.VerticesBeyond.swap(beyondVertices);
    BeyondBuffer.clear();
}

void ConvexHullAlgorithm::UpdateCenter()
{
    for (int i = 0; i < NumOfDimensions; i++)
        InsidePoint[i] *= ConvexHullSize;
    ConvexHullSize += 1;
    double f = 1.0 / ConvexHullSize;
    int co = CurrentVertex * NumOfDimensions;
    for (int i = 0; i < NumOfDimensions; i++)
        InsidePoint[i] = f * (InsidePoint[i] + Positions[co + i]);
}

void ConvexHullAlgorithm::RollbackCenter()
{
    for (int i = 0; i < NumOfDimensions; i++)
        InsidePoint[i] *= ConvexHullSize;
    ConvexHullSize -= 1;
    double f = ConvexHullSize > 0 ? 1.0 / ConvexHullSize : 0.0;
    int co = CurrentVertex * NumOfDimensions;
    for (int i = 0; i < NumOfDimensions; i++)
        InsidePoint[i] = f * (InsidePoint[i] - Positions[co + i]);
}

void ConvexHullAlgorithm::HandleSingular()
{
    RollbackCenter();

    SingularVertices.emplace(CurrentVertex);

    // This means that all the affected faces must be on the hull and that
    // all their "vertices beyond" are singular.
    for (auto fIndex = 0; fIndex < (int)AffectedFaceBuffer.size(); fIndex++) {
        auto &face = FacePool[AffectedFaceBuffer[fIndex]];
        auto &vb = face.VerticesBeyond;
        for (auto i = 0; i < (int)vb.size(); i++) {
            SingularVertices.emplace(vb[i]);
        }

        ConvexFaces.push_back(face.Index);
        RemoveFromUnprocessedFaces(face);
        DepositVertexBuffer(face.VerticesBeyond);
    }
}

int ConvexHullAlgorithm::GetHullVertices()
{
    auto cellCount = (int)ConvexFaces.size();
    auto hullVertexCount = 0;

    for (auto i = 0; i < NumberOfVertices; i++)
        VertexVisited[i] = false;

    for (auto i = 0; i < cellCount; i++) {
        auto &vs = FacePool[ConvexFaces[i]].Vertices;
        for (auto j = 0; j < NumOfDimensions; j++) {
            auto v = vs[j];
            if (!VertexVisited[v]) {
                VertexVisited[v] = true;
                hullVertexCount++;
            }
        }
    }
    return hullVertexCount;
}

void ConvexHullAlgorithm::RemoveUpperFaces()
{
    auto &delaunayFaces = ConvexFaces;
    int dimension = NumOfDimensions - 1;

    // Remove the "upper" faces
    for (int i = (int)delaunayFaces.size() - 1; i >= 0; i--) {
        int candidateIndex = delaunayFaces[i];
        auto &candidate = FacePool[candidateIndex];
        if (candidate.Normal[dimension] >= 0.0) {
            for (int fi = 0; fi < dimension; fi++) {
                int af = candidate.AdjacentFaces[fi];
                if (af >= 0) {
                    auto &face = FacePool[af];
                    for (int j = 0; j < dimension; j++) {
                        if (face.AdjacentFaces[j] == candidateIndex) {
                            face.AdjacentFaces[j] = -1;
                        }
                    }
                }
            }
            delaunayFaces[i] = delaunayFaces[delaunayFaces.size() - 1];
            delaunayFaces.pop_back();
        }
    }
}
