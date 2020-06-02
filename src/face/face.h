#pragma once

#include "header/constants.h"
#include "header/datatype.h"
#include "Maths/Maths.h"

class Face {
public:
    Vertex modelA, modelB, modelC;
    VertexOut clipA, clipB, clipC;
    Vec2 ndcA, ndcB, ndcC;
    Mat44 clipMatrixInv;

    Face(const Vertex& ma, const Vertex& mb, const Vertex& mc);

    Face();

    void copy2Face(Vertex a, Vertex b, Vertex c);

    void copy2FaceOut(VertexOut a, VertexOut b, VertexOut c);

    void calculateNDCVertex();

    void calculateClipMatrixInv();
};

