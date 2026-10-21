#include "engine/render/shaders/GenericShader2.h"
#include "engine/render/Engine.h"
#include "engine/render/Mesh.h"
#include <GLES2/gl2.h>

static const char *const kGenericShader2VertexSource =
    "attribute highp vec4 \ta_position;attribute mediump vec2  a_texCoord; attribute highp vec3  \ta_normal; \tattribute lowp vec3 \ta_tangent;\tattribute lowp vec3 \ta_bitangent;\tuniform highp mat4 u_ModelViewProjectionMatrix;uniform highp mat3 u_ModelMatrix;uniform highp vec3 u_lightdirmodel;uniform highp vec3 u_eyeposmodel;varying mediump    vec2 v_texCoord;void main(){\tv_texCoord = a_texCoord;\tgl_Position = u_ModelViewProjectionMatrix * a_position;\t}";

static const char *const kGenericShader2FragmentSource =
    "precision lowp float;uniform sampler2D  s_texture;uniform lowp vec4  glColor;uniform lowp vec4  u_AmbientColor;uniform lowp vec4  u_DiffuseColor;uniform lowp vec4  u_SpecularColor;varying mediump    vec2 v_texCoord;void main(){\tgl_FragColor =  texture2D( s_texture, v_texCoord );}";

namespace AbyssEngine {
    int GenericShader2::ShaderIndex;

    void GenericShader2::SetInActive() {
        if (aPositionLoc >= 0)
            glDisableVertexAttribArray(aPositionLoc);
        if (aTexCoordLoc >= 0)
            glDisableVertexAttribArray(aTexCoordLoc);
        if (aNormalLoc >= 0)
            glDisableVertexAttribArray(aNormalLoc);
        if (aTangentLoc >= 0)
            glDisableVertexAttribArray(aTangentLoc);
        if (aBitangentLoc >= 0)
            glDisableVertexAttribArray(aBitangentLoc);
    }

    void GenericShader2::Init(Engine *) {
        this->program = this->ES2LoadProgram(kGenericShader2VertexSource, kGenericShader2FragmentSource);

        aPositionLoc = glGetAttribLocation(this->program, "a_position");
        aTexCoordLoc = glGetAttribLocation(this->program, "a_texCoord");
        aNormalLoc = glGetAttribLocation(this->program, "a_normal");
        aTangentLoc = glGetAttribLocation(this->program, "a_tangent");
        aBitangentLoc = glGetAttribLocation(this->program, "a_bitangent");

        uModelViewProjectionMatrixLoc = glGetUniformLocation(this->program, "u_ModelViewProjectionMatrix");
        uModelMatrixLoc = glGetUniformLocation(this->program, "u_ModelMatrix");
        uLightdirmodelLoc = glGetUniformLocation(this->program, "u_lightdirmodel");
        uEyeposmodelLoc = glGetUniformLocation(this->program, "u_eyeposmodel");
        sTextureLoc = glGetUniformLocation(this->program, "s_texture");
        uGlColorLoc = glGetUniformLocation(this->program, "glColor");
        uAmbientColorLoc = glGetUniformLocation(this->program, "u_AmbientColor");
        uDiffuseColorLoc = glGetUniformLocation(this->program, "u_DiffuseColor");
        uSpecularColorLoc = glGetUniformLocation(this->program, "u_SpecularColor");

        glUseProgram(this->program);
        glUniform1i(sTextureLoc, 0);
    }

    void GenericShader2::UpdateMeshData(Mesh *mesh, Engine *engine) {
        if (uModelViewProjectionMatrixLoc >= 0)
            glUniformMatrix4fv(uModelViewProjectionMatrixLoc, 1, 0, engine->worldViewProjMatrix);
        if (uModelMatrixLoc >= 0)
            glUniformMatrix3fv(uModelMatrixLoc, 1, 0, engine->normalMatrix);

        if (this->dirty != 0) {
            if (uLightdirmodelLoc >= 0)
                glUniform3f(uLightdirmodelLoc, engine->lightDir.x, engine->lightDir.y, engine->lightDir.z);
            if (uEyeposmodelLoc >= 0)
                glUniform3f(uEyeposmodelLoc, engine->lightColor.x, engine->lightColor.y, engine->lightColor.z);
            if (uGlColorLoc >= 0)
                glUniform4fv(uGlColorLoc, 1, engine->glColor);
            if (uAmbientColorLoc >= 0)
                glUniform4fv(uAmbientColorLoc, 1, engine->materialAmbient);
            if (uDiffuseColorLoc >= 0)
                glUniform4fv(uDiffuseColorLoc, 1, engine->materialDiffuse);
            if (uSpecularColorLoc >= 0)
                glUniform4fv(uSpecularColorLoc, 1, engine->materialSpecular);
            this->dirty = 0;
        }

        if (aPositionLoc >= 0)
            glEnableVertexAttribArray(aPositionLoc);
        if (aTexCoordLoc >= 0)
            glEnableVertexAttribArray(aTexCoordLoc);
        if (aNormalLoc >= 0)
            glEnableVertexAttribArray(aNormalLoc);
        if (aTangentLoc >= 0)
            glEnableVertexAttribArray(aTangentLoc);
        if (aBitangentLoc >= 0)
            glEnableVertexAttribArray(aBitangentLoc);

        if (aPositionLoc >= 0)
            glVertexAttribPointer(aPositionLoc, 3, 0x1406, 0, 0, mesh->positions);
        if (aTexCoordLoc >= 0)
            glVertexAttribPointer(aTexCoordLoc, 2, 0x1406, 0, 0, mesh->texCoords);
        if (aNormalLoc >= 0)
            glVertexAttribPointer(aNormalLoc, 3, 0x1406, 0, 0, mesh->normals);
        if (aTangentLoc >= 0)
            glVertexAttribPointer(aTangentLoc, 3, 0x1406, 0, 0, mesh->tangents);
        if (aBitangentLoc >= 0)
            glVertexAttribPointer(aBitangentLoc, 3, 0x1406, 0, 0, mesh->binormals);
    }

    GenericShader2::GenericShader2() {
        ShaderIndex = ShaderBaseStruct::shaderIndexIntern;
        this->name = String("GenericShader2", false);
    }
}
