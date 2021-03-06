#pragma once

#include "Maths/Vec4.h"

struct FrameBuffer {
    unsigned char *colorBuffer;
    int width, height;
};

struct DepthBuffer {
    float *depthBuffer;
    int width, height;
};

struct Vertex {
    Vec4 Model;
    Vec3 Normal;
    float s, t;

    Vertex() noexcept : Model(0, 0, 0, 1),Normal(0, 0, 0), s(0), t(0) {}

    Vertex(float vx, float vy, float vz,
           float vnx, float vny, float vnz,
           float vs, float vt) :
            Model(vx, vy, vz, 1),
            Normal(vnx, vny, vnz),
            s(vs), t(vt) {}
};

struct VertexOut {
    Vec4 Clip;
    Vec4 World;
    Vec4 View;
    Vec3 Normal;
    float s, t;

    VertexOut() : Clip(0, 0, 0, 1),
                  World(0, 0, 0, 1),
                  View(0, 0, 0, 1),
                  Normal(0, 0, 0),
                  s(0), t(0) {}
};

struct Fragment {
    union { struct { float ndcX, ndcY, ndcZ; }; Vec3 Ndc; };
    Vec4 World;
    Vec3 Normal;
    float s, t;

    Fragment() : ndcX(0), ndcY(0), ndcZ(1),
                 World(0, 0, 0, 1),
                 Normal(0, 0, 0),
                 s(0), t(0) {}
};

struct FragmentOut {
    Vec4 Color;

    FragmentOut() : Color(0, 0, 0, 1) {}
};

using VertexShader = void (*)(const Vertex &input, VertexOut &output) noexcept;

using FragmentShader = void (*)(const Fragment &input, FragmentOut &output) noexcept;

using DrawCall = void (*)();

