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
    int BumpShader::ShaderIndex;

    BumpShader::BumpShader() {
        BumpShader::ShaderIndex = ShaderBaseStruct::shaderIndexIntern;
        this->name = String("BumpShader", false);
    }

    void BumpShader::SetInActive() {
        if (this->a0Loc >= 0)
            glDisableVertexAttribArray(this->a0Loc);
        if (this->a1Loc >= 0)
            glDisableVertexAttribArray(this->a1Loc);
        if (this->a2Loc >= 0)
            glDisableVertexAttribArray(this->a2Loc);
    }

    void BumpShader::Init(Engine *) {
        this->program = this->ES2LoadProgram(kBumpShaderVertexSource, kBumpShaderFragmentSource);

        this->a0Loc = glGetAttribLocation(this->program, "a_position");
        this->a1Loc = glGetAttribLocation(this->program, "a_texCoord");
        this->a2Loc = glGetAttribLocation(this->program, "a_normal");

        this->u0Loc = glGetUniformLocation(this->program, "u_ModelViewProjectionMatrix");
        this->u1Loc = glGetUniformLocation(this->program, "u_ModelMatrix");
        this->u2Loc = glGetUniformLocation(this->program, "u_lightdirmodel[0]");
        this->u3Loc = glGetUniformLocation(this->program, "u_lightdirmodel[1]");
        this->u4Loc = glGetUniformLocation(this->program, "u_eyeposmodel");
        this->u5Loc = glGetUniformLocation(this->program, "s_texture");
        this->u6Loc = glGetUniformLocation(this->program, "u_texture_cubemap");
        this->u7Loc = glGetUniformLocation(this->program, "glColor");
        this->u8Loc = glGetUniformLocation(this->program, "u_AmbientColor[0]");
        this->u9Loc = glGetUniformLocation(this->program, "u_DiffuseColor[0]");
        this->u10Loc = glGetUniformLocation(this->program, "u_SpecularColor[0]");
        this->u11Loc = glGetUniformLocation(this->program, "u_AmbientColor[1]");
        this->u12Loc = glGetUniformLocation(this->program, "u_DiffuseColor[1]");
        this->u13Loc = glGetUniformLocation(this->program, "u_SpecularColor[1]");
        this->u14Loc = glGetUniformLocation(this->program, "u_SpecularPower");
        this->u15Loc = glGetUniformLocation(this->program, "u_RimColor");
        this->u16Loc = glGetUniformLocation(this->program, "u_TexBiasDiffuse");
        this->u17Loc = glGetUniformLocation(this->program, "u_TexBiasNormal");
        this->u18Loc = glGetUniformLocation(this->program, "u_fogColor");
        this->u19Loc = glGetUniformLocation(this->program, "u_fogMaxDist");
        this->u20Loc = glGetUniformLocation(this->program, "u_fogMinDist");
        this->u21Loc = glGetUniformLocation(this->program, "u_EnableFog");

        glUseProgram(this->program);
        glUniform1i(this->u5Loc, 0);
        glUniform1i(this->u6Loc, 7);
    }

    void BumpShader::UpdateMeshData(Mesh *mesh, Engine *ctx) {
        if (this->u0Loc >= 0)
            glUniformMatrix4fv(this->u0Loc, 1, 0, ctx->worldViewProjMatrix);
        if (this->u1Loc >= 0)
            glUniformMatrix3fv(this->u1Loc, 1, 0, ctx->normalMatrix);
        if (this->u16Loc >= 0)
            glUniform1f(this->u16Loc, Engine::lodBiasDiffuse);
        if (this->u17Loc >= 0)
            glUniform1f(this->u17Loc, Engine::lodBiasNormal);

        if (this->dirty != 0) {
            glUniform3f(this->u2Loc, ctx->lightDir.x, ctx->lightDir.y, ctx->lightDir.z);
            if (this->u4Loc >= 0)
                glUniform3f(this->u4Loc, ctx->lightColor.x, ctx->lightColor.y, ctx->lightColor.z);
            if (this->u7Loc >= 0)
                glUniform4fv(this->u7Loc, 1, ctx->glColor);
            if (this->u8Loc >= 0)
                glUniform3fv(this->u8Loc, 1, (const float *) &ctx->lightAmbientShaded);
            if (this->u9Loc >= 0)
                glUniform3fv(this->u9Loc, 1, (const float *) &ctx->field_0x2fc);
            if (this->u10Loc >= 0)
                glUniform3fv(this->u10Loc, 1, (const float *) &ctx->lightDiffuseShaded);
            if (this->u14Loc >= 0)
                glUniform1f(this->u14Loc, ctx->materialShininess);
            if (this->u15Loc >= 0)
                glUniform3fv(this->u15Loc, 1, (const float *) &ctx->rimColor);
            if (this->u18Loc >= 0)
                glUniform3fv(this->u18Loc, 1, ctx->fogColor);
            if (this->u20Loc >= 0)
                glUniform1f(this->u20Loc, ctx->fogMinDist);
            if (this->u19Loc >= 0)
                glUniform1f(this->u19Loc, ctx->fogMaxDist);
            if (this->u21Loc >= 0)
                glUniform1i(this->u21Loc, AbyssEngine::Engine::fogEnabled);

            if (ctx->lightCount >= 2) {
                glUniform3fv(this->u11Loc, 1, (const float *) &ctx->lightSpecularShaded);
                glUniform3fv(this->u12Loc, 1, (const float *) &ctx->field_0x308);
                glUniform3fv(this->u13Loc, 1, (const float *) &ctx->particleAmbient);
                glUniform3f(this->u3Loc, ctx->field_0x33c.x, ctx->field_0x33c.y,
                            ctx->field_0x33c.z);
            } else {
                glUniform3f(this->u11Loc, 0, 0, 0);
                glUniform3f(this->u12Loc, 0, 0, 0);
                glUniform3f(this->u13Loc, 0, 0, 0);
                glUniform3f(this->u3Loc, ctx->field_0x33c.x, ctx->field_0x33c.y,
                            ctx->field_0x33c.z);
            }
            this->dirty = 0;
        }

        if (this->a0Loc >= 0)
            glEnableVertexAttribArray(this->a0Loc);
        if (this->a1Loc >= 0)
            glEnableVertexAttribArray(this->a1Loc);
        if (this->a2Loc >= 0)
            glEnableVertexAttribArray(this->a2Loc);

        if (mesh->uploaded == 0) {
            if (this->a0Loc >= 0)
                glVertexAttribPointer(this->a0Loc, 3, 0x1406, 0, 0, mesh->positions);
            if (this->a1Loc >= 0)
                glVertexAttribPointer(this->a1Loc, 2, 0x1406, 0, 0, mesh->texCoords);
            if (this->a2Loc >= 0)
                glVertexAttribPointer(this->a2Loc, 3, 0x1406, 0, 0, mesh->normals);
        } else {
            glBindBuffer(0x8892, mesh->positionVBO);
            glVertexAttribPointer(this->a0Loc, 3, 0x1406, 0, 0, 0);
            glBindBuffer(0x8892, mesh->texCoordVBO);
            glVertexAttribPointer(this->a1Loc, 2, 0x1406, 0, 0, 0);
            glBindBuffer(0x8892, mesh->normalVBO);
            glVertexAttribPointer(this->a2Loc, 3, 0x1406, 0, 0, 0);
        }
    }
}
