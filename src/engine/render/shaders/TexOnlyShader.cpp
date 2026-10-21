#include "engine/render/shaders/TexOnlyShader.h"
#include "engine/render/Engine.h"
#include "engine/render/Mesh.h"
#include <GLES2/gl2.h>

static const char *const kTexOnlyShaderVertexSource =
    "attribute highp vec4 a_position;\n"
    "attribute mediump vec2 a_texCoord;\n"
    "varying mediump vec2 v_texCoord;\n"
    "uniform highp mat4 u_WorldMatrix;\n"
    "void main()\n"
    "{\n"
    "\tgl_Position = u_WorldMatrix * a_position;\n"
    "\tv_texCoord = a_texCoord;\n"
    "}\n";

static const char *const kTexOnlyShaderFragmentSource =
    "precision lowp float;\n"
    "varying mediump vec2 v_texCoord;\n"
    "uniform sampler2D s_texture_base;\n"
    "void main()\n"
    "{\n"
    "\tvec4 textureColor = texture2D( s_texture_base, v_texCoord );\n"
    "\tgl_FragColor = textureColor; // * vec4(4.0,4.0,4.0,1.0);\n"
    "}\n";

namespace AbyssEngine {
    int TexOnlyShader::ShaderIndex;

    TexOnlyShader::TexOnlyShader() {
        TexOnlyShader::ShaderIndex = ShaderBaseStruct::shaderIndexIntern;
        this->name = String("TexOnlyShader", false);
    }

    void TexOnlyShader::Init(Engine *) {
        this->program = (int) this->ES2LoadProgram(kTexOnlyShaderVertexSource, kTexOnlyShaderFragmentSource);
        this->aPosition = glGetAttribLocation(this->program, "a_position");
        this->aTexCoord = glGetAttribLocation(this->program, "a_texCoord");
        this->uWorldMatrix = glGetUniformLocation(this->program, "u_WorldMatrix");
        this->sTexture = glGetUniformLocation(this->program, "s_texture_base");
        glUseProgram(this->program);
        glUniform1i(this->sTexture, 0);
    }

    void TexOnlyShader::SetInActive() {
        glDisableVertexAttribArray(this->aPosition);
        glDisableVertexAttribArray(this->aTexCoord);
    }

    void TexOnlyShader::UpdateMeshData(Mesh *mesh, Engine *engine) {
        glUniformMatrix4fv(this->uWorldMatrix, 1, 0, engine->worldViewProjMatrix);
        if (this->dirty != 0) {
            this->dirty = 0;
        }
        glEnableVertexAttribArray(this->aPosition);
        glEnableVertexAttribArray(this->aTexCoord);

        if (mesh->uploaded == 0) {
            glVertexAttribPointer(this->aPosition, 3, 0x1406, 0, 0, mesh->positions);
            glVertexAttribPointer(this->aTexCoord, 2, 0x1406, 0, 0, mesh->texCoords);
        } else {
            glBindBuffer(0x8892, mesh->positionVBO);
            glVertexAttribPointer(this->aPosition, 3, 0x1406, 0, 0, 0);
            glBindBuffer(0x8892, mesh->texCoordVBO);
            glVertexAttribPointer(this->aTexCoord, 2, 0x1406, 0, 0, 0);
        }
    }
}
