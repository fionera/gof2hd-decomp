#include "engine/render/shaders/ColorMixAdd.h"
#include "engine/render/Engine.h"
#include "engine/render/Mesh.h"
#include <GLES2/gl2.h>

static const char *const kColorMixAddVertexSource =
    "attribute highp vec4 a_position;   \n"
    "attribute mediump vec2 a_texCoord;   \n"
    "varying mediump vec2 v_texCoord;     \n"
    "uniform highp mat4 u_WorldMatrix;  \n"
    "uniform highp mat4 u_UvMatrix;  \n"
    "uniform bool u_UVAnimation; \n"
    "void main()                  \n"
    "{                            \n"
    "   gl_Position = u_WorldMatrix * a_position;\t\t\n"
    "   if ( u_UVAnimation ) \n"
    "       v_texCoord = (u_UvMatrix*vec4(a_texCoord.x, a_texCoord.y, 0.0, 1.0)).xy;  \n"
    "   else \n"
    "       v_texCoord = a_texCoord; \n"
    "}                            \n";

static const char *const kColorMixAddFragmentSource =
    "precision lowp float;                            \n"
    "varying mediump vec2 v_texCoord;                            \n"
    "uniform sampler2D s_texture;                        \n"
    "uniform lowp vec4 glColor;\t\t\t\t\t\t\t\t \n"
    "uniform float u_DarkenValue; \n"
    "void main()                                         \n"
    "{                                                   \n"
    "\tvec4 textureColor = texture2D( s_texture, v_texCoord );\t\n"
    "\tvec3 outColor = glColor.rgb + textureColor.rgb;\t\t\n"
    "   gl_FragColor.rgb = outColor; \n"
    "   gl_FragColor.a = textureColor.a;\n"
    "}   \t\t  \n";

namespace AbyssEngine {
    int ColorMixAdd::ShaderIndex;

    ColorMixAdd::ColorMixAdd() {
        ShaderIndex = ShaderBaseStruct::shaderIndexIntern;
        this->name = String("ColorMixAdd", false);
    }

    void ColorMixAdd::Init(Engine *) {
        int program = this->ES2LoadProgram(kColorMixAddVertexSource, kColorMixAddFragmentSource);
        this->program = program;

        this->u0Loc = glGetUniformLocation(program, "s_texture");
        this->a0Loc = glGetAttribLocation(this->program, "a_position");
        this->a1Loc = glGetAttribLocation(this->program, "a_texCoord");
        this->u1Loc = glGetUniformLocation(this->program, "u_WorldMatrix");
        this->u2Loc = glGetUniformLocation(this->program, "glColor");
        this->u3Loc = glGetUniformLocation(this->program, "u_DarkenValue");
        this->u4Loc = glGetUniformLocation(this->program, "u_UvMatrix");
        this->u5Loc = glGetUniformLocation(this->program, "u_UVAnimation");

        glUseProgram(this->program);
        glUniform1i(this->u0Loc, 0);
    }

    void ColorMixAdd::UpdateMeshData(Mesh *mesh, Engine *engine) {
        glUniformMatrix4fv(this->u1Loc, 1, 0, engine->worldViewProjMatrix);
        if (this->u4Loc >= 0)
            glUniformMatrix4fv(this->u4Loc, 1, 0, engine->uvMatrix);
        if (this->u5Loc >= 0)
            glUniform1i(this->u5Loc, mesh->hasAnimation);

        if (this->dirty != 0) {
            glUniform4fv(this->u2Loc, 1, engine->glColor);
            if (this->u3Loc >= 0)
                glUniform1f(this->u3Loc, 1.0f - *(float *) &mesh->materialId);
            this->dirty = 0;
        }

        if ((mesh->vertexFormat & 2) != 0) {
            glEnableVertexAttribArray(this->a0Loc);
            glEnableVertexAttribArray(this->a1Loc);
            if (mesh->uploaded == 0) {
                glVertexAttribPointer(this->a0Loc, 3, 0x1406, 0, 0, mesh->positions);
                glVertexAttribPointer(this->a1Loc, 2, 0x1406, 0, 0, mesh->texCoords);
            } else {
                glBindBuffer(0x8892, mesh->positionVBO);
                glVertexAttribPointer(this->a0Loc, 3, 0x1406, 0, 0, 0);
                glBindBuffer(0x8892, mesh->texCoordVBO);
                glVertexAttribPointer(this->a1Loc, 2, 0x1406, 0, 0, 0);
            }
        }
    }

    void ColorMixAdd::SetInActive() {
        for (int i = 2; i != 0; i = i - 1) {
            glDisableVertexAttribArray(this->a0Loc);
            glDisableVertexAttribArray(this->a1Loc);
        }
    }
}
