#include "shader.h"

Mat44 modelMatrix, viewMatrix, projectMatrix,
        lightProjectionMatrix, lightViewMatrix;
Vec4 lightDir, amb, diff, ambMat, diffMat;
Sampler *currTexture = nullptr;
Sampler *depthTexture = nullptr;

void vertexShader(const Vertex &input, VertexOut &output) noexcept {
    Vec4 modelNormal(input.nx, input.ny, input.nz, 0.0);
    Vec4 worldNormal = modelMatrix * modelNormal;
    output.World = modelMatrix * input.Model;
    output.View = viewMatrix * output.World;
    output.Clip = projectMatrix * output.View;
    output.Normal = worldNormal.Trim();
    output.s = input.s;
    output.t = input.t;
}

void fragmentShader(const Fragment &input, FragmentOut &output) noexcept {
    const auto worldNormal = input.Normal.GetNormalized();
    const auto worldLightDir = Vec3(lightDir.x, lightDir.y, lightDir.z).GetNormalized();
    const auto nDotL = max(worldLightDir.DotProduct(worldNormal), 0.0);

    Vec4 lightColor = amb * ambMat + (diff * diffMat) * nDotL;
    float shadowFactor = 1;
    Vec4 shadowVert = lightProjectionMatrix * lightViewMatrix * input.World;
    shadowVert *= (1.0f / shadowVert.w);
    float bias = 0.00001;
    if (shadowVert.x <= 1 && shadowVert.x >= -1 &&
        shadowVert.y <= 1 && shadowVert.y >= -1 &&
        shadowVert.z <= 1 && shadowVert.z >= -1) {
        shadowVert.x = 0.5f * (shadowVert.x + 1);
        shadowVert.y = 0.5f * (shadowVert.y + 1);
        shadowVert.z = 0.5f * (shadowVert.z + 1);
        float depthStore = depthTexture->texture2D(shadowVert.x, shadowVert.y).x;
        float depthNow = shadowVert.z + bias;
        if (depthNow > depthStore)
            shadowFactor = 0.5;
    }

    lightColor *= shadowFactor;
    Vec4 texColor(1, 1, 1, 1);
    if (currTexture != nullptr) texColor = currTexture->texture2D(input.s, input.t) * 1.2;
    output.Color = texColor * lightColor;
}

void simpleFragShader(const Fragment &input, FragmentOut &output) noexcept {
    const auto worldNormal = input.Normal.GetNormalized();
    const auto worldLightDir = Vec3(lightDir.x, lightDir.y, lightDir.z).GetNormalized();
    const auto nDotL = max(worldLightDir.DotProduct(worldNormal), 0.0);

    const auto lightColor = amb * ambMat + (diff * diffMat) * nDotL;
    const auto texColor = Vec4(1.2f, 1.2f, 0.0f, 0.0f);
    output.Color = lightColor * texColor + Vec4(0.0f, 0.0f, 0.0f, 0.6f);
}

void storeVertShader(const Vertex &input, VertexOut &output) noexcept {
    Vec4 modelNormal(input.nx, input.ny, input.nz, 0.0);
    Vec4 worldNormal = modelMatrix * modelNormal;
    output.World = modelMatrix * input.Model;
    output.View = lightViewMatrix * output.World;
    output.Clip = lightProjectionMatrix * output.View;
    output.Normal = worldNormal.Trim();
}

void storeFragShader(const Fragment &input, FragmentOut &output) noexcept {
    output.Color = Vec4(Sse::Vec4f(input.Ndc.GetZ() * 0.5f + 0.5f));
}
