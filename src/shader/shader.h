#pragma once

#include "../header/header.h"
#include "../graphicLib/sampler.h"

//uniforms
extern Mat44 modelMatrix, viewMatrix, projectMatrix,
        lightProjectionMatrix, lightViewMatrix;
extern Vec4 lightDir, amb, diff, ambMat, diffMat;
extern Sampler *currTexture;
extern Sampler *depthTexture;

void vertexShader(const Vertex &input, VertexOut &output) noexcept;

void fragmentShader(const Fragment &input, FragmentOut &output) noexcept;

void simpleFragShader(const Fragment &input, FragmentOut &output) noexcept;

void storeVertShader(const Vertex &input, VertexOut &output) noexcept;

void storeFragShader(const Fragment &input, FragmentOut &output) noexcept;

