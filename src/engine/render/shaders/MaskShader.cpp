#include "engine/render/shaders/MaskShader.h"

#include "engine/render/Engine.h"
#include "engine/render/Mesh.h"
#include <GLES2/gl2.h>

static const char *const kMaskShaderVertexSource =
    "attribute highp vec4 a_position;   \n"
    "attribute mediump vec2 a_texCoord_0;   \n"
    "attribute mediump vec2 a_texCoord_1;   \n"
    "varying mediump vec2 v_texCoord[2];     \n"
    "uniform highp mat4 u_WorldMatrix;  \n"
    "void main()                  \n"
    "{                            \n"
    "   gl_Position = u_WorldMatrix * a_position;\t\t\n"
    "   v_texCoord[0] = a_texCoord_0;  \n"
    "   v_texCoord[1] = a_texCoord_1;  \n"
    "}                            \n";

static const char *const kMaskShaderFragmentSource =
    "precision lowp float;                            \n"
    "varying mediump vec2 v_texCoord[2];                     \n"
    "uniform sampler2D s_texture[2];                        \n"
    "uniform lowp vec4 glColor;\t\t\t\t\t\t\t\t \n"
    "void main()                                         \n"
    "{                                                   \n"
    "\tvec4 textureColorBase = texture2D( s_texture[0], v_texCoord[0] );\t\n"
    "\tvec4 textureColorMask = texture2D( s_texture[1], v_texCoord[1] );\t\n"
    "\tgl_FragColor = glColor * textureColorBase;\t\t\n"
    "\tgl_FragColor.a = glColor.a * (1.0 - textureColorMask.a) * textureColorBase.a;\t\t\n"
    "}   \t\t  \n";

namespace AbyssEngine {
    int MaskShader::ShaderIndex;

    MaskShader::MaskShader() {
        ShaderIndex = ShaderBaseStruct::shaderIndexIntern;
        this->name = String("MaskShader", false);
    }

    void MaskShader::Init(::Engine *) {
        this->program = (int) ES2LoadProgram(kMaskShaderVertexSource, kMaskShaderFragmentSource);

        this->a_position = glGetAttribLocation(this->program, "a_position");
        this->a_texCoord = glGetAttribLocation(this->program, "a_texCoord_0");
        this->a_color = glGetAttribLocation(this->program, "a_texCoord_1");

        this->u_mvpMatrix = glGetUniformLocation(this->program, "u_WorldMatrix");
        this->u_texture0 = glGetUniformLocation(this->program, "s_texture[0]");
        this->u_texture1 = glGetUniformLocation(this->program, "s_texture[1]");
        this->u_color = glGetUniformLocation(this->program, "glColor");

        glUseProgram(this->program);
        for (int i = 0; i != 2; i++) {
            int loc = (&this->u_texture0)[i];
            if (loc >= 0)
                glUniform1i(loc, i);
        }
    }

    void MaskShader::SetInActive() {
        if (this->a_position >= 0)
            glDisableVertexAttribArray(this->a_position);
        if (this->a_texCoord >= 0)
            glDisableVertexAttribArray(this->a_texCoord);
        if (this->a_color >= 0)
            glDisableVertexAttribArray(this->a_color);
    }

    void MaskShader::UpdateMeshData(Mesh *mesh, ::Engine *engine) {
        if (this->u_mvpMatrix >= 0)
            glUniformMatrix4fv(this->u_mvpMatrix, 1, 0, engine->worldViewProjMatrix);

        if (this->dirty != 0) {
            if (this->u_color >= 0)
                glUniform4fv(this->u_color, 1, engine->glColor);
            this->dirty = 0;
        }

        if (this->a_position >= 0)
            glEnableVertexAttribArray(this->a_position);
        if (this->a_texCoord >= 0)
            glEnableVertexAttribArray(this->a_texCoord);
        if (this->a_color >= 0)
            glEnableVertexAttribArray(this->a_color);

        char *texBase = *(char **) engine->appManager;
        int *tex = *(int **) (texBase + 0x20);
        if (tex != 0)
            ((::Engine *) engine)->SetTextureSlot(tex[1], 1);

        if (mesh->uploaded == 0) {
            if (this->a_position >= 0)
                glVertexAttribPointer(this->a_position, 3, 0x1406, 0, 0, mesh->positions);
            if (this->a_texCoord >= 0)
                glVertexAttribPointer(this->a_texCoord, 2, 0x1406, 0, 0, mesh->texCoords);
            if (tex == 0)
                return;
            if (this->a_color < 0)
                return;
            glVertexAttribPointer(this->a_color, 2, 0x1406, 0, 0, *(unsigned char **) (*tex + 8));
        } else {
            glBindBuffer(0x8892, mesh->positionVBO);
            glVertexAttribPointer(this->a_position, 3, 0x1406, 0, 0, 0);
            glBindBuffer(0x8892, mesh->texCoordVBO);
            glVertexAttribPointer(this->a_texCoord, 2, 0x1406, 0, 0, 0);
            if (tex == 0)
                return;
            glBindBuffer(0x8892, *(uint32_t *) (*tex + 0x68));
            glVertexAttribPointer(this->a_color, 2, 0x1406, 0, 0, 0);
        }
    }
}
