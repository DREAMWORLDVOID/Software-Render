#include "sampler.h"

Sampler::Sampler(int sw, int sh) {
    width = sw;
    height = sh;
    imgData = new unsigned char[width * height * 3];
    memset(imgData, 0, width * height * 3 * sizeof(unsigned char));
}

Sampler::~Sampler() {
    delete[] imgData;
    printf("release sampler\n");
}

Vec4 Sampler::texture2D(float s, float t) {
    float u = (float) (width - 1) * s;
    float v = (float) (height - 1) * (1.0 - t);
    int iu = (int) u;
    int iv = (int) v;
    int uNext = iu + 1 <= (width - 1) ? iu + 1 : iu;
    int vNext = iv + 1 <= (height - 1) ? iv + 1 : iv;

    float uNextPer = u - iu;
    float vNextPer = v - iv;
    float uPer = 1.0f - uNextPer;
    float vPer = 1.0f - vNextPer;

    int imgIndex = 3 * (iv * width + iu);
    int imgIndexNextU = 3 * (iv * width + uNext);
    int imgIndexNextV = 3 * (vNext * width + iu);
    int imgIndexNextUV = 3 * (vNext * width + uNext);
    const auto color = Vec3{
            float(imgData[imgIndex]), float(imgData[imgIndex + 1]), float(imgData[imgIndex + 2])
    } * (uPer * vPer);

    const auto colorNextU = Vec3{
            float(imgData[imgIndexNextU]), float(imgData[imgIndexNextU + 1]), float(imgData[imgIndexNextU + 2])
    } * (uNextPer * vPer);

    const auto colorNextV = Vec3{
            float(imgData[imgIndexNextV]), float(imgData[imgIndexNextV + 1]), float(imgData[imgIndexNextV + 2])
    } * (uPer * vNextPer);

    const auto colorNextUV = Vec3{
            float(imgData[imgIndexNextUV]), float(imgData[imgIndexNextUV + 1]), float(imgData[imgIndexNextUV + 2])
    } * (uNextPer * vNextPer);
    return Vec4((color + colorNextU + colorNextV + colorNextUV) * INV_SCALE, 1);
}

void writeFrameBuffer2Sampler(FrameBuffer *fb, Sampler *sampler) {
    for (int i = 0; i < fb->height; i++) {
        for (int j = 0; j < fb->width; j++) {
            int index = (i * fb->width + j) * 3;
            sampler->imgData[((i) * fb->width + j) * 3] = fb->colorBuffer[index];
            sampler->imgData[((i) * fb->width + j) * 3 + 1] = fb->colorBuffer[index + 1];
            sampler->imgData[((i) * fb->width + j) * 3 + 2] = fb->colorBuffer[index + 2];
        }
    }
}
