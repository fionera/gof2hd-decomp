#include "engine/render/shaders/SimpleShader.h"
#include "engine/render/Engine.h"
#include "engine/render/Mesh.h"
#include <GLES2/gl2.h>

static const char *const kSimpleShaderVertexSource =
    "attribute highp vec4 a_position; uniform highp mat4 u_WorldMatrix;void main()                 {                          \tgl_Position = u_WorldMatrix * a_position;\t\t} ";

static const char *const kSimpleShaderFragmentSource =
    "precision lowp float;uniform lowp vec4 glColor;void main()  {   gl_FragColor = glColor;} ";

namespace AbyssEngine {
    int SimpleShader::ShaderIndex = -1;

    SimpleShader::SimpleShader() {
        SimpleShader::ShaderIndex = ShaderBaseStruct::shaderIndexIntern;
        this->name = String("SimpleShader", false);
    }

    void SimpleShader::Init(Engine *) {
        this->program = (int) this->ES2LoadProgram(kSimpleShaderVertexSource, kSimpleShaderFragmentSource);
        this->aPositionLoc = glGetAttribLocation(this->program, "a_position");
        this->uWorldMatrixLoc = glGetUniformLocation(this->program, "u_WorldMatrix");
        this->uGlColorLoc = glGetUniformLocation(this->program, "glColor");
        glUseProgram(this->program);
    }

    void SimpleShader::SetInActive() {
        glDisableVertexAttribArray(this->aPositionLoc);
    }

    void SimpleShader::UpdateMeshData(Mesh *mesh, Engine *engine) {
        glUniformMatrix4fv(this->uWorldMatrixLoc, 1, 0, engine->worldViewProjMatrix);
        if (this->dirty != 0) {
            glUniform4fv(this->uGlColorLoc, 1, engine->glColor);
            this->dirty = 0;
        }
        glEnableVertexAttribArray(this->aPositionLoc);
        if (mesh->uploaded != 0) {
            glBindBuffer(0x8892, mesh->positionVBO);
            glVertexAttribPointer(this->aPositionLoc, 3, 0x1406, 0, 0, 0);
            return;
        }
        glVertexAttribPointer(this->aPositionLoc, 3, 0x1406, 0, 0, mesh->positions);
    }
}
