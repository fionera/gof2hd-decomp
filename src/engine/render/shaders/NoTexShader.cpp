#include "engine/render/shaders/NoTexShader.h"
#include "engine/render/Engine.h"
#include "engine/render/Mesh.h"
#include <GLES2/gl2.h>

static const char kNoTexShaderVertexSource[] =
    "attribute highp vec4 a_position;   \n"
    "uniform highp mat4 u_WorldMatrix;  \n"
    "void main()                  \n"
    "{                            \n"
    "   gl_Position = u_WorldMatrix * a_position;\t\t\n"
    "}                            \n";

static const char kNoTexShaderFragmentSource[] =
    "precision lowp float;                            \n"
    "uniform lowp vec4 glColor;\t\t\t\t\t\t\t\t \n"
    "void main()                                         \n"
    "{                                                   \n"
    "\t\tgl_FragColor = glColor;\n"
    "}   \t\t  \n";

namespace AbyssEngine {
    int NoTexShader::ShaderIndex = -1;

    void NoTexShader::SetInActive() {
        glDisableVertexAttribArray(this->aPositionLoc);
    }

    void NoTexShader::UpdateMeshData(Mesh *mesh, Engine *engine) {
        glUniformMatrix4fv(this->uWorldMatrixLoc, 1, 0, engine->worldViewProjMatrix);
        if (this->dirty != 0) {
            glUniform4fv(this->uGlColorLoc, 1, engine->glColor);
            this->dirty = 0;
        }
        glEnableVertexAttribArray(this->aPositionLoc);

        if (mesh != 0) {
            if (mesh->uploaded != 0) {
                glBindBuffer(0x8892, mesh->positionVBO);
                glVertexAttribPointer(this->aPositionLoc, 3, 0x1406, 0, 0, 0);
            } else {
                glVertexAttribPointer(this->aPositionLoc, 3, 0x1406, 0, 0, mesh->positions);
            }
        } else {
            glVertexAttribPointer(this->aPositionLoc, 2, 0x1406, 0, 0, *(unsigned char **) &engine->lineVertexBase);
        }
    }

    void NoTexShader::Init(Engine *) {
        this->program = this->ES2LoadProgram(kNoTexShaderVertexSource, kNoTexShaderFragmentSource);
        this->aPositionLoc = glGetAttribLocation(this->program, "a_position");
        this->uWorldMatrixLoc = glGetUniformLocation(this->program, "u_WorldMatrix");
        this->uGlColorLoc = glGetUniformLocation(this->program, "glColor");
        glUseProgram(this->program);
    }

    NoTexShader::NoTexShader() {
        NoTexShader::ShaderIndex = ShaderBaseStruct::shaderIndexIntern;
        this->name = String("NoTexShader", false);
    }
}
