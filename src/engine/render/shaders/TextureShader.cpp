#include "engine/render/shaders/TextureShader.h"
#include "engine/render/Engine.h"
#include "engine/render/Mesh.h"
#include <GLES2/gl2.h>


static const char *const kTextureShaderVertexSource =
    "attribute highp vec4 a_position;   \n"
    "attribute mediump vec2 a_texCoord;   \n"
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
    "    if ( u_EnableFog ) { \n"
    "       highp float eyeDist = distance(u_eyeposmodel, a_position.xyz); \n"
    "       v_FogFactor = computeLinearFogFactor(eyeDist); \n"
    "    } else { \n"
    "       v_FogFactor = 1.0; \n"
    "    } \n"
    "}                            \n";

static const char *const kTextureShaderFragmentSource =
    "precision lowp float;                            \n"
    "varying mediump vec2 v_texCoord;                            \n"
    "uniform sampler2D s_texture;                        \n"
    "uniform lowp vec4 glColor;\t\t\t\t\t\t\t\t \n"
    "uniform float u_DarkenValue; \n"
    "void main()                                         \n"
    "{                                                   \n"
    "\tvec4 textureColor = texture2D( s_texture, v_texCoord );\t\n"
    "\tgl_FragColor = glColor * textureColor;\t\t\n"
    "} \n";

static const char *const kTextureShaderFogFragmentSource =
    "precision lowp float;                            \n"
    "varying mediump vec2 v_texCoord;                            \n"
    "uniform sampler2D s_texture;                        \n"
    "uniform lowp vec4 glColor;\t\t\t\t\t\t\t\t \n"
    "uniform float u_DarkenValue; \n"
    "uniform vec3 u_fogColor; \n"
    "varying lowp float v_FogFactor;\n"
    "void main()                                         \n"
    "{                                                   \n"
    "\tvec4 textureColor = texture2D( s_texture, v_texCoord );\t\n"
    "\tvec4 outColor = textureColor;\t\t\n"
    "   gl_FragColor.rgb = mix(outColor.rgb, u_fogColor, v_FogFactor); \n"
    "   gl_FragColor.a = glColor.a*outColor.a;\n"
    "}   \t\t  \n";

namespace AbyssEngine {
    int TextureShader::ShaderIndex = -1;

    TextureShader::TextureShader() {
        TextureShader::ShaderIndex = ShaderBaseStruct::shaderIndexIntern;
        this->name = String("TextureShader", false);
    }

    void TextureShader::Init(Engine *) {
        const char *vertexShader = kTextureShaderVertexSource;

        this->program = this->ES2LoadProgram(vertexShader, kTextureShaderFragmentSource);
        ConnectShaderComponents(this->program, 0);

        this->programExt = this->ES2LoadProgram(vertexShader, kTextureShaderFogFragmentSource);
        ConnectShaderComponents(this->programExt, 1);
    }

    void TextureShader::UseShader(bool) {
        if (AbyssEngine::Engine::fogEnabled != 0 && this->programExt != 0) {
            glUseProgram(this->programExt);
            return;
        }
        glUseProgram(this->program);
    }

    void TextureShader::SetInActive() {
        for (uint32_t i = 0; i != 2; i += 1) {
            glDisableVertexAttribArray(this->aPositionLoc[i]);
            glDisableVertexAttribArray(this->aTexCoordLoc[i]);
        }
    }

    void TextureShader::UpdateMeshData(Mesh *mesh, Engine *engine) {
        int slot = AbyssEngine::Engine::fogEnabled;

        glUniformMatrix4fv(this->uWorldMatrixLoc[slot], 1, 0, engine->worldViewProjMatrix);

        int location = this->uUvMatrixLoc[slot];
        if (location >= 0) {
            glUniformMatrix4fv(location, 1, 0, engine->uvMatrix);
        }

        location = this->uDarkenValueLoc[slot];
        if (location >= 0) {
            glUniform1f(location, 1.0f - reinterpret_cast<float &>(mesh->materialId));
        }

        if (this->dirty != 0) {
            glUniform4fv(this->uGlColorLoc[slot], 1, engine->glColor);

            location = this->uUVAnimationLoc[slot];
            if (location >= 0) {
                glUniform1i(location, mesh->hasAnimation);
            }

            location = this->uFogColorLoc[slot];
            if (location >= 0) {
                glUniform3fv(location, 1, engine->fogColor);
            }

            location = this->uFogMinDistLoc[slot];
            if (location >= 0) {
                glUniform1f(location, engine->fogMinDist);
            }

            location = this->uFogMaxDistLoc[slot];
            if (location >= 0) {
                glUniform1f(location, engine->fogMaxDist);
            }

            location = this->uEnableFogLoc[slot];
            if (location >= 0) {
                glUniform1i(location, AbyssEngine::Engine::fogEnabled);
            }

            location = this->uEyePosModelLoc[slot];
            if (location >= 0) {
                glUniform3f(location, engine->lightColor.x, engine->lightColor.y, engine->lightColor.z);
            }

            this->dirty = 0;
        }

        if ((int) (mesh->vertexFormat << 30) < 0) {
            glEnableVertexAttribArray(this->aPositionLoc[slot]);
            glEnableVertexAttribArray(this->aTexCoordLoc[slot]);

            if (mesh->uploaded == 0) {
                glVertexAttribPointer(this->aPositionLoc[slot], 3, 0x1406, 0, 0, mesh->positions);
                glVertexAttribPointer(this->aTexCoordLoc[slot], 2, 0x1406, 0, 0, mesh->texCoords);
            } else {
                glBindBuffer(0x8892, mesh->positionVBO);
                glVertexAttribPointer(this->aPositionLoc[slot], 3, 0x1406, 0, 0, 0);
                glBindBuffer(0x8892, mesh->texCoordVBO);
                glVertexAttribPointer(this->aTexCoordLoc[slot], 2, 0x1406, 0, 0, 0);
            }
        }
    }

    void TextureShader::ConnectShaderComponents(uint32_t program, int slot) {
        this->sTextureLoc[slot] = glGetUniformLocation(program, "s_texture");
        this->aPositionLoc[slot] = glGetAttribLocation(program, "a_position");
        this->aTexCoordLoc[slot] = glGetAttribLocation(program, "a_texCoord");
        this->uWorldMatrixLoc[slot] = glGetUniformLocation(program, "u_WorldMatrix");
        this->uGlColorLoc[slot] = glGetUniformLocation(program, "glColor");
        this->uFogColorLoc[slot] = glGetUniformLocation(program, "u_fogColor");
        this->uFogMaxDistLoc[slot] = glGetUniformLocation(program, "u_fogMaxDist");
        this->uFogMinDistLoc[slot] = glGetUniformLocation(program, "u_fogMinDist");
        this->uEnableFogLoc[slot] = glGetUniformLocation(program, "u_EnableFog");
        this->uEyePosModelLoc[slot] = glGetUniformLocation(program, "u_eyeposmodel");
        this->uDarkenValueLoc[slot] = glGetUniformLocation(program, "u_DarkenValue");
        this->uUVAnimationLoc[slot] = glGetUniformLocation(program, "u_UVAnimation");
        this->uUvMatrixLoc[slot] = glGetUniformLocation(program, "u_UvMatrix");

        glUseProgram(program);
        glUniform1i(this->sTextureLoc[slot], 0);
    }
}
