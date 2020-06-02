#include "graphicLib.h"
#include "shader/shader.h"
#include <utility>
#include <array>

float eyeX, eyeY, eyeZ, clipNear;
Face *nFace1;
Face *nFace2;
bool blendFlag = false;
FrameBuffer *frontBuffer = nullptr;
FrameBuffer *backBuffer = nullptr;
FrameBuffer *frameBuffer1 = nullptr;
FrameBuffer *frameBuffer2 = nullptr;
DepthBuffer *depthBuffer = nullptr;
bool buffersReady = false;

unsigned char *screenBits = nullptr;

void initFrameBuffer(FrameBuffer **pfb, int width, int height) {
    *pfb = (FrameBuffer *) malloc(sizeof(FrameBuffer));
    (*pfb)->width = width;
    (*pfb)->height = height;
    (*pfb)->colorBuffer = new unsigned char[width * height * 3];
    memset((*pfb)->colorBuffer, 0, sizeof(unsigned char) * width * height * 3);
}

void releaseFrameBuffer(FrameBuffer **pfb) {
    if (*pfb == nullptr)
        return;
    delete[] (*pfb)->colorBuffer;
    free(*pfb);
    *pfb = nullptr;
}

void initDepthBuffer(DepthBuffer **pdb, int width, int height) {
    *pdb = (DepthBuffer *) malloc(sizeof(DepthBuffer));
    (*pdb)->width = width;
    (*pdb)->height = height;
    (*pdb)->depthBuffer = new float[width * height];
    memset((*pdb)->depthBuffer, 0, sizeof(float) * width * height);
}

void releaseDepthBuffer(DepthBuffer **pdb) {
    if (*pdb == NULL)
        return;
    delete[] (*pdb)->depthBuffer;
    free(*pdb);
    *pdb = NULL;
}

void initDevice(FrameBuffer **pfb, DepthBuffer **pdb, int width, int height) {
    initFrameBuffer(pfb, width, height);
    initDepthBuffer(pdb, width, height);
}

void releaseDevice(FrameBuffer **pfb, DepthBuffer **pdb) {
    releaseFrameBuffer(pfb);
    releaseDepthBuffer(pdb);
}

void initDevice2Buf(FrameBuffer **pfb1, FrameBuffer **pfb2, DepthBuffer **pdb, int width, int height) {
    initFrameBuffer(pfb1, width, height);
    initFrameBuffer(pfb2, width, height);
    initDepthBuffer(pdb, width, height);
    frontBuffer = *pfb1;
    backBuffer = *pfb2;
    buffersReady = false;
}

void releaseDevice2Buf(FrameBuffer **pfb1, FrameBuffer **pfb2, DepthBuffer **pdb) {
    frontBuffer = NULL;
    backBuffer = NULL;
    releaseFrameBuffer(pfb1);
    releaseFrameBuffer(pfb2);
    releaseDepthBuffer(pdb);
}

void clearScreen(FrameBuffer *fb, unsigned char red, unsigned char green, unsigned char blue) {
    for (int i = 0; i < fb->height; i++) {
        for (int j = 0; j < fb->width; j++) {
            int index = (i * fb->width + j) * 3;
            fb->colorBuffer[index] = red;
            fb->colorBuffer[index + 1] = green;
            fb->colorBuffer[index + 2] = blue;
        }
    }
}

void clearScreenFast(FrameBuffer *fb, unsigned char color) {
    int size = fb->width * fb->height * 3;
    memset(fb->colorBuffer, color * sizeof(unsigned char), size);
}

void clearDepth(DepthBuffer *db) {
    for (int i = 0; i < db->height; i++) {
        for (int j = 0; j < db->width; j++)
            db->depthBuffer[i * db->width + j] = 1.0;
    }
}

void flush(FrameBuffer *fb) {
    for (int i = 0; i < fb->height; i++) {
        for (int j = 0; j < fb->width; j++) {
            int index = (i * fb->width + j) * 3;
            unsigned char red = fb->colorBuffer[index];
            unsigned char green = fb->colorBuffer[index + 1];
            unsigned char blue = fb->colorBuffer[index + 2];
            screenBits[index] = blue;
            screenBits[index + 1] = green;
            screenBits[index + 2] = red;
        }
    }
}

void swapBuffer() {
    if (frontBuffer == frameBuffer1) {
        frontBuffer = frameBuffer2;
        backBuffer = frameBuffer1;
    } else {
        frontBuffer = frameBuffer1;
        backBuffer = frameBuffer2;
    }

    if (!buffersReady) {
        buffersReady = true;
        return;
    }
    FrameBuffer *fb = frontBuffer;
    for (int i = 0; i < fb->height; i++) {
        for (int j = 0; j < fb->width; j++) {
            int index = (i * fb->width + j) * 3;
            unsigned char red = fb->colorBuffer[index];
            unsigned char green = fb->colorBuffer[index + 1];
            unsigned char blue = fb->colorBuffer[index + 2];
            screenBits[index] = blue;
            screenBits[index + 1] = green;
            screenBits[index + 2] = red;
        }
    }
}

void convertToScreen(int height, int &sx, int &sy) {
    sy = height - 1 - sy;
}

void drawPixel(FrameBuffer *fb, int x, int y,
               unsigned char r, unsigned char g, unsigned char b) {
    convertToScreen(fb->height, x, y);
    int index = (y * fb->width + x) * 3;
    fb->colorBuffer[index] = r;
    fb->colorBuffer[index + 1] = g;
    fb->colorBuffer[index + 2] = b;
}

void readFrameBuffer(FrameBuffer *fb, int x, int y,
                     unsigned char &r, unsigned char &g, unsigned char &b) {
    convertToScreen(fb->height, x, y);
    int index = (y * fb->width + x) * 3;
    r = fb->colorBuffer[index];
    g = fb->colorBuffer[index + 1];
    b = fb->colorBuffer[index + 2];
}

void writeDepth(DepthBuffer *db, int x, int y, float depth) {
    convertToScreen(db->height, x, y);
    db->depthBuffer[y * db->width + x] = depth;
}

float readDepth(DepthBuffer *db, int x, int y) {
    convertToScreen(db->height, x, y);
    return db->depthBuffer[y * db->width + x];
}

void scaleColor(const Vec3& color, unsigned char &iRed, unsigned char &iGreen, unsigned char &iBlue) {
    const auto scaled = color * 255.0f;
    iRed = min(255, int(scaled.GetX()));
    iGreen = min(255, int(scaled.GetY()));
    iBlue = min(255, int(scaled.GetZ()));
}

void viewPortTransform(float ndcX, float ndcY, float width, float height,
                       int &screenX, int &screenY) {
    float biasX = (ndcX + 1.0) * 0.5;
    float biasY = (ndcY + 1.0) * 0.5;
    screenX = roundf(biasX * (width - 1.0));
    screenY = roundf(biasY * (height - 1.0));
}

void viewPortTransform(float ndcX, float ndcY, float width, float height,
                       float &screenX, float &screenY) {
    float biasX = (ndcX + 1.0) * 0.5;
    float biasY = (ndcY + 1.0) * 0.5;
    screenX = biasX * (width - 1.0);
    screenY = biasY * (height - 1.0);
}

void invViewPortTransform(int screenX, int screenY, float width, float height,
                          float &ndcX, float &ndcY) {
    float scaleX = (float) screenX / (width - 1.0);
    float scaleY = (float) screenY / (height - 1.0);
    ndcX = scaleX * 2.0 - 1.0;
    ndcY = scaleY * 2.0 - 1.0;
}

void blend(unsigned char srcR, unsigned char srcG, unsigned char srcB, float srcA,
           unsigned char dstR, unsigned char dstG, unsigned char dstB,
           unsigned char &finalR, unsigned char &finalG, unsigned char &finalB) {
    float dstA = 1.0 - srcA;
    float finR = (float) srcR * srcA + (float) dstR * dstA;
    float finG = (float) srcG * srcA + (float) dstG * dstA;
    float finB = (float) srcB * srcA + (float) dstB * dstA;
    finalR = finR > 255 ? 255 : finR;
    finalG = finG > 255 ? 255 : finG;
    finalB = finB > 255 ? 255 : finB;
}

void calcBounds(float scrAX, float scrAY, float scrBX, float scrBY, float scrCX, float scrCY,
                float scrY, float &x1, float &x2) {
    if (scrAY == scrBY) {
        float paramAC = (scrY - scrAY) / (scrCY - scrAY);
        float paramBC = (scrY - scrBY) / (scrCY - scrBY);
        x1 = (scrCX - scrAX) * paramAC + scrAX;
        x2 = (scrCX - scrBX) * paramBC + scrBX;
    } else if (scrAY == scrCY) {
        float paramAB = (scrY - scrAY) / (scrBY - scrAY);
        float paramBC = (scrY - scrBY) / (scrCY - scrBY);
        x1 = (scrBX - scrAX) * paramAB + scrAX;
        x2 = (scrCX - scrBX) * paramBC + scrBX;
    } else if (scrBY == scrCY) {
        float paramAB = (scrY - scrAY) / (scrBY - scrAY);
        float paramAC = (scrY - scrAY) / (scrCY - scrAY);
        x1 = (scrBX - scrAX) * paramAB + scrAX;
        x2 = (scrCX - scrAX) * paramAC + scrAX;
    } else {
        float paramAB = (scrY - scrAY) / (scrBY - scrAY);
        float paramAC = (scrY - scrAY) / (scrCY - scrAY);
        float paramBC = (scrY - scrBY) / (scrCY - scrBY);
        bool ab = paramAB <= 1.0 && paramAB >= 0.0;
        bool ac = paramAC <= 1.0 && paramAC >= 0.0;
        bool bc = paramBC <= 1.0 && paramBC >= 0.0;
        float xAB = (scrBX - scrAX) * paramAB + scrAX;
        float xAC = (scrCX - scrAX) * paramAC + scrAX;
        float xBC = (scrCX - scrBX) * paramBC + scrBX;
        if (!ab) {
            x1 = xAC;
            x2 = xBC;
        } else if (!ac) {
            x1 = xAB;
            x2 = xBC;
        } else if (!bc) {
            x1 = xAB;
            x2 = xAC;
        } else {
            x1 = xAC;
            x2 = xBC;
            if (x1 == x2)
                x2 = xAB;
        }
    }
}

auto GetUnits(float width, float height, const Face &face) noexcept {
    float baseX, baseY, oX, oY;
    invViewPortTransform(0, 0, width, height, baseX, baseY);
    invViewPortTransform(1, 1, width, height, oX, oY);
    Vec4 x = face.clipMatrixInv * Vec4{oX - baseX, 0, 0, 0}; // proportion 4D
    Vec4 y = face.clipMatrixInv * Vec4{0, oY - baseY, 0, 0}; // proportion 4D
    Vec4 z = face.clipMatrixInv * Vec4{baseX, baseY, 1, 0}; // proportion 4D
    return std::array<Vec3, 3> { x.Trim(), y.Trim(), z.Trim() };
}

void rasterize2(FrameBuffer *fb, DepthBuffer *db, FragmentShader fs, const Face *face) {
    float ndcX = 0, ndcY = 0;
    float scrAX, scrAY, scrBX, scrBY, scrCX, scrCY;
    viewPortTransform(face->ndcA.x, face->ndcA.y, fb->width, fb->height, scrAX, scrAY);
    viewPortTransform(face->ndcB.x, face->ndcB.y, fb->width, fb->height, scrBX, scrBY);
    viewPortTransform(face->ndcC.x, face->ndcC.y, fb->width, fb->height, scrCX, scrCY);
    if (scrAY == scrBY && scrAY == scrCY) return;
    int minY = max(0, min(scrAY, min(scrBY, scrCY)));
    int maxY = min(fb->height - 1, max(scrAY, max(scrBY, scrCY)));
    auto&& [uX, uY, uB] = GetUnits(fb->width, fb->height, *face);
    auto baseY = uB + uY * float(minY);
    for (int scrY = minY; scrY <= maxY; (baseY += uY, scrY++)) {
        float x1, x2;
        calcBounds(scrAX, scrAY, scrBX, scrBY, scrCX, scrCY, (float) scrY, x1, x2);
        int minX = max(0, roundf(min(x1, x2)));
        int maxX = min(fb->width - 1, roundf(max(x1, x2)));
        auto pFrag0 = baseY + uX * float(minX); // proportion fragment
        for (int scrX = minX; scrX <= maxX; (pFrag0 += uX, scrX++)) {
            if (pFrag0.Sse().sign_bits()) continue;
            float sum = pFrag0.GetX() + pFrag0.GetY() + pFrag0.GetZ();
            const auto pFrag = pFrag0 * (1.0f / sum);

            const auto& cA = face->clipA;
            const auto& cB = face->clipB;
            const auto& cC = face->clipC;
            // NDC Check
            const auto ndcRaw = cA.Clip * pFrag.GetX() + cB.Clip * pFrag.GetY() + cC.Clip * pFrag.GetZ();
            const auto ndc = ndcRaw * (1.0f / ndcRaw.GetW());

            if (ndc.GetZ() < -1.0f || ndc.GetZ() > 1.0f) continue;

            // early depth
            if (db != nullptr) {
                float storeZ = readDepth(db, scrX, scrY);
                if (storeZ < ndc.GetZ()) continue;
                writeDepth(db, scrX, scrY, ndc.GetZ());
            }

            Fragment frag;
            frag.Ndc = ndc.Trim();
            frag.World = cA.World * pFrag.GetX() + cB.World * pFrag.GetY() + cC.World * pFrag.GetZ();
            frag.Normal = cA.Normal * pFrag.GetX() + cB.Normal * pFrag.GetY() + cC.Normal * pFrag.GetZ();
            frag.s = pFrag.DotProduct({face->clipA.s, face->clipB.s, face->clipC.s});
            frag.t = pFrag.DotProduct({face->clipA.t, face->clipB.t, face->clipC.t});

            FragmentOut outFrag;
            fs(frag, outFrag);
            unsigned char cr = 255, cg = 255, cb = 255, sr = 255, sg = 255, sb = 255;
            scaleColor(outFrag.Color.Trim(), cr, cg, cb);
            if (blendFlag) {
                readFrameBuffer(fb, scrX, scrY, sr, sg, sb);
                blend(cr, cg, cb, outFrag.Color.GetW(), sr, sg, sb, cr, cg, cb);
            }
            drawPixel(fb, scrX, scrY, cr, cg, cb);
        }
    }
}

bool cullFace(Face *face, int flag) {
    Vec3 faceNormal = face->clipA.Normal;
    Vec3 eyeVec = Vec3(eyeX, eyeY, eyeZ) - face->clipA.World.Trim();
    if (flag == CULL_NONE) return false;
    if (flag == CULL_BACK) return eyeVec.DotProduct(faceNormal) <= 0;
    if (flag == CULL_FRONT) return eyeVec.DotProduct(faceNormal) >= 0;
    return false;
}

void drawFace(FrameBuffer *fb, DepthBuffer *db, VertexShader vs, FragmentShader fs, int cullFlag, Face *face) {
    vs(face->modelA, face->clipA);
    vs(face->modelB, face->clipB);
    vs(face->modelC, face->clipC);
    if (cullFace(face, cullFlag))
        return;
    int clipFlag = checkFace(face);
    if (clipFlag != 0b111) {
        if (clipFlag == 0b000)
            return;
        fixFaces(face, clipFlag);
        if (!cullFace(nFace1, cullFlag)) {
            nFace1->calculateClipMatrixInv();
            nFace1->calculateNDCVertex();
            rasterize2(fb, db, fs, nFace1);
        }
        if (clipFlag == 0b011 || clipFlag == 0b101 || clipFlag == 0b110) {
            if (!cullFace(nFace2, cullFlag)) {
                nFace2->calculateClipMatrixInv();
                nFace2->calculateNDCVertex();
                rasterize2(fb, db, fs, nFace2);
            }
        }
    } else if (clipFlag == 0b111) {
        face->calculateClipMatrixInv();
        face->calculateNDCVertex();
        rasterize2(fb, db, fs, face);
    }
}

void drawFaces(FrameBuffer *fb, DepthBuffer *db, VertexShader vs, FragmentShader fs, int cullFlag, Vertex *buffer,
               int count) {
    for (int i = 0; i < count; i++) {
        Face face(buffer[i * 3], buffer[i * 3 + 1], buffer[i * 3 + 2]);
        drawFace(fb, db, vs, fs, cullFlag, &face);
    }
}

int checkFace(Face *face) {
    auto flags = 0b111u;
    if (face->clipA.View.GetZ() / face->clipA.View.GetW() > -clipNear) flags&=0b011u;
    if (face->clipB.View.GetZ() / face->clipB.View.GetW() > -clipNear) flags&=0b101u;
    if (face->clipC.View.GetZ() / face->clipC.View.GetW() > -clipNear) flags&=0b110u;
    return flags;
}

void initFixFace() {
    nFace1 = new Face();
    nFace2 = new Face();
}

void releaseFixFace() {
    delete nFace1;
    delete nFace2;
}

void fixFaces(Face *face, int fixFlag) {
    switch (fixFlag) {
        case 0b011:return fix1FailFace(face->clipA, face->clipB, face->clipC);
        case 0b101:return fix1FailFace(face->clipB, face->clipA, face->clipC);
        case 0b110:return fix1FailFace(face->clipC, face->clipA, face->clipB);
        case 0b001:return fix2FailFace(face->clipA, face->clipB, face->clipC);
        case 0b010:return fix2FailFace(face->clipA, face->clipC, face->clipB);
        case 0b100:return fix2FailFace(face->clipB, face->clipC, face->clipA);
        default: return;
    }
}

void interpolate2v(float pa, float pb,
                   const VertexOut& a, const VertexOut& b,
                   VertexOut &result) {
    result.Clip = a.Clip * pa + b.Clip * pb;
    result.World = a.World * pa + b.World * pb;
    result.View = a.View * pa + b.View * pb;
    result.Normal = a.Normal * pa + b.Normal * pb;
    interpolate2f(pa, pb, a.s, b.s, result.s);
    interpolate2f(pa, pb, a.t, b.t, result.t);
}

void fix1FailFace(const VertexOut& fail, const VertexOut& succ1, const VertexOut& succ2) {
    float z = -clipNear;
    Vec3 pFail = fail.View.Trim() * (1.0f / fail.View.GetW());
    Vec3 pSucc1 = succ1.View.Trim() * (1.0f / succ1.View.GetW());
    Vec3 pSucc2 = succ2.View.Trim() * (1.0f / succ2.View.GetW());
    float param1 = calcZPara(pFail.z, pSucc1.z, z);
    Vec3 interPoint1 = calcParaEqu(pFail, pSucc1, param1);
    float sp = (pFail - interPoint1).GetLength();
    float fp = (pSucc1 - interPoint1).GetLength();
    float sum = sp + fp;
    float invSum = 1.0 / sum;
    sp *= invSum;
    fp *= invSum;
    VertexOut inter1;
    interpolate2v(sp, fp, succ1, fail, inter1);

    float param2 = calcZPara(pFail.z, pSucc2.z, z);
    Vec3 interPoint2 = calcParaEqu(pFail, pSucc2, param2);
    sp = (pFail - interPoint2).GetLength();
    fp = (pSucc2 - interPoint2).GetLength();
    sum = sp + fp;
    invSum = 1.0 / sum;
    sp *= invSum;
    fp *= invSum;
    VertexOut inter2;
    interpolate2v(sp, fp, succ2, fail, inter2);

    nFace1->copy2FaceOut(succ1, inter1, inter2);
    nFace2->copy2FaceOut(succ2, succ1, inter2);
}

void fix2FailFace(const VertexOut& fail1, const VertexOut& fail2, const VertexOut& succ) {
    float z = -clipNear;
    Vec3 pFail1 = fail1.View.Trim() * (1.0f / fail1.View.GetW());
    Vec3 pFail2 = fail2.View.Trim() * (1.0f / fail2.View.GetW());
    Vec3 pSucc = succ.View.Trim() * (1.0f / succ.View.GetW());
    float param1 = calcZPara(pFail1.z, pSucc.z, z);
    Vec3 interPoint1 = calcParaEqu(pFail1, pSucc, param1);
    float sp = (pFail1 - interPoint1).GetLength();
    float fp = (pSucc - interPoint1).GetLength();
    float sum = sp + fp;
    float invSum = 1.0 / sum;
    sp *= invSum;
    fp *= invSum;
    VertexOut inter1;
    interpolate2v(sp, fp, succ, fail1, inter1);

    float param2 = calcZPara(pFail2.z, pSucc.z, z);
    Vec3 interPoint2 = calcParaEqu(pFail2, pSucc, param2);
    sp = (pFail2 - interPoint2).GetLength();
    fp = (pSucc - interPoint2).GetLength();
    sum = sp + fp;
    invSum = 1.0 / sum;
    sp *= invSum;
    fp *= invSum;
    VertexOut inter2;
    interpolate2v(sp, fp, succ, fail2, inter2);

    nFace1->copy2FaceOut(succ, inter1, inter2);
}
