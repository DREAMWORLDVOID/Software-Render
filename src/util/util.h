#ifndef UTIL_H_
#define UTIL_H_

#include "Maths/Maths.h"

#define PI 3.1415926
#define PI2 6.2831852
#define A2R 0.01745329222 //�Ƕ�ת����
#define R2A 57.2957804905 //����ת�Ƕ�

Mat44 lookAt(float eyeX, float eyeY, float eyeZ,
             float centerX, float centerY, float centerZ,
             float upX, float upY, float upZ);

Mat44 perspective(float fovy, float aspect, float zNear, float zFar);

Mat44 ortho(float left, float right, float bottom, float top, float n, float f);

int project(float objX, float objY, float objZ,
            const float *model, const float *proj, const int *view,
            float *winX, float *winY, float *winZ);

Mat44 rotateX(float angle);

Mat44 rotateY(float angle);

Mat44 rotateZ(float angle);

Mat44 scale(float size);

Mat44 translate(float tx, float ty, float tz);

Vec4 mul(Vec4 a, Vec4 b);

void interpolate3f(float pa, float pb, float pc,
                   float a, float b, float c,
                   float &result);

void interpolate2f(float pa, float pb,
                   float a, float b,
                   float &result);

float calcZPara(float v1z, float v2z, float z);

Vec3 calcParaEqu(Vec3 vect1, Vec3 vect2, float param);

inline float roundf(float number) {
    return number < 0.0 ? ceilf(number - 0.5) : floorf(number + 0.5);
}

#endif /* UTIL_H_ */
