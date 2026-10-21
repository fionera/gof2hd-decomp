#include "engine/render/shaders/NoTexVtxColorShader.h"
#include "engine/render/Engine.h"
#include "engine/render/Mesh.h"
#include <GLES2/gl2.h>

static const char *const kNoTexVtxColorShaderVertexSource =
    "attribute highp vec4 a_position;   \n"
    "attribute mediump vec4 a_VertexColor;   \n"
    "varying mediump vec4 v_VertexColor;     \n"
    "uniform highp mat4 u_WorldMatrix;  \n"
    "void main()                  \n"
    "{                            \n"
    "   gl_Position = u_WorldMatrix * a_position;\t\t\n"
    "   v_VertexColor = a_VertexColor;  \n"
    "}                            \n";

static const char *const kNoTexVtxColorShaderFragmentSource =
    "precision lowp float;                            \n"
    "varying mediump vec4 v_VertexColor;                            \n"
    "uniform lowp vec4 glColor;\t\t\t\t\t\t\t\t \n"
    "void main()                                         \n"
    "{                                                   \n"
    "\tgl_FragColor = glColor * v_VertexColor;\t\t\n"
    "}   \t\t  \n";

namespace AbyssEngine {
    int NoTexVtxColorShader::ShaderIndex;

    void NoTexVtxColorShader::SetInActive() {
        glDisableVertexAttribArray(aPosition);
        glDisableVertexAttribArray(aColor);
    }

    void NoTexVtxColorShader::Init(Engine *) {
        this->program = this->ES2LoadProgram(kNoTexVtxColorShaderVertexSource, kNoTexVtxColorShaderFragmentSource);

        aPosition = glGetAttribLocation(this->program, "a_position");
        aColor = glGetAttribLocation(this->program, "a_VertexColor");

        uMvpMatrix = glGetUniformLocation(this->program, "u_WorldMatrix");
        uColor = glGetUniformLocation(this->program, "glColor");

        glUseProgram(this->program);
    }

    NoTexVtxColorShader::NoTexVtxColorShader() {
        NoTexVtxColorShader::ShaderIndex = ShaderBaseStruct::shaderIndexIntern;
        this->name = String("NoTexVtxColorShader", false);
    }

    void NoTexVtxColorShader::UpdateMeshData(Mesh *mesh, Engine *engine) {
        glUniformMatrix4fv(uMvpMatrix, 1, 0, engine->worldViewProjMatrix);
        if (this->dirty != 0) {
            glUniform4fv(uColor, 1, engine->glColor);
            this->dirty = 0;
        }
        glEnableVertexAttribArray(aPosition);
        glEnableVertexAttribArray(aColor);

        if (mesh != 0) {
            glVertexAttribPointer(aPosition, 3, 0x1406, 0, 0, mesh->positions);
            glVertexAttribPointer(aColor, 4, 0x1406, 0, 0, mesh->colors);
        } else {
            glVertexAttribPointer(aPosition, 2, 0x1406, 0, 0, *(unsigned char **) &engine->lineVertexBase);
        }
    }
}
