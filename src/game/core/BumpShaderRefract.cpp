#include "game/core/BumpShaderRefract.h"

#include "engine/render/Engine.h"
#include <GLES2/gl2.h>

static const char *const kBumpShaderRefractVertexSource =
    "attribute highp vec3 a_position;\n"
    "attribute mediump vec2 a_texCoord;\n"
    "attribute mediump vec4 a_VertexColor;   \n"
    "varying mediump vec2 v_texCoord;  \n"
    "varying mediump vec4 v_VertexColor;     \n"
    "uniform highp mat4 u_ModelViewProjectionMatrix;\n"
    "uniform highp mat4 u_UvMatrix;  \n"
    "uniform bool u_UVAnimation; \n"
    "void main()  \n"
    "{  \n"
    "\tgl_Position = u_ModelViewProjectionMatrix * vec4(a_position, 1.0);\n"
    "   if ( u_UVAnimation ) \n"
    "       v_texCoord = (u_UvMatrix*vec4(a_texCoord.x, a_texCoord.y, 0.0, 1.0)).xy;  \n"
    "   else \n"
    "       v_texCoord = a_texCoord; \n"
    "   v_VertexColor = a_VertexColor;  \n"
    "}\n";

static const char *const kBumpShaderRefractFragmentSource =
    "precision lowp float;  \n"
    "varying mediump vec2 v_texCoord;\n"
    "varying mediump vec4 v_VertexColor;     \n"
    "uniform sampler2D  s_texture[2];\n"
    "uniform mediump vec2 u_texelSize;\n"
    "uniform sampler2D s_texture_base;\n"
    "uniform lowp vec4 glColor;\t\t\t\t\t\t\t\t \n"
    "void main()\n"
    "{\n"
    "\tlowp vec4 colorTex   = (texture2D( s_texture[0], v_texCoord) * glColor);\n"
    "\tmediump vec4 normalTex  = texture2D( s_texture[1], v_texCoord);\n"
    "\tmediump vec3 normalTexX = normalTex.rgb * 2.0 - 1.0;\n"
    "   mediump vec2 coord;\n"
    "   mediump vec2 coord1;\n"
    "   coord1.x = (gl_FragCoord.x ) * u_texelSize.x;\n"
    "   coord1.y = (gl_FragCoord.y ) * u_texelSize.y;\n"
    "   coord.x = (gl_FragCoord.x + glColor.r*v_VertexColor.r*(normalTexX.x*40.0 * (0.75))) * u_texelSize.x;\n"
    "   coord.y = (gl_FragCoord.y + glColor.r*v_VertexColor.r*(normalTexX.y*30.0 * (0.75))) * u_texelSize.y;\n"
    "\tgl_FragColor.rgb = (colorTex.rgb*v_VertexColor.rgb + texture2D(s_texture_base, coord).rgb) - texture2D(s_texture_base, coord1).rgb;\n"
    "}\n";

namespace AbyssEngine {
    int BumpShaderRefract::ShaderIndex;

    void BumpShaderRefract::SetInActive() {
        if (aPosition >= 0)
            glDisableVertexAttribArray(aPosition);
        if (aTexCoord >= 0)
            glDisableVertexAttribArray(aTexCoord);
        if (uSampler0 >= 0)
            glDisableVertexAttribArray(uSampler0);
    }

    BumpShaderRefract::BumpShaderRefract() {
        ShaderIndex = ShaderBaseStruct::shaderIndexIntern;
        this->name = String("BumpShaderRefract", false);
    }

    void BumpShaderRefract::Init(::Engine *engine) {
        int program = this->ES2LoadProgram(kBumpShaderRefractVertexSource, kBumpShaderRefractFragmentSource);
        this->program = program;

        aPosition = glGetAttribLocation(program, "a_position");
        aTexCoord = glGetAttribLocation(this->program, "a_texCoord");

        uSampler0 = glGetUniformLocation(this->program, "a_VertexColor");
        uMvpMatrix = glGetUniformLocation(this->program, "u_ModelViewProjectionMatrix");
        uSampler2 = glGetUniformLocation(this->program, "s_texture[0]");
        uSampler3 = glGetUniformLocation(this->program, "s_texture[1]");
        uPixelSize = glGetUniformLocation(this->program, "u_texelSize");
        uViewMatrix = glGetUniformLocation(this->program, "u_UvMatrix");
        uColorIndex = glGetUniformLocation(this->program, "u_UVAnimation");
        uColor = glGetUniformLocation(this->program, "glColor");

        glActiveTexture(0x84c7);
        engine->ActivateRefractFBO();
        uRefractSampler = glGetUniformLocation(this->program, "s_texture_base");

        glUseProgram(this->program);

        int *samplers = &uSampler2;
        for (int i = 0; i != 2; i++) {
            int loc = samplers[i];
            if (loc >= 0)
                glUniform1i(loc, i);
        }
        glUniform1i(uRefractSampler, 7);
    }

    void BumpShaderRefract::UpdateMeshData(Mesh *mesh, ::Engine *engine) {
        char *e = (char *) engine;
        char *m = (char *) mesh;

        if (uMvpMatrix >= 0)
            glUniformMatrix4fv(uMvpMatrix, 1, 0, (float *) (e + 0x104));
        if (uViewMatrix >= 0)
            glUniformMatrix4fv(uViewMatrix, 1, 0, (float *) (e + 0x1c4));

        if (this->dirty != 0) {
            if (uColorIndex >= 0)
                glUniform1i(uColorIndex, *(uint8_t *) (m + 0x85));
            glUniform4fv(uColor, 1, (float *) (e + 0xd0));
            int loc = uPixelSize;
            if (loc >= 0) {
                float w = (float) engine->GetDisplayWidth();
                float h = (float) engine->GetDisplayHeight();
                glUniform2f(loc, 1.0f / w, 1.0f / h);
            }
            glActiveTexture(0x84c7);
            engine->ActivateRefractFBO();
            this->dirty = 0;
        }

        if (aPosition >= 0)
            glEnableVertexAttribArray(aPosition);
        if (aTexCoord >= 0)
            glEnableVertexAttribArray(aTexCoord);
        if (uSampler0 >= 0)
            glEnableVertexAttribArray(uSampler0);

        int loc0 = aPosition;
        const float *ptr;
        int last;
        if (*(uint8_t *) (m + 0x5c) == 0) {
            if (loc0 >= 0)
                glVertexAttribPointer(loc0, 3, 0x1406, 0, 0, *(float **) (m + 0x4));
            if (aTexCoord >= 0)
                glVertexAttribPointer(aTexCoord, 2, 0x1406, 0, 0, *(float **) (m + 0x8));
            last = uSampler0;
            if (last < 0)
                return;
            ptr = *(float **) (m + 0xc);
        } else {
            if (loc0 >= 0) {
                glBindBuffer(0x8892, *(uint32_t *) (m + 0x60));
                glVertexAttribPointer(aPosition, 3, 0x1406, 0, 0, 0);
            }
            if (aTexCoord >= 0) {
                glBindBuffer(0x8892, *(uint32_t *) (m + 0x68));
                glVertexAttribPointer(aTexCoord, 2, 0x1406, 0, 0, 0);
            }
            if (uSampler0 < 0)
                return;
            glBindBuffer(0x8892, *(uint32_t *) (m + 0x78));
            ptr = 0;
            last = uSampler0;
        }
        glVertexAttribPointer(last, 4, 0x1406, 0, 0, ptr);
    }
}
