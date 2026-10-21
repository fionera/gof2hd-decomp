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


static const char *const kPulseShaderVertexSource =
    "attribute highp vec4 a_position;   \n"
    "attribute mediump vec2 a_texCoord;   \n"
    "varying mediump vec2 v_texCoord;     \n"
    "uniform highp mat4 u_ModelViewProjectionMatrix;  \n"
    "void main()                  \n"
    "{                            \n"
    "   gl_Position = u_ModelViewProjectionMatrix * a_position;\t\t\n"
    "   v_texCoord = a_texCoord;  \n"
    "}                            \n";

static const char *const kPulseShaderFragmentSource =
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
    int PulseShader::ShaderIndex;

    PulseShader::PulseShader() {
        PulseShader::ShaderIndex = ShaderBaseStruct::shaderIndexIntern;
        this->name = String("PulseShader", false);
    }

    void PulseShader::SetInActive() {
        if (this->a0Loc >= 0) glDisableVertexAttribArray(this->a0Loc);
        if (this->a1Loc >= 0) glDisableVertexAttribArray(this->a1Loc);
        if (this->a2Loc >= 0) glDisableVertexAttribArray(this->a2Loc);
        if (this->a3Loc >= 0) glDisableVertexAttribArray(this->a3Loc);
        if (this->a4Loc >= 0) glDisableVertexAttribArray(this->a4Loc);
    }

    void PulseShader::Init(Engine *) {
        int program = this->ES2LoadProgram(kPulseShaderVertexSource, kPulseShaderFragmentSource);
        this->program = program;

        this->a0Loc = glGetAttribLocation(program, "a_position");
        this->a1Loc = glGetAttribLocation(this->program, "a_texCoord");
        this->a2Loc = glGetAttribLocation(this->program, "a_normal");
        this->a3Loc = glGetAttribLocation(this->program, "a_tangent");
        this->a4Loc = glGetAttribLocation(this->program, "a_bitangent");

        this->u0Loc = glGetUniformLocation(this->program, "u_ModelViewProjectionMatrix");
        this->u1Loc = glGetUniformLocation(this->program, "u_ModelMatrix");
        this->u2Loc = glGetUniformLocation(this->program, "u_lightdirmodel");
        this->u3Loc = glGetUniformLocation(this->program, "u_eyeposmodel");
        this->u4Loc = glGetUniformLocation(this->program, "u_timevalue");
        this->u5Loc = glGetUniformLocation(this->program, "s_texture");
        this->u6Loc = glGetUniformLocation(this->program, "glColor");
        this->u7Loc = glGetUniformLocation(this->program, "u_AmbientColor");
        this->u8Loc = glGetUniformLocation(this->program, "u_DiffuseColor");
        this->u9Loc = glGetUniformLocation(this->program, "u_SpecularColor");

        glUseProgram(this->program);
        glUniform1i(this->u5Loc, 0);
    }

    void PulseShader::UpdateMeshData(Mesh *mesh, Engine *engine) {
        if (this->u0Loc >= 0)
            glUniformMatrix4fv(this->u0Loc, 1, 0, engine->worldViewProjMatrix);
        if (this->u1Loc >= 0)
            glUniformMatrix3fv(this->u1Loc, 1, 0, engine->normalMatrix);

        if (this->dirty != 0) {
            if (this->u2Loc >= 0)
                glUniform3f(this->u2Loc, engine->lightDir.x, engine->lightDir.y,
                            engine->lightDir.z);
            if (this->u3Loc >= 0)
                glUniform3f(this->u3Loc, engine->lightColor.x, engine->lightColor.y,
                            engine->lightColor.z);
            if (this->u6Loc >= 0)
                glUniform4fv(this->u6Loc, 1, engine->glColor);
            if (this->u7Loc >= 0)
                glUniform4fv(this->u7Loc, 1, engine->materialAmbient);
            if (this->u8Loc >= 0)
                glUniform4fv(this->u8Loc, 1, engine->materialDiffuse);
            if (this->u9Loc >= 0)
                glUniform4fv(this->u9Loc, 1, engine->materialSpecular);

            float pulse = sinf((float) engine->appManager->GetCurrentTimeMillis() / 1000.0f) + 2.0f;
            glUniform1f(this->u4Loc, pulse);
            this->dirty = 0;
        }

        if (this->a0Loc >= 0) glEnableVertexAttribArray(this->a0Loc);
        if (this->a1Loc >= 0) glEnableVertexAttribArray(this->a1Loc);
        if (this->a2Loc >= 0) glEnableVertexAttribArray(this->a2Loc);
        if (this->a3Loc >= 0) glEnableVertexAttribArray(this->a3Loc);
        if (this->a4Loc >= 0) glEnableVertexAttribArray(this->a4Loc);

        if (mesh->uploaded == 0) {
            if (this->a0Loc >= 0)
                glVertexAttribPointer(this->a0Loc, 3, 0x1406, 0, 0, mesh->positions);
            if (this->a1Loc >= 0)
                glVertexAttribPointer(this->a1Loc, 2, 0x1406, 0, 0, mesh->texCoords);
            if (this->a2Loc >= 0)
                glVertexAttribPointer(this->a2Loc, 3, 0x1406, 0, 0, mesh->normals);
            if (this->a3Loc >= 0)
                glVertexAttribPointer(this->a3Loc, 3, 0x1406, 0, 0, mesh->tangents);
            if (this->a4Loc >= 0)
                glVertexAttribPointer(this->a4Loc, 3, 0x1406, 0, 0, mesh->binormals);
        } else {
            glBindBuffer(0x8892, mesh->positionVBO);
            glVertexAttribPointer(this->a0Loc, 3, 0x1406, 0, 0, 0);
            glBindBuffer(0x8892, mesh->texCoordVBO);
            glVertexAttribPointer(this->a1Loc, 2, 0x1406, 0, 0, 0);
            glBindBuffer(0x8892, mesh->normalVBO);
            glVertexAttribPointer(this->a2Loc, 3, 0x1406, 0, 0, 0);
            glBindBuffer(0x8892, mesh->tangentVBO);
            glVertexAttribPointer(this->a3Loc, 3, 0x1406, 0, 0, 0);
            glBindBuffer(0x8892, mesh->binormalVBO);
            glVertexAttribPointer(this->a4Loc, 3, 0x1406, 0, 0, 0);
        }
    }
}
