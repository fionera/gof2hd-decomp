#include "engine/render/shaders/VertexColorShader.h"
#include "engine/render/Engine.h"
#include "engine/render/Mesh.h"
#include <GLES2/gl2.h>

static const char *const kVertexColorShaderVertexSource =
    "attribute highp   vec4 \ta_position; attribute mediump vec2  a_texCoord;   attribute highp   vec3  a_normal; \t\tattribute lowp    vec3 \ta_tangent;\t\tattribute lowp    vec3 \ta_bitangent;\tattribute highp    vec4  a_vertexColor;varying mediump vec2 v_texCoord;varying mediump vec3 v_light_dir;varying mediump vec3 v_eye_dir;varying mediump vec3 v_lightvec;varying mediump vec3 v_normal;varying mediump vec3 v_specular_dir;varying mediump vec4 v_color;uniform highp mat4 u_ModelViewProjectionMatrix;uniform highp vec3 u_lightdirmodel;uniform highp vec3 u_eyeposmodel;\n"
    "//uniform highp mat3 u_ModelMatrix;\n"
    "void main(){\tgl_Position = u_ModelViewProjectionMatrix * a_position;\tv_color     = a_vertexColor;\tv_texCoord  = a_texCoord;}";

static const char *const kVertexColorShaderFragmentSource =
    "precision lowp float;varying mediump vec2 v_texCoord;varying mediump vec3 v_light_dir;varying mediump vec3 v_eye_dir;varying mediump vec3 v_lightvec;varying mediump vec3 v_normal;varying mediump vec3 v_specular_dir;varying highp    vec4 v_color;uniform sampler2D  s_texture;\n"
    "//uniform lowp vec4  u_AmbientColor;\n"
    "//uniform lowp vec4  u_DiffuseColor;\n"
    "//uniform lowp vec4  u_SpecularColor;\n"
    "uniform highp vec4  glColor;\t\n"
    "void main(){\tlowp vec4 colorTex   = texture2D( s_texture, v_texCoord );\n"
    "\tgl_FragColor = colorTex * v_color; // * glColor;  (Color scheint nicht benÃ¶tigt zu werden)\n"
    "}";

namespace AbyssEngine {
    int VertexColorShader::ShaderIndex;

    VertexColorShader::VertexColorShader() {
        VertexColorShader::ShaderIndex = ShaderBaseStruct::shaderIndexIntern;
        this->name = String("VertexColorShader", false);
    }

    void VertexColorShader::Init(Engine *) {
        this->program = this->ES2LoadProgram(kVertexColorShaderVertexSource, kVertexColorShaderFragmentSource);

        this->aVertexColorLoc = glGetAttribLocation(this->program, "a_vertexColor");
        this->aPositionLoc = glGetAttribLocation(this->program, "a_position");
        this->aTexCoordLoc = glGetAttribLocation(this->program, "a_texCoord");
        this->aNormalLoc = glGetAttribLocation(this->program, "a_normal");
        this->aTangentLoc = glGetAttribLocation(this->program, "a_tangent");
        this->aBitangentLoc = glGetAttribLocation(this->program, "a_bitangent");

        this->uModelViewProjectionMatrixLoc = glGetUniformLocation(this->program, "u_ModelViewProjectionMatrix");
        this->uModelMatrixLoc = glGetUniformLocation(this->program, "u_ModelMatrix");
        this->uLightdirmodelLoc = glGetUniformLocation(this->program, "u_lightdirmodel");
        this->uEyeposmodelLoc = glGetUniformLocation(this->program, "u_eyeposmodel");
        this->sTextureLoc = glGetUniformLocation(this->program, "s_texture");
        this->uGlColorLoc = glGetUniformLocation(this->program, "glColor");
        this->uAmbientColorLoc = glGetUniformLocation(this->program, "u_AmbientColor");
        this->uDiffuseColorLoc = glGetUniformLocation(this->program, "u_DiffuseColor");
        this->uSpecularColorLoc = glGetUniformLocation(this->program, "u_SpecularColor");

        glUseProgram(this->program);
        glUniform1i(this->sTextureLoc, 0);
    }

    void VertexColorShader::UpdateMeshData(Mesh *mesh, Engine *engine) {
        if (this->uModelViewProjectionMatrixLoc >= 0) {
            glUniformMatrix4fv(this->uModelViewProjectionMatrixLoc, 1, false, engine->worldViewProjMatrix);
        }
        if (this->uModelMatrixLoc >= 0) {
            glUniformMatrix3fv(this->uModelMatrixLoc, 1, false, engine->normalMatrix);
        }

        if (this->dirty != 0) {
            if (this->uLightdirmodelLoc >= 0) {
                glUniform3f(this->uLightdirmodelLoc, engine->lightDir.x, engine->lightDir.y, engine->lightDir.z);
            }
            if (this->uEyeposmodelLoc >= 0) {
                glUniform3f(this->uEyeposmodelLoc, engine->lightColor.x, engine->lightColor.y, engine->lightColor.z);
            }
            if (this->uGlColorLoc >= 0) {
                glUniform4fv(this->uGlColorLoc, 1, engine->glColor);
            }
            if (this->uAmbientColorLoc >= 0) {
                glUniform4fv(this->uAmbientColorLoc, 1, engine->materialAmbient);
            }
            if (this->uDiffuseColorLoc >= 0) {
                glUniform4fv(this->uDiffuseColorLoc, 1, engine->materialDiffuse);
            }
            if (this->uSpecularColorLoc >= 0) {
                glUniform4fv(this->uSpecularColorLoc, 1, engine->materialSpecular);
            }
            this->dirty = 0;
        }

        if (this->aPositionLoc >= 0) {
            glEnableVertexAttribArray(this->aPositionLoc);
        }
        if (this->aTexCoordLoc >= 0) {
            glEnableVertexAttribArray(this->aTexCoordLoc);
        }
        if (this->aNormalLoc >= 0) {
            glEnableVertexAttribArray(this->aNormalLoc);
        }
        if (this->aTangentLoc >= 0) {
            glEnableVertexAttribArray(this->aTangentLoc);
        }
        if (this->aBitangentLoc >= 0) {
            glEnableVertexAttribArray(this->aBitangentLoc);
        }
        if (this->aVertexColorLoc >= 0) {
            glEnableVertexAttribArray(this->aVertexColorLoc);
        }

        if (mesh->uploaded != 0) {
            glBindBuffer(0x8892, mesh->positionVBO);
            glVertexAttribPointer(this->aPositionLoc, 3, 0x1406, false, 0, 0);
            glBindBuffer(0x8892, mesh->texCoordVBO);
            glVertexAttribPointer(this->aTexCoordLoc, 2, 0x1406, false, 0, 0);
            glBindBuffer(0x8892, mesh->normalVBO);
            glVertexAttribPointer(this->aNormalLoc, 3, 0x1406, false, 0, 0);
            glBindBuffer(0x8892, mesh->tangentVBO);
            glVertexAttribPointer(this->aTangentLoc, 3, 0x1406, false, 0, 0);
            glBindBuffer(0x8892, mesh->binormalVBO);
            glVertexAttribPointer(this->aBitangentLoc, 3, 0x1406, false, 0, 0);
            glBindBuffer(0x8892, mesh->colorVBO);
            glVertexAttribPointer(this->aVertexColorLoc, 4, 0x1406, false, 0, 0);
            return;
        }

        if (this->aPositionLoc >= 0) {
            glVertexAttribPointer(this->aPositionLoc, 3, 0x1406, false, 0, mesh->positions);
        }
        if (this->aTexCoordLoc >= 0) {
            glVertexAttribPointer(this->aTexCoordLoc, 2, 0x1406, false, 0, mesh->texCoords);
        }
        if (this->aNormalLoc >= 0) {
            glVertexAttribPointer(this->aNormalLoc, 3, 0x1406, false, 0, mesh->normals);
        }
        if (this->aTangentLoc >= 0) {
            glVertexAttribPointer(this->aTangentLoc, 3, 0x1406, false, 0, mesh->tangents);
        }
        if (this->aBitangentLoc >= 0) {
            glVertexAttribPointer(this->aBitangentLoc, 3, 0x1406, false, 0, mesh->binormals);
        }
        if (this->aVertexColorLoc >= 0) {
            glVertexAttribPointer(this->aVertexColorLoc, 4, 0x1406, false, 0, mesh->colors);
        }
    }

    void VertexColorShader::SetInActive() {
        if (this->aPositionLoc >= 0) {
            glDisableVertexAttribArray(this->aPositionLoc);
        }
        if (this->aTexCoordLoc >= 0) {
            glDisableVertexAttribArray(this->aTexCoordLoc);
        }
        if (this->aNormalLoc >= 0) {
            glDisableVertexAttribArray(this->aNormalLoc);
        }
        if (this->aTangentLoc >= 0) {
            glDisableVertexAttribArray(this->aTangentLoc);
        }
        if (this->aBitangentLoc >= 0) {
            glDisableVertexAttribArray(this->aBitangentLoc);
        }
        if (this->aVertexColorLoc >= 0) {
            glDisableVertexAttribArray(this->aVertexColorLoc);
        }
    }
}
