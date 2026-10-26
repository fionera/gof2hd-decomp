#include "engine/render/shaders/PulseShader.h"
#include "game/core/Globals.h"
#include "game/core/String.h"
#include "engine/render/Mesh.h"
#include "engine/render/Engine.h"
#include "engine/core/ApplicationManager.h"
#include <GLES2/gl2.h>

extern "C" { // lint: extern_c (native ABI boundary; original exports the symbol unmangled / GL+libc C ABI)
float sinf(float x);
}


static const char kPulseShaderVertexSource[] =
    "attribute highp vec4 a_position;   \n"
    "attribute mediump vec2 a_texCoord;   \n"
    "varying mediump vec2 v_texCoord;     \n"
    "uniform highp mat4 u_ModelViewProjectionMatrix;  \n"
    "void main()                  \n"
    "{                            \n"
    "   gl_Position = u_ModelViewProjectionMatrix * a_position;\t\t\n"
    "   v_texCoord = a_texCoord;  \n"
    "}                            \n";

static const char kPulseShaderFragmentSource[] =
    "precision lowp float;\n"
    "varying mediump vec2 v_texCoord;\n"
    "uniform sampler2D  s_texture;\n"
    "uniform highp float u_timevalue;\n"
    "void main()\n"
    "{\n"
    "\tvec4 textureColor = texture2D(  s_texture, v_texCoord );\n"
    "\tgl_FragColor = textureColor * vec4(vec3( u_timevalue ), 1.0); // * vec4(4.0,4.0,4.0,1.0);\n"
    "}\n";

namespace AbyssEngine {
    int PulseShader::ShaderIndex = -1;

    PulseShader::PulseShader() {
        PulseShader::ShaderIndex = ShaderBaseStruct::shaderIndexIntern;
        this->name = String("PulseShader", false);
    }

    void PulseShader::SetInActive() {
        if (this->aPositionLoc >= 0) glDisableVertexAttribArray(this->aPositionLoc);
        if (this->aTexCoordLoc >= 0) glDisableVertexAttribArray(this->aTexCoordLoc);
        if (this->aNormalLoc >= 0) glDisableVertexAttribArray(this->aNormalLoc);
        if (this->aTangentLoc >= 0) glDisableVertexAttribArray(this->aTangentLoc);
        if (this->aBitangentLoc >= 0) glDisableVertexAttribArray(this->aBitangentLoc);
    }

    void PulseShader::Init(Engine *) {
        int program = this->ES2LoadProgram(kPulseShaderVertexSource, kPulseShaderFragmentSource);
        this->program = program;

        this->aPositionLoc = glGetAttribLocation(program, "a_position");
        this->aTexCoordLoc = glGetAttribLocation(this->program, "a_texCoord");
        this->aNormalLoc = glGetAttribLocation(this->program, "a_normal");
        this->aTangentLoc = glGetAttribLocation(this->program, "a_tangent");
        this->aBitangentLoc = glGetAttribLocation(this->program, "a_bitangent");

        this->uModelViewProjectionMatrixLoc = glGetUniformLocation(this->program, "u_ModelViewProjectionMatrix");
        this->uModelMatrixLoc = glGetUniformLocation(this->program, "u_ModelMatrix");
        this->uLightdirmodelLoc = glGetUniformLocation(this->program, "u_lightdirmodel");
        this->uEyeposmodelLoc = glGetUniformLocation(this->program, "u_eyeposmodel");
        this->uTimevalueLoc = glGetUniformLocation(this->program, "u_timevalue");
        this->sTextureLoc = glGetUniformLocation(this->program, "s_texture");
        this->uGlColorLoc = glGetUniformLocation(this->program, "glColor");
        this->uAmbientColorLoc = glGetUniformLocation(this->program, "u_AmbientColor");
        this->uDiffuseColorLoc = glGetUniformLocation(this->program, "u_DiffuseColor");
        this->uSpecularColorLoc = glGetUniformLocation(this->program, "u_SpecularColor");

        glUseProgram(this->program);
        glUniform1i(this->sTextureLoc, 0);
    }

    void PulseShader::UpdateMeshData(Mesh *mesh, Engine *engine) {
        if (this->uModelViewProjectionMatrixLoc >= 0)
            glUniformMatrix4fv(this->uModelViewProjectionMatrixLoc, 1, 0, engine->worldViewProjMatrix);
        if (this->uModelMatrixLoc >= 0)
            glUniformMatrix3fv(this->uModelMatrixLoc, 1, 0, engine->normalMatrix);

        if (this->dirty != 0) {
            if (this->uLightdirmodelLoc >= 0)
                glUniform3f(this->uLightdirmodelLoc, engine->lightDir[0].x, engine->lightDir[0].y,
                            engine->lightDir[0].z);
            if (this->uEyeposmodelLoc >= 0)
                glUniform3f(this->uEyeposmodelLoc, engine->lightColor.x, engine->lightColor.y,
                            engine->lightColor.z);
            if (this->uGlColorLoc >= 0)
                glUniform4fv(this->uGlColorLoc, 1, engine->glColor);
            if (this->uAmbientColorLoc >= 0)
                glUniform4fv(this->uAmbientColorLoc, 1, engine->materialAmbient);
            if (this->uDiffuseColorLoc >= 0)
                glUniform4fv(this->uDiffuseColorLoc, 1, engine->materialDiffuse);
            if (this->uSpecularColorLoc >= 0)
                glUniform4fv(this->uSpecularColorLoc, 1, engine->materialSpecular);

            float pulse = sinf((float) engine->appManager->GetCurrentTimeMillis() / 1000.0f) + 2.0f;
            glUniform1f(this->uTimevalueLoc, pulse);
            this->dirty = 0;
        }

        if (this->aPositionLoc >= 0) glEnableVertexAttribArray(this->aPositionLoc);
        if (this->aTexCoordLoc >= 0) glEnableVertexAttribArray(this->aTexCoordLoc);
        if (this->aNormalLoc >= 0) glEnableVertexAttribArray(this->aNormalLoc);
        if (this->aTangentLoc >= 0) glEnableVertexAttribArray(this->aTangentLoc);
        if (this->aBitangentLoc >= 0) glEnableVertexAttribArray(this->aBitangentLoc);

        if (mesh->uploaded == 0) {
            if (this->aPositionLoc >= 0)
                glVertexAttribPointer(this->aPositionLoc, 3, 0x1406, 0, 0, mesh->positions);
            if (this->aTexCoordLoc >= 0)
                glVertexAttribPointer(this->aTexCoordLoc, 2, 0x1406, 0, 0, mesh->texCoords);
            if (this->aNormalLoc >= 0)
                glVertexAttribPointer(this->aNormalLoc, 3, 0x1406, 0, 0, mesh->normals);
            if (this->aTangentLoc >= 0)
                glVertexAttribPointer(this->aTangentLoc, 3, 0x1406, 0, 0, mesh->tangents);
            if (this->aBitangentLoc >= 0)
                glVertexAttribPointer(this->aBitangentLoc, 3, 0x1406, 0, 0, mesh->binormals);
        } else {
            glBindBuffer(0x8892, mesh->positionVBO);
            glVertexAttribPointer(this->aPositionLoc, 3, 0x1406, 0, 0, 0);
            glBindBuffer(0x8892, mesh->texCoordVBO);
            glVertexAttribPointer(this->aTexCoordLoc, 2, 0x1406, 0, 0, 0);
            glBindBuffer(0x8892, mesh->normalVBO);
            glVertexAttribPointer(this->aNormalLoc, 3, 0x1406, 0, 0, 0);
            glBindBuffer(0x8892, mesh->tangentVBO);
            glVertexAttribPointer(this->aTangentLoc, 3, 0x1406, 0, 0, 0);
            glBindBuffer(0x8892, mesh->binormalVBO);
            glVertexAttribPointer(this->aBitangentLoc, 3, 0x1406, 0, 0, 0);
        }
    }
}
