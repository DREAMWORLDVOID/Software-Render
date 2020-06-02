#include "face.h"

Face::Face(const Vertex& ma, const Vertex& mb, const Vertex& mc) {
    modelA = ma;
    modelB = mb;
    modelC = mc;

    clipMatrixInv.LoadIdentity();
}

Face::Face() {
    clipMatrixInv.LoadIdentity();
}

void Face::calculateNDCVertex() {
    float invClipAW = 1.0f / clipA.Clip.GetW();
    float invClipBW = 1.0f / clipB.Clip.GetW();
    float invClipCW = 1.0f / clipC.Clip.GetW();
    ndcA.SetX(clipA.Clip.GetX() * invClipAW);
    ndcA.SetY(clipA.Clip.GetY() * invClipAW);
    ndcB.SetX(clipB.Clip.GetX() * invClipBW);
    ndcB.SetY(clipB.Clip.GetY() * invClipBW);
    ndcC.SetX(clipC.Clip.GetX() * invClipCW);
    ndcC.SetY(clipC.Clip.GetY() * invClipCW);
}

void Face::calculateClipMatrixInv() {
    const auto &cA = clipA.Clip;
    const auto &cB = clipB.Clip;
    const auto &cC = clipC.Clip;
    clipMatrixInv = Mat44(
            cA.GetX(), cA.GetY(), cA.GetW(), 0,
            cB.GetX(), cB.GetY(), cB.GetW(), 0,
            cC.GetX(), cC.GetY(), cC.GetW(), 0,
            0, 0, 0, 1
    );
    clipMatrixInv.Invert();
}

void Face::copy2Face(Vertex a, Vertex b, Vertex c) {
    modelA = a;
    modelB = b;
    modelC = c;
}

void Face::copy2FaceOut(VertexOut a, VertexOut b, VertexOut c) {
    clipA = a;
    clipB = b;
    clipC = c;
}

