#include "ConvexHull.hpp"

ConvexHullResult<Vector2f> ConvexHull::Create2D(std::vector<Vector2f> &vertices)
{
    int NumOfDimensions = 2;
    int NumberOfVertices = (int)vertices.size();
    Positions.resize(NumberOfVertices * NumOfDimensions);
    size_t index = 0;
    for (auto &v : vertices) {
        Positions[index++] = v.x;
        Positions[index++] = v.y;
    }
    chAlgo.Initialize(Positions.data(), NumberOfVertices, NumOfDimensions);
    chAlgo.GetConvexHull();
    ConvexHullResult<Vector2f> result;
    Get2DResultInOrder(vertices, result);
    return result;
}

ConvexHullResult<Site> ConvexHull::Create(std::vector<Site> &vertices)
{
    int NumOfDimensions = 3;
    int NumberOfVertices = (int)vertices.size();
    Positions.resize(NumberOfVertices * NumOfDimensions);
    size_t index = 0;
    for (auto &v : vertices) {
        Positions[index++] = v.x;
        Positions[index++] = v.y;
        Positions[index++] = v.z;
    }
    chAlgo.Initialize(Positions.data(), NumberOfVertices, NumOfDimensions);
    chAlgo.GetConvexHull();
    ConvexHullResult<Site> result;
    GetHullVertices(vertices, result.Points);
    GetConvexFaces(vertices, false, NumOfDimensions, result.Faces);
    return result;
}

ConvexHullResult<Site> ConvexHull::CreateDelaunay(std::vector<Site> &vertices)
{
    int NumOfDimensions = 3;
    int NumberOfVertices = (int)vertices.size();
    Positions.resize(NumberOfVertices * NumOfDimensions);

    size_t index = 0;
    for (auto &v : vertices) {
        auto parabolaTerm = 0.0; // the lifted term is a sum of squares.
        Positions[index++] = v.x;
        Positions[index++] = v.y;
        Positions[index++] = (double)v.x * v.x + (double)v.y * v.y;
    }
    chAlgo.IsLifted = true;
    chAlgo.Initialize(Positions.data(), NumberOfVertices, NumOfDimensions);
    chAlgo.GetConvexHull();
    chAlgo.RemoveUpperFaces();
    ConvexHullResult<Site> result;
    GetConvexFaces(vertices, true, NumOfDimensions, result.Faces);
    return result;
}

void ConvexHull::GetHullVertices(std::vector<Site> &vertices,
                                 std::vector<Site *> &Points)
{
    int NumberOfVertices = (int)vertices.size();
    auto hullVertexCount = chAlgo.GetHullVertices();
    auto &VertexVisited = chAlgo.VertexVisited;

    Points.resize(hullVertexCount);
    for (auto i = 0; i < NumberOfVertices; i++) {
        if (VertexVisited[i])
            Points[--hullVertexCount] = (Site *)&vertices[i];
    }
}

template<typename T>
void ConvexHull::GetConvexFaces(std::vector<T> &vertices, bool lift, int dim,
                                std::vector<ConvexFace<T>> &Faces)
{
    if (dim < 2 || 3 < dim) {
        return;
    }
    int NumOfDimensions = dim;
    auto &FacePool = chAlgo.FacePool;
    auto &faces = chAlgo.ConvexFaces;
    auto cellCount = faces.size();
    Faces.resize(cellCount);

    for (auto i = 0; i < cellCount; i++) {
        auto &face = FacePool[faces[i]];
        auto &cell = Faces[i];
        for (auto j = 0; j < NumOfDimensions; j++) {
            cell.Vertices[j] = &vertices[face.Vertices[j]];
        }
        cell.Normal = lift ? nullptr : face.Normal.data();
        face.Tag = i;
    }

    for (auto i = 0; i < cellCount; i++) {
        auto &face = FacePool[faces[i]];
        auto &cell = Faces[i];
        for (auto j = 0; j < NumOfDimensions; j++) {
            if (face.AdjacentFaces[j] < 0) {
                cell.Adjacency[j] = nullptr;
                continue;
            }
            cell.Adjacency[j] = &Faces[FacePool[face.AdjacentFaces[j]].Tag];
        }

        // Fix the vertex orientation.
        if (face.IsNormalFlipped) {
            auto tempVertex = cell.Vertices[0];
            cell.Vertices[0] = cell.Vertices[NumOfDimensions - 1];
            cell.Vertices[NumOfDimensions - 1] = tempVertex;

            auto tempAdj = cell.Adjacency[0];
            cell.Adjacency[0] = cell.Adjacency[NumOfDimensions - 1];
            cell.Adjacency[NumOfDimensions - 1] = tempAdj;
        }
    }
}

void ConvexHull::Get2DResultInOrder(std::vector<Vector2f> &data,
                                    ConvexHullResult<Vector2f> &result)
{
    std::vector<ConvexFace<Vector2f>> Faces;
    GetConvexFaces<Vector2f>(data, true, 2, Faces);
    int num = Faces.size();
    std::map<Vector2f *, ConvexFace<Vector2f> *> dictionary;
    for (auto &val : Faces) {
        auto ptr = (Vector2f *)val.Vertices[1];
        dictionary.insert({ptr, &val});
    }
    auto val2 = Faces[0].Vertices[1];
    auto val3 = Faces[0].Vertices[0];
    std::vector<Vector2f *> list;
    list.push_back(val2);
    int num2 = 0;
    int num3 = 0;
    while (val3 != val2) {
        list.push_back(val3);
        auto val4 = dictionary[val3];
        if (val3->x < list[num2]->x ||
            (val3->x == list[num2]->x && val3->y <= list[num2]->y)) {
            num2 = num3;
        }
        num3++;
        val3 = val4->Vertices[0];
    }
    result.Points.resize(num);
    for (int j = 0; j < num; j++) {
        int index = (j + num2) % num;
        result.Points[j] = list[index];
    }
}
