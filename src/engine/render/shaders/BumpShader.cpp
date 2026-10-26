#include "engine/render/shaders/BumpShader.h"
#include "engine/render/Engine.h"
#include "engine/render/Mesh.h"
#include <GLES2/gl2.h>


static const char *const kBumpShaderVertexSource =
    "attribute highp vec3 \ta_position;  \n"
    "attribute highp vec2  a_texCoord;    \n"
    "attribute highp vec3  \ta_normal; \t  \n"
    "varying highp vec2 v_texCoord;  \n"
    "varying highp vec3 v_normal;   \n"
    "uniform highp mat4 u_ModelViewProjectionMatrix;  \n"
    "uniform mediump mat3 u_ModelMatrix;  \n"
    "uniform highp float u_fogMaxDist; \n"
    "uniform highp float u_fogMinDist;\n"
    "uniform highp vec3 u_eyeposmodel;  \n"
    "varying lowp float v_FogFactor;\n"
    "varying vec3 DiffuseColor; \n"
    "const float C1 = 0.429043;\n"
    "const float C2 = 0.511664;\n"
    "const float C3 = 0.743125;\n"
    "const float C4 = 0.886227;\n"
    "const float C5 = 0.247708;\n"
    "const vec3 L00  = vec3( 0.871297, 0.875222, 0.864470 );\n"
    "const vec3 L1m1 = vec3( 0.175058, 0.245335, 0.312891 );\n"
    "const vec3 L10  = vec3( 0.034675, 0.036107, 0.037362 );\n"
    "const vec3 L11  = vec3(-0.004629,-0.029448,-0.048028 );\n"
    "const vec3 L2m2 = vec3(-0.120535,-0.121160,-0.117507 );\n"
    "const vec3 L2m1 = vec3( 0.003242, 0.003624, 0.007511 );\n"
    "const vec3 L20  = vec3(-0.028667,-0.024926,-0.020998 );\n"
    "const vec3 L21  = vec3(-0.077539,-0.086325,-0.091591 );\n"
    "const vec3 L22  = vec3(-0.161784,-0.191783,-0.219152 );\n"
    "lowp float computeLinearFogFactor(highp float eyeDist) { \n"
    "    highp float factor; \n"
    "    // Compute linear fog equation \n"
    "    factor = (u_fogMaxDist - eyeDist) / (u_fogMaxDist - u_fogMinDist);\n"
    "    // Clamp in the [0,1] range \n"
    "    factor = (1.0 - clamp(factor, 0.0, 1.0));\n"
    "    return factor; \n"
    "}    \n"
    "void main()  \n"
    "{  \n"
    "\tgl_Position = u_ModelViewProjectionMatrix * vec4(a_position, 1.0);  \n"
    "\tv_texCoord  = a_texCoord;    \n"
    "   highp float eyeDist = distance(u_eyeposmodel, a_position); \n"
    "   v_FogFactor = computeLinearFogFactor(eyeDist); \n"
    "   vec3 tnorm = normalize(u_ModelMatrix*a_normal);\n"
    "   DiffuseColor = C1 * L22 * (tnorm.x * tnorm.x - tnorm.y * tnorm.y) + \n"
    "                  C3 * L20 * tnorm.z * tnorm.z +\n"
    "                  C4 * L00 - \n"
    "                  C5 * L20 + \n"
    "                  2.0 * C1 * L2m2 * tnorm.x * tnorm.y +\n"
    "                  2.0 * C1 * L21  * tnorm.x * tnorm.z +\n"
    "                  2.0 * C1 * L2m1 * tnorm.y * tnorm.z +\n"
    "                  2.0 * C2 * L11  * tnorm.x +\n"
    "                  2.0 * C2 * L1m1 * tnorm.y +\n"
    "                  2.0 * C2 * L10 * tnorm.z;\n"
    "   v_normal = a_normal; \n"
    "}  \n";

static const char *const kBumpShaderFragmentSource =
    "precision lowp float;  \n"
    "varying highp vec2 v_texCoord;  \n"
    "varying highp vec3 v_normal;   \n"
    "varying vec3 DiffuseColor; \n"
    "uniform sampler2D  s_texture;  \n"
    "uniform lowp vec3  u_AmbientColor[2];  \n"
    "uniform lowp vec3 u_DiffuseColor[2];\n"
    "uniform lowp vec3 u_SpecularColor[2];\n"
    "uniform mediump float u_SpecularPower;\n"
    "uniform highp float u_TexBiasDiffuse; \n"
    "uniform highp float u_TexBiasNormal; \n"
    "uniform highp vec3 u_lightdirmodel[2];  \n"
    "uniform highp vec3 u_eyeposmodel;  \n"
    "uniform bool u_EnableFog; \n"
    "uniform vec3 u_fogColor; \n"
    "varying lowp float v_FogFactor;\n"
    "float specularIntensity1; \n"
    "float specularIntensity2; \n"
    "lowp float diffuseIntensity1; \n"
    "lowp float diffuseIntensity2; \n"
    "vec3 outColor; \n"
    "lowp vec4 colorTex;\n"
    "lowp vec4 normalTex;\n"
    "highp vec3 reflection;\n"
    "void main()  \n"
    "{  \n"
    "\tcolorTex   = texture2D( s_texture, v_texCoord, u_TexBiasDiffuse );  \n"
    "\tdiffuseIntensity1 = clamp(dot(v_normal, u_lightdirmodel[0]), 0.0, 1.0);\n"
    "   reflection = normalize(-reflect(u_lightdirmodel[0], v_normal)); \n"
    "\tspecularIntensity1 = pow( clamp(dot(reflection, normalize(u_eyeposmodel)), 0.0, 1.0), u_SpecularPower );  \n"
    "\tdiffuseIntensity2 = clamp(dot(v_normal, u_lightdirmodel[1]), 0.0, 1.0);\n"
    "   reflection = normalize(-reflect(u_lightdirmodel[1], v_normal)); \n"
    "\tspecularIntensity2 = pow( clamp(dot(reflection, normalize(u_eyeposmodel)), 0.0, 1.0), u_SpecularPower );  \n"
    "   outColor = (u_AmbientColor[0] + u_AmbientColor[1] + diffuseIntensity1*u_DiffuseColor[0] + diffuseIntensity2*u_DiffuseColor[1])* colorTex.rgb  + (specularIntensity1 * u_SpecularColor[0] + specularIntensity2 * u_SpecularColor[1])*0.2;  \n"
    "   //outColor = (u_AmbientColor[0] + u_AmbientColor[1] + diffuseIntensity1*u_DiffuseColor[0] + diffuseIntensity2*u_DiffuseColor[1])* colorTex.rgb; \n"
    "   if ( u_EnableFog ) { \n"
    "       gl_FragColor.rgb = mix(outColor, u_fogColor, v_FogFactor); \n"
    "       gl_FragColor.a = colorTex.a;\n"
    "   } else { \n"
    "       gl_FragColor.rgb = outColor; \n"
    "       gl_FragColor.a = colorTex.a;\n"
    "   } \n"
    "}  \n";

namespace AbyssEngine {
    int BumpShader::ShaderIndex = -1;

    BumpShader::BumpShader() {
        BumpShader::ShaderIndex = ShaderBaseStruct::shaderIndexIntern;
        this->name = String("BumpShader", false);
    }

    void BumpShader::SetInActive() {
        if (this->aPositionLoc >= 0)
            glDisableVertexAttribArray(this->aPositionLoc);
        if (this->aTexCoordLoc >= 0)
            glDisableVertexAttribArray(this->aTexCoordLoc);
        if (this->aNormalLoc >= 0)
            glDisableVertexAttribArray(this->aNormalLoc);
    }

    void BumpShader::Init(Engine *) {
        this->program = this->ES2LoadProgram(kBumpShaderVertexSource, kBumpShaderFragmentSource);

        this->aPositionLoc = glGetAttribLocation(this->program, "a_position");
        this->aTexCoordLoc = glGetAttribLocation(this->program, "a_texCoord");
        this->aNormalLoc = glGetAttribLocation(this->program, "a_normal");

        this->uModelViewProjectionMatrixLoc = glGetUniformLocation(this->program, "u_ModelViewProjectionMatrix");
        this->uModelMatrixLoc = glGetUniformLocation(this->program, "u_ModelMatrix");
        this->uLightdirmodel0Loc = glGetUniformLocation(this->program, "u_lightdirmodel[0]");
        this->uLightdirmodel1Loc = glGetUniformLocation(this->program, "u_lightdirmodel[1]");
        this->uEyeposmodelLoc = glGetUniformLocation(this->program, "u_eyeposmodel");
        this->sTextureLoc = glGetUniformLocation(this->program, "s_texture");
        this->uTextureCubemapLoc = glGetUniformLocation(this->program, "u_texture_cubemap");
        this->uGlColorLoc = glGetUniformLocation(this->program, "glColor");
        this->uAmbientColor0Loc = glGetUniformLocation(this->program, "u_AmbientColor[0]");
        this->uDiffuseColor0Loc = glGetUniformLocation(this->program, "u_DiffuseColor[0]");
        this->uSpecularColor0Loc = glGetUniformLocation(this->program, "u_SpecularColor[0]");
        this->uAmbientColor1Loc = glGetUniformLocation(this->program, "u_AmbientColor[1]");
        this->uDiffuseColor1Loc = glGetUniformLocation(this->program, "u_DiffuseColor[1]");
        this->uSpecularColor1Loc = glGetUniformLocation(this->program, "u_SpecularColor[1]");
        this->uSpecularPowerLoc = glGetUniformLocation(this->program, "u_SpecularPower");
        this->uRimColorLoc = glGetUniformLocation(this->program, "u_RimColor");
        this->uTexBiasDiffuseLoc = glGetUniformLocation(this->program, "u_TexBiasDiffuse");
        this->uTexBiasNormalLoc = glGetUniformLocation(this->program, "u_TexBiasNormal");
        this->uFogColorLoc = glGetUniformLocation(this->program, "u_fogColor");
        this->uFogMaxDistLoc = glGetUniformLocation(this->program, "u_fogMaxDist");
        this->uFogMinDistLoc = glGetUniformLocation(this->program, "u_fogMinDist");
        this->uEnableFogLoc = glGetUniformLocation(this->program, "u_EnableFog");

        glUseProgram(this->program);
        glUniform1i(this->sTextureLoc, 0);
        glUniform1i(this->uTextureCubemapLoc, 7);
    }

    void BumpShader::UpdateMeshData(Mesh *mesh, Engine *ctx) {
        if (this->uModelViewProjectionMatrixLoc >= 0)
            glUniformMatrix4fv(this->uModelViewProjectionMatrixLoc, 1, 0, ctx->worldViewProjMatrix);
        if (this->uModelMatrixLoc >= 0)
            glUniformMatrix3fv(this->uModelMatrixLoc, 1, 0, ctx->normalMatrix);
        if (this->uTexBiasDiffuseLoc >= 0)
            glUniform1f(this->uTexBiasDiffuseLoc, Engine::lodBiasDiffuse);
        if (this->uTexBiasNormalLoc >= 0)
            glUniform1f(this->uTexBiasNormalLoc, Engine::lodBiasNormal);

        if (this->dirty != 0) {
            glUniform3f(this->uLightdirmodel0Loc, ctx->lightDir[0].x, ctx->lightDir[0].y, ctx->lightDir[0].z);
            if (this->uEyeposmodelLoc >= 0)
                glUniform3f(this->uEyeposmodelLoc, ctx->lightColor.x, ctx->lightColor.y, ctx->lightColor.z);
            if (this->uGlColorLoc >= 0)
                glUniform4fv(this->uGlColorLoc, 1, ctx->glColor);
            if (this->uAmbientColor0Loc >= 0)
                glUniform3fv(this->uAmbientColor0Loc, 1, (const float *) ctx->ambientShaded[0]);
            if (this->uDiffuseColor0Loc >= 0)
                glUniform3fv(this->uDiffuseColor0Loc, 1, (const float *) ctx->diffuseShaded[0]);
            if (this->uSpecularColor0Loc >= 0)
                glUniform3fv(this->uSpecularColor0Loc, 1, (const float *) ctx->specularShaded[0]);
            if (this->uSpecularPowerLoc >= 0)
                glUniform1f(this->uSpecularPowerLoc, ctx->materialShininess);
            if (this->uRimColorLoc >= 0)
                glUniform3fv(this->uRimColorLoc, 1, (const float *) ctx->rimColor);
            if (this->uFogColorLoc >= 0)
                glUniform3fv(this->uFogColorLoc, 1, ctx->fogColor);
            if (this->uFogMinDistLoc >= 0)
                glUniform1f(this->uFogMinDistLoc, ctx->fogMinDist);
            if (this->uFogMaxDistLoc >= 0)
                glUniform1f(this->uFogMaxDistLoc, ctx->fogMaxDist);
            if (this->uEnableFogLoc >= 0)
                glUniform1i(this->uEnableFogLoc, AbyssEngine::Engine::fogEnabled);

            if (ctx->lightCount >= 2) {
                glUniform3fv(this->uAmbientColor1Loc, 1, (const float *) ctx->ambientShaded[1]);
                glUniform3fv(this->uDiffuseColor1Loc, 1, (const float *) ctx->diffuseShaded[1]);
                glUniform3fv(this->uSpecularColor1Loc, 1, (const float *) ctx->specularShaded[1]);
                glUniform3f(this->uLightdirmodel1Loc, ctx->lightDir[1].x, ctx->lightDir[1].y,
                            ctx->lightDir[1].z);
            } else {
                glUniform3f(this->uAmbientColor1Loc, 0, 0, 0);
                glUniform3f(this->uDiffuseColor1Loc, 0, 0, 0);
                glUniform3f(this->uSpecularColor1Loc, 0, 0, 0);
                glUniform3f(this->uLightdirmodel1Loc, ctx->lightDir[1].x, ctx->lightDir[1].y,
                            ctx->lightDir[1].z);
            }
            this->dirty = 0;
        }

        if (this->aPositionLoc >= 0)
            glEnableVertexAttribArray(this->aPositionLoc);
        if (this->aTexCoordLoc >= 0)
            glEnableVertexAttribArray(this->aTexCoordLoc);
        if (this->aNormalLoc >= 0)
            glEnableVertexAttribArray(this->aNormalLoc);

        if (mesh->uploaded == 0) {
            if (this->aPositionLoc >= 0)
                glVertexAttribPointer(this->aPositionLoc, 3, 0x1406, 0, 0, mesh->positions);
            if (this->aTexCoordLoc >= 0)
                glVertexAttribPointer(this->aTexCoordLoc, 2, 0x1406, 0, 0, mesh->texCoords);
            if (this->aNormalLoc >= 0)
                glVertexAttribPointer(this->aNormalLoc, 3, 0x1406, 0, 0, mesh->normals);
        } else {
            glBindBuffer(0x8892, mesh->positionVBO);
            glVertexAttribPointer(this->aPositionLoc, 3, 0x1406, 0, 0, 0);
            glBindBuffer(0x8892, mesh->texCoordVBO);
            glVertexAttribPointer(this->aTexCoordLoc, 2, 0x1406, 0, 0, 0);
            glBindBuffer(0x8892, mesh->normalVBO);
            glVertexAttribPointer(this->aNormalLoc, 3, 0x1406, 0, 0, 0);
        }
    }
}
