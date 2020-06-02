#include "util.h"

Mat44 lookAt(float eyeX, float eyeY, float eyeZ,
             float centerX, float centerY, float centerZ,
             float upX, float upY, float upZ) {
    Vec3 eye;
    eye.x = eyeX;
    eye.y = eyeY;
    eye.z = eyeZ;
    Vec3 center;
    center.x = centerX;
    center.y = centerY;
    center.z = centerZ;
    Vec3 up;
    up.x = upX;
    up.y = upY;
    up.z = upZ;

    Vec3 f = center - eye;
    f.Normalize();
    Vec3 s = f.CrossProduct(up);
    s.Normalize();
    Vec3 u = s.CrossProduct(f);
    u.Normalize();

    Mat44 mat;
    mat.LoadIdentity();
    mat.entries[0] = s.x;
    mat.entries[4] = s.y;
    mat.entries[8] = s.z;
    mat.entries[1] = u.x;
    mat.entries[5] = u.y;
    mat.entries[9] = u.z;
    mat.entries[2] = -f.x;
    mat.entries[6] = -f.y;
    mat.entries[10] = -f.z;
    mat.entries[12] = -(s.DotProduct(eye));
    mat.entries[13] = -(u.DotProduct(eye));
    mat.entries[14] = f.DotProduct(eye);
    return mat;
}

Mat44 perspective(float fovy, float aspect, float zNear, float zFar) {
    float rFovy = fovy * PI / 180;
    float tanHalfFovy = tanf(rFovy / 2);

    Mat44 mat;
    mat.LoadIdentity();
    mat.entries[0] = 1 / (aspect * tanHalfFovy);
    mat.entries[5] = 1 / tanHalfFovy;
    mat.entries[10] = -(zFar + zNear) / (zFar - zNear);
    mat.entries[11] = -1;
    mat.entries[14] = -2 * zFar * zNear / (zFar - zNear);
    mat.entries[15] = 0;
    return mat;
}

Mat44 ortho(float left, float right, float bottom, float top, float n, float f) {
    Mat44 mat;
    mat.LoadIdentity();
    mat.entries[0] = 2.0f / (right - left);
    mat.entries[5] = 2.0f / (top - bottom);
    mat.entries[10] = -2.0f / (f - n);
    mat.entries[12] = -(right + left) / (right - left);
    mat.entries[13] = -(top + bottom) / (top - bottom);
    mat.entries[14] = -(f + n) / (f - n);
    return mat;
}

int project(float objX, float objY, float objZ,
            const float *model, const float *proj, const int *view,
            float *winX, float *winY, float *winZ) {
    Vec4 point;
    point.x = objX;
    point.y = objY;
    point.z = objZ;
    point.w = 1.0f;

    Mat44 modelMat, projMat;
    modelMat = model;
    projMat = proj;
    point = modelMat * point;
    point = projMat * point;
    point /= point.w;

    Vec4 bias(0.5, 0.5, 0.5, 0.5);
    point = (point * 0.5);
    point = (point + bias);

    point.x = point.x * view[2] + view[0];
    point.y = point.y * view[3] + view[1];
    *winX = point.x;
    *winY = point.y;
    *winZ = point.z;
    return 1;
}

Mat44 rotateX(float angle) {
    float radian = angle * A2R;
    float sinA = sinf(radian);
    float cosA = cosf(radian);
    Mat44 mat;
    mat.LoadIdentity();
    mat.entries[5] = cosA;
    mat.entries[6] = sinA;
    mat.entries[9] = -sinA;
    mat.entries[10] = cosA;
    return mat;
}

Mat44 rotateY(float angle) {
    float radian = angle * A2R;
    float sinA = sinf(radian);
    float cosA = cosf(radian);
    Mat44 mat;
    mat.LoadIdentity();
    mat.entries[0] = cosA;
    mat.entries[2] = -sinA;
    mat.entries[8] = sinA;
    mat.entries[10] = cosA;
    return mat;
}

Mat44 rotateZ(float angle) {
    float radian = angle * A2R;
    float sinA = sinf(radian);
    float cosA = cosf(radian);
    Mat44 mat;
    mat.LoadIdentity();
    mat.entries[0] = cosA;
    mat.entries[1] = sinA;
    mat.entries[4] = -sinA;
    mat.entries[5] = cosA;
    return mat;
}

Mat44 scale(float size) {
    Mat44 mat;
    mat.LoadIdentity();
    mat.entries[0] = size;
    mat.entries[5] = size;
    mat.entries[10] = size;
    return mat;
}

Mat44 translate(float tx, float ty, float tz) {
    Mat44 mat;
    mat.LoadIdentity();
    mat.entries[12] = tx;
    mat.entries[13] = ty;
    mat.entries[14] = tz;
    return mat;
}

void interpolate3f(float pa, float pb, float pc,
                   float a, float b, float c,
                   float &result) {
    result = pa * a + pb * b + pc * c;
}

void interpolate2f(float pa, float pb,
                   float a, float b,
                   float &result) {
    result = pa * a + pb * b;
}

float calcZPara(float v1z, float v2z, float z) {
    return (z - v2z) / (v1z - v2z);
}

Vec3 calcParaEqu(Vec3 vect1, Vec3 vect2, float param) {
    Vec3 result;
    result = param * (vect1 - vect2) + vect2;
    return result;
}
