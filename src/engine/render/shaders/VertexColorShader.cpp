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

        this->aColor = glGetAttribLocation(this->program, "a_vertexColor");
        this->aPosition = glGetAttribLocation(this->program, "a_position");
        this->aTexCoord = glGetAttribLocation(this->program, "a_texCoord");
        this->aNormal = glGetAttribLocation(this->program, "a_normal");
        this->aTangent = glGetAttribLocation(this->program, "a_tangent");
        this->aBiNormal = glGetAttribLocation(this->program, "a_bitangent");

        this->uWorldViewProj = glGetUniformLocation(this->program, "u_ModelViewProjectionMatrix");
        this->uNormalMatrix = glGetUniformLocation(this->program, "u_ModelMatrix");
        this->uLightDir = glGetUniformLocation(this->program, "u_lightdirmodel");
        this->uLightColor = glGetUniformLocation(this->program, "u_eyeposmodel");
        this->uAmbientColor = glGetUniformLocation(this->program, "s_texture");
        this->uMaterialDiffuse = glGetUniformLocation(this->program, "glColor");
        this->uMaterialAmbient = glGetUniformLocation(this->program, "u_AmbientColor");
        this->uMaterialSpecular = glGetUniformLocation(this->program, "u_DiffuseColor");
        this->uMaterialShininess = glGetUniformLocation(this->program, "u_SpecularColor");

        glUseProgram(this->program);
        glUniform1i(this->uAmbientColor, 0);
    }

    void VertexColorShader::UpdateMeshData(Mesh *mesh, Engine *engine) {
        if (this->uWorldViewProj >= 0) {
            glUniformMatrix4fv(this->uWorldViewProj, 1, false, engine->worldViewProjMatrix);
        }
        if (this->uNormalMatrix >= 0) {
            glUniformMatrix3fv(this->uNormalMatrix, 1, false, engine->normalMatrix);
        }

        if (this->dirty != 0) {
            if (this->uLightDir >= 0) {
                glUniform3f(this->uLightDir, engine->lightDir.x, engine->lightDir.y, engine->lightDir.z);
            }
            if (this->uLightColor >= 0) {
                glUniform3f(this->uLightColor, engine->lightColor.x, engine->lightColor.y, engine->lightColor.z);
            }
            if (this->uMaterialDiffuse >= 0) {
                glUniform4fv(this->uMaterialDiffuse, 1, engine->glColor);
            }
            if (this->uMaterialAmbient >= 0) {
                glUniform4fv(this->uMaterialAmbient, 1, engine->materialAmbient);
            }
            if (this->uMaterialSpecular >= 0) {
                glUniform4fv(this->uMaterialSpecular, 1, engine->materialDiffuse);
            }
            if (this->uMaterialShininess >= 0) {
                glUniform4fv(this->uMaterialShininess, 1, engine->materialSpecular);
            }
            this->dirty = 0;
        }

        if (this->aPosition >= 0) {
            glEnableVertexAttribArray(this->aPosition);
        }
        if (this->aTexCoord >= 0) {
            glEnableVertexAttribArray(this->aTexCoord);
        }
        if (this->aNormal >= 0) {
            glEnableVertexAttribArray(this->aNormal);
        }
        if (this->aTangent >= 0) {
            glEnableVertexAttribArray(this->aTangent);
        }
        if (this->aBiNormal >= 0) {
            glEnableVertexAttribArray(this->aBiNormal);
        }
        if (this->aColor >= 0) {
            glEnableVertexAttribArray(this->aColor);
        }

        if (mesh->uploaded != 0) {
            glBindBuffer(0x8892, mesh->positionVBO);
            glVertexAttribPointer(this->aPosition, 3, 0x1406, false, 0, 0);
            glBindBuffer(0x8892, mesh->texCoordVBO);
            glVertexAttribPointer(this->aTexCoord, 2, 0x1406, false, 0, 0);
            glBindBuffer(0x8892, mesh->normalVBO);
            glVertexAttribPointer(this->aNormal, 3, 0x1406, false, 0, 0);
            glBindBuffer(0x8892, mesh->tangentVBO);
            glVertexAttribPointer(this->aTangent, 3, 0x1406, false, 0, 0);
            glBindBuffer(0x8892, mesh->binormalVBO);
            glVertexAttribPointer(this->aBiNormal, 3, 0x1406, false, 0, 0);
            glBindBuffer(0x8892, mesh->colorVBO);
            glVertexAttribPointer(this->aColor, 4, 0x1406, false, 0, 0);
            return;
        }

        if (this->aPosition >= 0) {
            glVertexAttribPointer(this->aPosition, 3, 0x1406, false, 0, mesh->positions);
        }
        if (this->aTexCoord >= 0) {
            glVertexAttribPointer(this->aTexCoord, 2, 0x1406, false, 0, mesh->texCoords);
        }
        if (this->aNormal >= 0) {
            glVertexAttribPointer(this->aNormal, 3, 0x1406, false, 0, mesh->normals);
        }
        if (this->aTangent >= 0) {
            glVertexAttribPointer(this->aTangent, 3, 0x1406, false, 0, mesh->tangents);
        }
        if (this->aBiNormal >= 0) {
            glVertexAttribPointer(this->aBiNormal, 3, 0x1406, false, 0, mesh->binormals);
        }
        if (this->aColor >= 0) {
            glVertexAttribPointer(this->aColor, 4, 0x1406, false, 0, mesh->colors);
        }
    }

    void VertexColorShader::SetInActive() {
        if (this->aPosition >= 0) {
            glDisableVertexAttribArray(this->aPosition);
        }
        if (this->aTexCoord >= 0) {
            glDisableVertexAttribArray(this->aTexCoord);
        }
        if (this->aNormal >= 0) {
            glDisableVertexAttribArray(this->aNormal);
        }
        if (this->aTangent >= 0) {
            glDisableVertexAttribArray(this->aTangent);
        }
        if (this->aBiNormal >= 0) {
            glDisableVertexAttribArray(this->aBiNormal);
        }
        if (this->aColor >= 0) {
            glDisableVertexAttribArray(this->aColor);
        }
    }
}
