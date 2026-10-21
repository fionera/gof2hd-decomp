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
        if (aPositionLoc >= 0)
            glDisableVertexAttribArray(aPositionLoc);
        if (aTexCoordLoc >= 0)
            glDisableVertexAttribArray(aTexCoordLoc);
        if (aVertexColorLoc >= 0)
            glDisableVertexAttribArray(aVertexColorLoc);
    }

    BumpShaderRefract::BumpShaderRefract() {
        ShaderIndex = ShaderBaseStruct::shaderIndexIntern;
        this->name = String("BumpShaderRefract", false);
    }

    void BumpShaderRefract::Init(::Engine *engine) {
        int program = this->ES2LoadProgram(kBumpShaderRefractVertexSource, kBumpShaderRefractFragmentSource);
        this->program = program;

        aPositionLoc = glGetAttribLocation(program, "a_position");
        aTexCoordLoc = glGetAttribLocation(this->program, "a_texCoord");

        aVertexColorLoc = glGetAttribLocation(this->program, "a_VertexColor");
        uModelViewProjectionMatrixLoc = glGetUniformLocation(this->program, "u_ModelViewProjectionMatrix");
        sTexture0Loc = glGetUniformLocation(this->program, "s_texture[0]");
        sTexture1Loc = glGetUniformLocation(this->program, "s_texture[1]");
        uTexelSizeLoc = glGetUniformLocation(this->program, "u_texelSize");
        uUvMatrixLoc = glGetUniformLocation(this->program, "u_UvMatrix");
        uUVAnimationLoc = glGetUniformLocation(this->program, "u_UVAnimation");
        uGlColorLoc = glGetUniformLocation(this->program, "glColor");

        glActiveTexture(0x84c7);
        engine->ActivateRefractFBO();
        sTextureBaseLoc = glGetUniformLocation(this->program, "s_texture_base");

        glUseProgram(this->program);

        int *samplers = &sTexture0Loc;
        for (int i = 0; i != 2; i++) {
            int loc = samplers[i];
            if (loc >= 0)
                glUniform1i(loc, i);
        }
        glUniform1i(sTextureBaseLoc, 7);
    }

    void BumpShaderRefract::UpdateMeshData(Mesh *mesh, ::Engine *engine) {
        if (uModelViewProjectionMatrixLoc >= 0)
            glUniformMatrix4fv(uModelViewProjectionMatrixLoc, 1, 0, engine->worldViewProjMatrix);
        if (uUvMatrixLoc >= 0)
            glUniformMatrix4fv(uUvMatrixLoc, 1, 0, engine->uvMatrix);

        if (this->dirty != 0) {
            if (uUVAnimationLoc >= 0)
                glUniform1i(uUVAnimationLoc, mesh->hasAnimation);
            glUniform4fv(uGlColorLoc, 1, engine->glColor);
            int loc = uTexelSizeLoc;
            if (loc >= 0) {
                float w = (float) (int) engine->GetDisplayWidth();
                float h = (float) (int) engine->GetDisplayHeight();
                glUniform2f(loc, 1.0f / w, 1.0f / h);
            }
            glActiveTexture(0x84c7);
            engine->ActivateRefractFBO();
            this->dirty = 0;
        }

        if (aPositionLoc >= 0)
            glEnableVertexAttribArray(aPositionLoc);
        if (aTexCoordLoc >= 0)
            glEnableVertexAttribArray(aTexCoordLoc);
        if (aVertexColorLoc >= 0)
            glEnableVertexAttribArray(aVertexColorLoc);

        if (mesh->uploaded != 0) {
            if (aPositionLoc >= 0) {
                glBindBuffer(0x8892, mesh->positionVBO);
                glVertexAttribPointer(aPositionLoc, 3, 0x1406, 0, 0, 0);
            }
            if (aTexCoordLoc >= 0) {
                glBindBuffer(0x8892, mesh->texCoordVBO);
                glVertexAttribPointer(aTexCoordLoc, 2, 0x1406, 0, 0, 0);
            }
            if (aVertexColorLoc >= 0) {
                glBindBuffer(0x8892, mesh->colorVBO);
                glVertexAttribPointer(aVertexColorLoc, 4, 0x1406, 0, 0, 0);
            }
        } else {
            if (aPositionLoc >= 0)
                glVertexAttribPointer(aPositionLoc, 3, 0x1406, 0, 0, mesh->positions);
            if (aTexCoordLoc >= 0)
                glVertexAttribPointer(aTexCoordLoc, 2, 0x1406, 0, 0, mesh->texCoords);
            if (aVertexColorLoc >= 0)
                glVertexAttribPointer(aVertexColorLoc, 4, 0x1406, 0, 0, mesh->colors);
        }
    }
}
