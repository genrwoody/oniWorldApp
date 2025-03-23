#include "ConvexHull.hpp"

Vector2f ConvexFace::GetDualPoint()
{
    if (!dualPoint.has_value()) {
        Vector3d vec1(Vertices[0]->x, Vertices[0]->y, Vertices[0]->z);
        Vector3d vec2(Vertices[1]->x, Vertices[1]->y, Vertices[1]->z);
        Vector3d vec3(Vertices[2]->x, Vertices[2]->y, Vertices[2]->z);
        double num1 = vec1.y * (vec2.z - vec3.z) + vec2.y * (vec3.z - vec1.z) +
                      vec3.y * (vec1.z - vec2.z);
        double num2 = vec1.z * (vec2.x - vec3.x) + vec2.z * (vec3.x - vec1.x) +
                      vec3.z * (vec1.x - vec2.x);
        double num3 = vec1.x * (vec2.y - vec3.y) + vec2.x * (vec3.y - vec1.y) +
                      vec3.x * (vec1.y - vec2.y);
        num3 = -0.5 / num3;
        dualPoint.emplace((float)(num1 * num3), (float)(num2 * num3));
    }
    return dualPoint.value();
}

inline static double Det(Vector3d *m)
{
    return m[0].x * (m[1].y * m[2].z - m[2].y * m[1].z) -
           m[0].y * (m[1].x * m[2].z - m[2].x * m[1].z) +
           m[0].z * (m[1].x * m[2].y - m[2].x * m[1].y);
}

inline static double LengthSquared(double x, double y) { return x * x + y * y; }

Vector2f ConvexFace::GetCircumcenter()
{
    if (!circumcenter.has_value()) {
        Vector3d data[3];
        for (int i = 0; i < 3; i++) {
            data[i].x = Vertices[i]->x;
            data[i].y = Vertices[i]->y;
            data[i].z = 1.0;
        }
        double num = Det(data);
        double num2 = -1.0 / (2.0 * num);
        for (int j = 0; j < 3; j++) {
            data[j].x = LengthSquared(Vertices[j]->x, Vertices[j]->y);
        }
        double num3 = 0.0 - Det(data);
        for (int k = 0; k < 3; k++) {
            data[k].y = Vertices[k]->x;
        }
        double num4 = Det(data);
        circumcenter.emplace((float)(num2 * num3), (float)(num2 * num4));
    }
    return circumcenter.value();
}

void ConvexHull::Create2D(std::vector<Vector2f> &vertices)
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
    Return2DResultInOrder(vertices);
}

void ConvexHull::Create(std::vector<Site> &vertices)
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
    GetHullVertices(vertices);
    GetConvexFaces(vertices, false, NumOfDimensions);
}

void ConvexHull::CreateDelaunay(std::vector<Site> &vertices)
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
    GetConvexFaces(vertices, true, NumOfDimensions);
}

void ConvexHull::GetHullVertices(std::vector<Site> &vertices)
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
void ConvexHull::GetConvexFaces(std::vector<T> &vertices, bool lift, int dim)
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
            cell.Vertices[j] = (Site *)&vertices[face.Vertices[j]];
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

void ConvexHull::Return2DResultInOrder(std::vector<Vector2f> &data)
{
    GetConvexFaces<Vector2f>(data, true, 2);
    int num = Faces.size();
    std::map<Vector2f *, ConvexFace *> dictionary;
    for (auto &val : Faces) {
        auto ptr = (Vector2f *)val.Vertices[1];
        dictionary.insert({ptr, &val});
    }
    auto val2 = (Vector2f *)(Faces[0].Vertices[1]);
    auto val3 = (Vector2f *)(Faces[0].Vertices[0]);
    std::vector<Vector2f *> list;
    list.push_back(val2);
    std::vector<ConvexFace *> list2;
    list2.push_back(&Faces[1]);
    int num2 = 0;
    int num3 = 0;
    while (val3 != val2) {
        list.push_back(val3);
        auto val4 = dictionary[val3];
        list2.push_back(val4);
        if (val3->x < list[num2]->x ||
            (val3->x == list[num2]->x && val3->y <= list[num2]->y)) {
            num2 = num3;
        }
        num3++;
        val3 = (Vector2f *)(val4->Vertices[0]);
    }
    Points.resize(num);
    Faces2D.resize(num);
    for (int j = 0; j < num; j++) {
        int index = (j + num2) % num;
        Points[j] = (Site *)(list[index]);
        Faces2D[j] = list2[index];
    }
}
