#include "engine/render/shaders/TextureAlphaTestShader.h"
#include "engine/render/Engine.h"
#include "engine/render/Mesh.h"
#include <GLES2/gl2.h>


static const char kTextureAlphaTestShaderVertexSource[] =
    "attribute highp vec4 a_position;   \n"
    "attribute mediump vec2 a_texCoord;   \n"
    "varying mediump vec2 v_texCoord;     \n"
    "uniform highp mat4 u_WorldMatrix;  \n"
    "uniform highp float u_fogMaxDist; \n"
    "uniform highp float u_fogMinDist;\n"
    "varying lowp float v_FogFactor;\n"
    "uniform highp vec3 u_eyeposmodel;  \n"
    "uniform bool u_EnableFog; \n"
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
    "   v_texCoord = a_texCoord;  \n"
    "    if ( u_EnableFog ) { \n"
    "       highp float eyeDist = distance(u_eyeposmodel, a_position.xyz); \n"
    "       v_FogFactor = computeLinearFogFactor(eyeDist); \n"
    "    } else { \n"
    "       v_FogFactor = 1.0; \n"
    "    } \n"
    "}                            \n";

static const char kTextureAlphaTestShaderFragmentSource[] =
    "precision lowp float;                            \n"
    "varying mediump vec2 v_texCoord;                            \n"
    "uniform sampler2D s_texture;                        \n"
    "uniform lowp vec4 glColor;\t\t\t\t\t\t\t\t \n"
    "void main()                                         \n"
    "{                                                   \n"
    "\t\tvec4 textureColor = texture2D( s_texture, v_texCoord );\t\n"
    "\t\tif ( textureColor.a < 0.5)\t\n"
    "\t\t\tdiscard;\t\n"
    "\t\tgl_FragColor = glColor * textureColor;\t\t\n"
    "}   \t\t  \n";

static const char kTextureAlphaTestShaderFogFragmentSource[] =
    "precision lowp float;                            \n"
    "varying mediump vec2 v_texCoord;                            \n"
    "uniform sampler2D s_texture;                        \n"
    "uniform lowp vec4 glColor;\t\t\t\t\t\t\t\t \n"
    "uniform vec3 u_fogColor; \n"
    "varying lowp float v_FogFactor;\n"
    "void main()                                         \n"
    "{                                                   \n"
    "\t\tvec4 textureColor = texture2D( s_texture, v_texCoord );\t\n"
    "\t\tif ( textureColor.a < 0.5)\t\n"
    "\t\t\tdiscard;\t\n"
    "       vec4 outColor = glColor * textureColor;\t\t\n"
    "       gl_FragColor.rgb = mix(outColor.rgb, u_fogColor, v_FogFactor); \n"
    "\t\tgl_FragColor.a = outColor.a;\t\t\n"
    "}   \t\t  \n";

namespace AbyssEngine {
    int TextureAlphaTestShader::ShaderIndex = -1;

    TextureAlphaTestShader::TextureAlphaTestShader() {
        ShaderIndex = ShaderBaseStruct::shaderIndexIntern;
        this->name = String("TextureAlphaTestShader", false);
    }

    void TextureAlphaTestShader::Init(Engine *) {
        this->program = this->ES2LoadProgram(kTextureAlphaTestShaderVertexSource,
                                             kTextureAlphaTestShaderFragmentSource);
        ConnectShaderComponents(this->program, 0);

        this->fogProgram = this->ES2LoadProgram(kTextureAlphaTestShaderVertexSource,
                                                  kTextureAlphaTestShaderFogFragmentSource);
        ConnectShaderComponents(this->fogProgram, 1);
    }

    void TextureAlphaTestShader::SetInActive() {
        for (int i = 0; i != 2; i++) {
            glDisableVertexAttribArray(this->aPositionLoc[i]);
            glDisableVertexAttribArray(this->aTexCoordLoc[i]);
        }
    }

    void TextureAlphaTestShader::UpdateMeshData(Mesh *mesh, Engine *engine) {
        uint8_t programIndex = AbyssEngine::Engine::fogEnabled;

        glUniformMatrix4fv(this->uWorldMatrixLoc[programIndex], 1, 0, engine->worldViewProjMatrix);

        if (this->dirty != 0) {
            int location = this->uGlColorLoc[programIndex];
            if (location >= 0) {
                glUniform4fv(location, 1, engine->glColor);
            }

            location = this->uFogColorLoc[programIndex];
            if (location >= 0) {
                glUniform3fv(location, 1, engine->fogColor);
            }

            location = this->uFogMinDistLoc[programIndex];
            if (location >= 0) {
                glUniform1f(location, engine->fogMinDist);
            }

            location = this->uFogMaxDistLoc[programIndex];
            if (location >= 0) {
                glUniform1f(location, engine->fogMaxDist);
            }

            location = this->uEnableFogLoc[programIndex];
            if (location >= 0) {
                glUniform1i(location, AbyssEngine::Engine::fogEnabled);
            }

            location = this->uEyePosModelLoc[programIndex];
            if (location >= 0) {
                glUniform3f(location, engine->lightColor.x, engine->lightColor.y,
                            engine->lightColor.z);
            }

            this->dirty = 0;
        }

        if ((((uint32_t) mesh->vertexFormat) << 30) < 0x80000000u) {
            return;
        }

        glEnableVertexAttribArray(this->aPositionLoc[programIndex]);
        glEnableVertexAttribArray(this->aTexCoordLoc[programIndex]);

        if (mesh->uploaded == 0) {
            glVertexAttribPointer(this->aPositionLoc[programIndex], 3, 0x1406, 0, 0, mesh->positions);
            glVertexAttribPointer(this->aTexCoordLoc[programIndex], 2, 0x1406, 0, 0, mesh->texCoords);
        } else {
            glBindBuffer(0x8892, mesh->positionVBO);
            glVertexAttribPointer(this->aPositionLoc[programIndex], 3, 0x1406, 0, 0, 0);
            glBindBuffer(0x8892, mesh->texCoordVBO);
            glVertexAttribPointer(this->aTexCoordLoc[programIndex], 2, 0x1406, 0, 0, 0);
        }
    }

    void TextureAlphaTestShader::ConnectShaderComponents(unsigned int program, int index) {
        this->sTextureLoc[index] = glGetUniformLocation(program, "s_texture");
        this->aPositionLoc[index] = glGetAttribLocation(program, "a_position");
        this->aTexCoordLoc[index] = glGetAttribLocation(program, "a_texCoord");
        this->uWorldMatrixLoc[index] = glGetUniformLocation(program, "u_WorldMatrix");
        this->uGlColorLoc[index] = glGetUniformLocation(program, "glColor");
        this->uFogColorLoc[index] = glGetUniformLocation(program, "u_fogColor");
        this->uFogMaxDistLoc[index] = glGetUniformLocation(program, "u_fogMaxDist");
        this->uFogMinDistLoc[index] = glGetUniformLocation(program, "u_fogMinDist");
        this->uEnableFogLoc[index] = glGetUniformLocation(program, "u_EnableFog");
        this->uEyePosModelLoc[index] = glGetUniformLocation(program, "u_eyeposmodel");

        glUseProgram(program);
        glUniform1i(this->sTextureLoc[index], 0);
    }

    void TextureAlphaTestShader::UseShader(bool) {
        if (AbyssEngine::Engine::fogEnabled != 0) {
            int program = this->fogProgram;
            if (program != 0) {
                glUseProgram(program);
                return;
            }
        }
        glUseProgram(this->program);
    }
}
