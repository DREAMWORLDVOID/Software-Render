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
    union { struct { float x, y, z, w; }; Vec4 Model; };
    union { struct { float nx, ny, nz; }; Vec3 Normal; };
    float s, t;

    Vertex(const Vertex& r) noexcept : Model(r.Model), Normal(r.Normal), s(r.s), t(r.t) {}

    Vertex() : x(0), y(0), z(0), w(1),
               nx(0), ny(0), nz(0),
               s(0), t(0) {}

    Vertex(float vx, float vy, float vz,
           float vnx, float vny, float vnz,
           float vs, float vt) :
            x(vx), y(vy), z(vz), w(1),
            nx(vnx), ny(vny), nz(vnz),
            s(vs), t(vt) {}
};

struct VertexOut {
    union { struct { float x, y, z, w; }; Vec4 Clip; };
    union { struct { float wx, wy, wz, ww; }; Vec4 World; };
    union { struct { float vx, vy, vz, vw;}; Vec4 View; };
    union { struct { float nx, ny, nz; }; Vec3 Normal; };
    float s, t;

    VertexOut() : Clip(0, 0, 0, 1),
                  World(0, 0, 0, 1),
                  View(0, 0, 0, 1),
                  Normal(0, 0, 0),
                  s(0), t(0) {}

    VertexOut(const VertexOut& r) noexcept: Clip(r.Clip), World(r.World), View(r.View), Normal(r.Normal), s(r.s), t(r.t) {}

};

struct Fragment {
    union { struct { float ndcX, ndcY, ndcZ; }; Vec3 Ndc; };
    union { struct { float wx, wy, wz, ww; }; Vec4 World; };
    union { struct { float nx, ny, nz; }; Vec3 Normal; };
    float s, t;

    Fragment() : ndcX(0), ndcY(0), ndcZ(1),
                 wx(0), wy(0), wz(0), ww(1),
                 nx(0), ny(0), nz(0),
                 s(0), t(0) {}
};

struct FragmentOut {
    union { struct { float r, g, b, a; }; Vec4 Color; };

    FragmentOut() : r(0), g(0), b(0), a(1) {}
};

typedef void (*VertexShader)(const Vertex &input, VertexOut &output);

typedef void (*FragmentShader)(const Fragment &input, FragmentOut &output);

typedef void (*DrawCall)();

