#include "engine/render/shaders/GenericShader1.h"
#include "engine/render/Engine.h"
#include "engine/render/Mesh.h"
#include <GLES2/gl2.h>

static const char *const kGenericShader1VertexSource =
    "attribute highp vec4 \ta_position;  attribute mediump vec2  a_texCoord;   \tattribute highp vec3  \ta_normal; \t\tattribute lowp vec3 \ta_tangent;\t\tattribute lowp vec3 \ta_bitangent;\tuniform highp mat4 u_ModelViewProjectionMatrix;uniform highp mat3 u_ModelMatrix;uniform highp vec3 u_lightdirmodel;uniform highp vec3 u_eyeposmodel;varying mediump    vec2 v_texCoord;void main(){\tv_texCoord = a_texCoord;\tgl_Position = u_ModelViewProjectionMatrix * a_position;\t}";

static const char *const kGenericShader1FragmentSource =
    "precision lowp float;uniform sampler2D  s_texture;uniform lowp vec4  glColor;uniform lowp vec4  u_AmbientColor;uniform lowp vec4  u_DiffuseColor;uniform lowp vec4  u_SpecularColor;varying mediump    vec2 v_texCoord;void main(){\tgl_FragColor =  texture2D( s_texture, v_texCoord );}\t";

namespace AbyssEngine {
    int GenericShader1::ShaderIndex = -1;

    GenericShader1::GenericShader1() {
        GenericShader1::ShaderIndex = ShaderBaseStruct::shaderIndexIntern;
        this->name = String("GenericShader1", false);
    }

    void GenericShader1::Init(Engine *) {
        this->program = this->ES2LoadProgram(kGenericShader1VertexSource, kGenericShader1FragmentSource);

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

    void GenericShader1::UpdateMeshData(Mesh *mesh, Engine *engine) {
        if (this->uModelViewProjectionMatrixLoc >= 0)
            glUniformMatrix4fv(this->uModelViewProjectionMatrixLoc, 1, 0, engine->worldViewProjMatrix);
        if (this->uModelMatrixLoc >= 0)
            glUniformMatrix3fv(this->uModelMatrixLoc, 1, 0, engine->normalMatrix);

        if (this->dirty != 0) {
            if (this->uLightdirmodelLoc >= 0)
                glUniform3f(this->uLightdirmodelLoc, engine->lightDir[0].x, engine->lightDir[0].y,
                            engine->lightDir[0].z);
            if (this->uEyeposmodelLoc >= 0)
                glUniform3f(this->uEyeposmodelLoc, engine->lightColor.x, engine->lightColor.y,
                            engine->lightColor.z);
            if (this->uGlColorLoc >= 0)
                glUniform4fv(this->uGlColorLoc, 1, engine->glColor);
            if (this->uAmbientColorLoc >= 0)
                glUniform4fv(this->uAmbientColorLoc, 1, engine->materialAmbient);
            if (this->uDiffuseColorLoc >= 0)
                glUniform4fv(this->uDiffuseColorLoc, 1, engine->materialDiffuse);
            if (this->uSpecularColorLoc >= 0)
                glUniform4fv(this->uSpecularColorLoc, 1, engine->materialSpecular);
            this->dirty = 0;
        }

        if (this->aPositionLoc >= 0)
            glEnableVertexAttribArray(this->aPositionLoc);
        if (this->aTexCoordLoc >= 0)
            glEnableVertexAttribArray(this->aTexCoordLoc);
        if (this->aNormalLoc >= 0)
            glEnableVertexAttribArray(this->aNormalLoc);
        if (this->aTangentLoc >= 0)
            glEnableVertexAttribArray(this->aTangentLoc);
        if (this->aBitangentLoc >= 0)
            glEnableVertexAttribArray(this->aBitangentLoc);

        if (this->aPositionLoc >= 0)
            glVertexAttribPointer(this->aPositionLoc, 3, 0x1406, 0, 0, mesh->positions);
        if (this->aTexCoordLoc >= 0)
            glVertexAttribPointer(this->aTexCoordLoc, 2, 0x1406, 0, 0, mesh->texCoords);
        if (this->aNormalLoc >= 0)
            glVertexAttribPointer(this->aNormalLoc, 3, 0x1406, 0, 0, mesh->normals);
        if (this->aTangentLoc >= 0)
            glVertexAttribPointer(this->aTangentLoc, 3, 0x1406, 0, 0, mesh->tangents);
        if (this->aBitangentLoc >= 0)
            glVertexAttribPointer(this->aBitangentLoc, 3, 0x1406, 0, 0, mesh->binormals);
    }

    void GenericShader1::SetInActive() {
        if (this->aPositionLoc >= 0)
            glDisableVertexAttribArray(this->aPositionLoc);
        if (this->aTexCoordLoc >= 0)
            glDisableVertexAttribArray(this->aTexCoordLoc);
        if (this->aNormalLoc >= 0)
            glDisableVertexAttribArray(this->aNormalLoc);
        if (this->aTangentLoc >= 0)
            glDisableVertexAttribArray(this->aTangentLoc);
        if (this->aBitangentLoc >= 0)
            glDisableVertexAttribArray(this->aBitangentLoc);
    }
}
