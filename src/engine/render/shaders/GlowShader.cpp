#include "engine/render/shaders/GlowShader.h"
#include "engine/render/Engine.h"
#include "engine/render/Mesh.h"
#include <GLES2/gl2.h>

static const char *const kGlowShaderVertexSource =
    "attribute highp vec4 \ta_position;  \n"
    "attribute mediump vec2  a_texCoord;    \n"
    "varying mediump vec2 v_texCoord;  \n"
    "uniform highp mat4 u_ModelViewProjectionMatrix;  \n"
    "void main()  \n"
    "{  \n"
    "\tgl_Position = u_ModelViewProjectionMatrix * a_position;  \n"
    "\tv_texCoord  = a_texCoord;    \n"
    "}  \n";

static const char *const kGlowShaderFragmentSource =
    "precision lowp float;  \n"
    "varying mediump vec2 v_texCoord;\n"
    "uniform sampler2D  s_texture;\n"
    "void main()\n"
    "{\n"
    "   gl_FragColor.a = step(0.5/*08*/, texture2D(s_texture, v_texCoord).a); \n"
    "}\n";

namespace AbyssEngine {
    int GlowShader::ShaderIndex;

    GlowShader::GlowShader() {
        GlowShader::ShaderIndex = ShaderBaseStruct::shaderIndexIntern;
        this->name = String("GlowShader", false);
    }

    void GlowShader::Init(Engine *) {
        this->program = this->ES2LoadProgram(kGlowShaderVertexSource, kGlowShaderFragmentSource);

        this->aPositionLoc = glGetAttribLocation(this->program, "a_position");
        this->aTexCoordLoc = glGetAttribLocation(this->program, "a_texCoord");

        this->uModelViewProjectionMatrixLoc = glGetUniformLocation(this->program, "u_ModelViewProjectionMatrix");
        this->uModelMatrixLoc = glGetUniformLocation(this->program, "u_ModelMatrix");
        this->sTextureLoc = glGetUniformLocation(this->program, "s_texture");

        glUseProgram(this->program);
        glUniform1i(this->sTextureLoc, 0);
    }

    void GlowShader::SetInActive() {
        if (this->aPositionLoc >= 0)
            glDisableVertexAttribArray(this->aPositionLoc);
        if (this->aTexCoordLoc >= 0)
            glDisableVertexAttribArray(this->aTexCoordLoc);
    }

    void GlowShader::UpdateMeshData(Mesh *mesh, Engine *engine) {
        if (this->uModelViewProjectionMatrixLoc >= 0)
            glUniformMatrix4fv(this->uModelViewProjectionMatrixLoc, 1, 0, engine->worldViewProjMatrix);
        if (this->uModelMatrixLoc >= 0)
            glUniformMatrix3fv(this->uModelMatrixLoc, 1, 0, engine->normalMatrix);

        if (this->aPositionLoc >= 0)
            glEnableVertexAttribArray(this->aPositionLoc);
        if (this->aTexCoordLoc >= 0)
            glEnableVertexAttribArray(this->aTexCoordLoc);

        if (mesh->uploaded != 0) {
            glBindBuffer(0x8892, mesh->positionVBO);
            glVertexAttribPointer(this->aPositionLoc, 3, 0x1406, 0, 0, 0);
            glBindBuffer(0x8892, mesh->texCoordVBO);
            glVertexAttribPointer(this->aTexCoordLoc, 2, 0x1406, 0, 0, 0);
            return;
        }

        if (this->aPositionLoc >= 0)
            glVertexAttribPointer(this->aPositionLoc, 3, 0x1406, 0, 0, mesh->positions);
        if (this->aTexCoordLoc >= 0)
            glVertexAttribPointer(this->aTexCoordLoc, 2, 0x1406, 0, 0, mesh->texCoords);
    }
}
