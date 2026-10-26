#include "engine/render/shaders/TextureVtxColorShader.h"
#include "engine/render/Engine.h"
#include "engine/render/Mesh.h"
#include <GLES2/gl2.h>

static const char kTextureVtxColorShaderVertexSource[] =
    "attribute highp vec4 a_position;   \n"
    "attribute mediump vec2 a_texCoord;   \n"
    "attribute mediump vec4 a_VertexColor;   \n"
    "varying mediump vec4 v_VertexColor;     \n"
    "varying mediump vec2 v_texCoord;     \n"
    "uniform highp mat4 u_WorldMatrix;  \n"
    "uniform highp mat4 u_UvMatrix;  \n"
    "uniform highp float u_fogMaxDist; \n"
    "uniform highp float u_fogMinDist;\n"
    "varying lowp float v_FogFactor;\n"
    "uniform highp vec3 u_eyeposmodel;  \n"
    "uniform bool u_EnableFog; \n"
    "uniform bool u_UVAnimation; \n"
    "lowp float computeLinearFogFactor(highp float eyeDist) { \n"
    "    highp float factor; \n"
    "    // Compute linear fog equation \n"
    "    factor = (u_fogMaxDist - eyeDist) / (u_fogMaxDist - u_fogMinDist);\n"
    "    // Clamp in the [0,1] range \n"
    "    factor = (1.0 - clamp(factor, 0.0, 1.0));\n"
    "    return factor; \n"
    "}    \n"
    "void main()                  \n"
    "{                            \n"
    "   gl_Position = u_WorldMatrix * a_position;\t\t\n"
    "   if ( u_UVAnimation ) \n"
    "       v_texCoord = (u_UvMatrix*vec4(a_texCoord.x, a_texCoord.y, 0.0, 1.0)).xy;  \n"
    "   else \n"
    "       v_texCoord = a_texCoord; \n"
    "   v_VertexColor = a_VertexColor;  \n"
    "    if ( u_EnableFog ) { \n"
    "       highp float eyeDist = distance(u_eyeposmodel, a_position.xyz); \n"
    "       v_FogFactor = computeLinearFogFactor(eyeDist); \n"
    "    } else { \n"
    "       v_FogFactor = 1.0; \n"
    "    } \n"
    "}                            \n";

static const char kTextureVtxColorShaderFragmentSource[] =
    "precision lowp float;                            \n"
    "varying mediump vec2 v_texCoord;                            \n"
    "varying mediump vec4 v_VertexColor;     \n"
    "uniform sampler2D s_texture;                        \n"
    "uniform lowp vec4 glColor;\t\t\t\t\t\t\t\t \n"
    "uniform bool u_EnableFog; \n"
    "uniform vec3 u_fogColor; \n"
    "uniform float u_DarkenValue; \n"
    "varying lowp float v_FogFactor;\n"
    "void main()                                         \n"
    "{                                                   \n"
    "\tvec4 textureColor = texture2D( s_texture, v_texCoord );\t\n"
    "\tgl_FragColor = v_VertexColor * textureColor;\t\t\n"
    "}   \n";

static const char kTextureVtxColorShaderFogFragmentSource[] =
    "precision lowp float;                            \n"
    "varying mediump vec2 v_texCoord;                            \n"
    "varying mediump vec4 v_VertexColor;     \n"
    "uniform sampler2D s_texture;                        \n"
    "uniform lowp vec4 glColor;\t\t\t\t\t\t\t\t \n"
    "uniform bool u_EnableFog; \n"
    "uniform vec3 u_fogColor; \n"
    "uniform float u_DarkenValue; \n"
    "varying lowp float v_FogFactor;\n"
    "void main()                                         \n"
    "{                                                   \n"
    "\tvec4 textureColor = texture2D( s_texture, v_texCoord );\t\n"
    "\tvec4 outColor = v_VertexColor * textureColor;\t\t\n"
    "   gl_FragColor.rgb = v_VertexColor.rgb*mix(outColor.rgb, u_fogColor, v_FogFactor); \n"
    "   gl_FragColor.a = outColor.a;\n"
    "}   \n";

namespace AbyssEngine {
    static inline bool EngineFogEnabled() { return Engine::fogEnabled; }

    int TextureVtxColorShader::ShaderIndex = -1;

    TextureVtxColorShader::TextureVtxColorShader() {
        TextureVtxColorShader::ShaderIndex = ShaderBaseStruct::shaderIndexIntern;
        this->name = String("TextureVtxColorShader", false);
    }

    void TextureVtxColorShader::Init(Engine *) {
        this->program = this->ES2LoadProgram(kTextureVtxColorShaderVertexSource,
                                             kTextureVtxColorShaderFragmentSource);
        ConnectShaderComponents(this->program, 0);

        this->fogProgram = this->ES2LoadProgram(kTextureVtxColorShaderVertexSource,
                                                kTextureVtxColorShaderFogFragmentSource);
        ConnectShaderComponents(this->fogProgram, 1);
    }

    void TextureVtxColorShader::ConnectShaderComponents(uint32_t program, int index) {
        loc_s_texture[index] = glGetUniformLocation(program, "s_texture");
        loc_a_position[index] = glGetAttribLocation(program, "a_position");
        loc_a_texCoord[index] = glGetAttribLocation(program, "a_texCoord");
        loc_a_VertexColor[index] = glGetAttribLocation(program, "a_VertexColor");
        loc_u_WorldMatrix[index] = glGetUniformLocation(program, "u_WorldMatrix");
        loc_glColor[index] = glGetUniformLocation(program, "glColor");
        loc_u_DarkenValue[index] = glGetUniformLocation(program, "u_DarkenValue");
        loc_u_fogColor[index] = glGetUniformLocation(program, "u_fogColor");
        loc_u_fogMaxDist[index] = glGetUniformLocation(program, "u_fogMaxDist");
        loc_u_fogMinDist[index] = glGetUniformLocation(program, "u_fogMinDist");
        loc_u_EnableFog[index] = glGetUniformLocation(program, "u_EnableFog");
        loc_u_eyeposmodel[index] = glGetUniformLocation(program, "u_eyeposmodel");
        loc_u_UVAnimation[index] = glGetUniformLocation(program, "u_UVAnimation");
        loc_u_UvMatrix[index] = glGetUniformLocation(program, "u_UvMatrix");

        glUseProgram(program);
        glUniform1i(loc_s_texture[index], 0);
    }

    void TextureVtxColorShader::UseShader(bool) {
        if (EngineFogEnabled() && this->fogProgram != 0) {
            glUseProgram(this->fogProgram);
            return;
        }
        glUseProgram(this->program);
    }

    void TextureVtxColorShader::SetInActive() {
        for (int i = 0; i != 2; i++) {
            glDisableVertexAttribArray((uint32_t) loc_a_position[i]);
            glDisableVertexAttribArray((uint32_t) loc_a_texCoord[i]);
            glDisableVertexAttribArray((uint32_t) loc_a_VertexColor[i]);
        }
    }

    void TextureVtxColorShader::UpdateMeshData(Mesh *mesh, Engine *engine) {
        int index = EngineFogEnabled() ? 1 : 0;

        glUniformMatrix4fv(loc_u_WorldMatrix[index], 1, 0, engine->worldViewProjMatrix);

        int location = loc_u_UvMatrix[index];
        if (location >= 0) {
            glUniformMatrix4fv(location, 1, 0, engine->uvMatrix);
        }

        location = loc_u_DarkenValue[index];
        if (location >= 0) {
            glUniform1f(location, 1.0f - reinterpret_cast<float &>(mesh->materialId));
        }

        if (this->dirty != 0) {
            glUniform4fv(loc_glColor[index], 1, engine->glColor);

            location = loc_u_UVAnimation[index];
            if (location >= 0) {
                glUniform1i(location, mesh->hasAnimation);
            }

            location = loc_u_fogColor[index];
            if (location >= 0) {
                glUniform3fv(location, 1, engine->fogColor);
            }

            location = loc_u_fogMinDist[index];
            if (location >= 0) {
                glUniform1f(location, engine->fogMinDist);
            }

            location = loc_u_fogMaxDist[index];
            if (location >= 0) {
                glUniform1f(location, engine->fogMaxDist);
            }

            location = loc_u_EnableFog[index];
            if (location >= 0) {
                glUniform1i(location, EngineFogEnabled() ? 1 : 0);
            }

            location = loc_u_eyeposmodel[index];
            if (location >= 0) {
                glUniform3f(location,
                            engine->lightColor.x,
                            engine->lightColor.y,
                            engine->lightColor.z);
            }

            this->dirty = 0;
        }

        glEnableVertexAttribArray((uint32_t) loc_a_position[index]);
        glEnableVertexAttribArray((uint32_t) loc_a_texCoord[index]);
        glEnableVertexAttribArray((uint32_t) loc_a_VertexColor[index]);

        if (mesh->uploaded != 0) {
            glBindBuffer(0x8892, mesh->positionVBO);
            glVertexAttribPointer((uint32_t) loc_a_position[index], 3, 0x1406, 0, 0, 0);
            glBindBuffer(0x8892, mesh->texCoordVBO);
            glVertexAttribPointer((uint32_t) loc_a_texCoord[index], 2, 0x1406, 0, 0, 0);
            glBindBuffer(0x8892, mesh->colorVBO);
            glVertexAttribPointer((uint32_t) loc_a_VertexColor[index], 4, 0x1406, 0, 0, 0);
            return;
        }

        glVertexAttribPointer((uint32_t) loc_a_position[index], 3, 0x1406, 0, 0,
                              mesh->positions);
        glVertexAttribPointer((uint32_t) loc_a_texCoord[index], 2, 0x1406, 0, 0,
                              mesh->texCoords);
        glVertexAttribPointer((uint32_t) loc_a_VertexColor[index], 4, 0x1406, 0, 0,
                              mesh->colors);
    }
}
