#include "engine/render/shaders/NoTexVtxColorShader.h"
#include "engine/render/Engine.h"
#include "engine/render/Mesh.h"
#include <GLES2/gl2.h>

static const char kNoTexVtxColorShaderVertexSource[] =
    "attribute highp vec4 a_position;   \n"
    "attribute mediump vec4 a_VertexColor;   \n"
    "varying mediump vec4 v_VertexColor;     \n"
    "uniform highp mat4 u_WorldMatrix;  \n"
    "void main()                  \n"
    "{                            \n"
    "   gl_Position = u_WorldMatrix * a_position;\t\t\n"
    "   v_VertexColor = a_VertexColor;  \n"
    "}                            \n";

static const char kNoTexVtxColorShaderFragmentSource[] =
    "precision lowp float;                            \n"
    "varying mediump vec4 v_VertexColor;                            \n"
    "uniform lowp vec4 glColor;\t\t\t\t\t\t\t\t \n"
    "void main()                                         \n"
    "{                                                   \n"
    "\tgl_FragColor = glColor * v_VertexColor;\t\t\n"
    "}   \t\t  \n";

namespace AbyssEngine {
    int NoTexVtxColorShader::ShaderIndex = -1;

    void NoTexVtxColorShader::SetInActive() {
        glDisableVertexAttribArray(aPositionLoc);
        glDisableVertexAttribArray(aVertexColorLoc);
    }

    void NoTexVtxColorShader::Init(Engine *) {
        this->program = this->ES2LoadProgram(kNoTexVtxColorShaderVertexSource, kNoTexVtxColorShaderFragmentSource);

        aPositionLoc = glGetAttribLocation(this->program, "a_position");
        aVertexColorLoc = glGetAttribLocation(this->program, "a_VertexColor");

        uWorldMatrixLoc = glGetUniformLocation(this->program, "u_WorldMatrix");
        uGlColorLoc = glGetUniformLocation(this->program, "glColor");

        glUseProgram(this->program);
    }

    NoTexVtxColorShader::NoTexVtxColorShader() {
        NoTexVtxColorShader::ShaderIndex = ShaderBaseStruct::shaderIndexIntern;
        this->name = String("NoTexVtxColorShader", false);
    }

    void NoTexVtxColorShader::UpdateMeshData(Mesh *mesh, Engine *engine) {
        glUniformMatrix4fv(uWorldMatrixLoc, 1, 0, engine->worldViewProjMatrix);
        if (this->dirty != 0) {
            glUniform4fv(uGlColorLoc, 1, engine->glColor);
            this->dirty = 0;
        }
        glEnableVertexAttribArray(aPositionLoc);
        glEnableVertexAttribArray(aVertexColorLoc);

        if (mesh != 0) {
            glVertexAttribPointer(aPositionLoc, 3, 0x1406, 0, 0, mesh->positions);
            glVertexAttribPointer(aVertexColorLoc, 4, 0x1406, 0, 0, mesh->colors);
        } else {
            glVertexAttribPointer(aPositionLoc, 2, 0x1406, 0, 0, *(unsigned char **) &engine->lineVertexBase);
        }
    }
}
