#include "engine/render/shaders/DNSShader.h"
#include "engine/render/Engine.h"
#include "engine/render/Mesh.h"
#include "game/core/String.h"
#include <GLES2/gl2.h>


static const char *const kDNSShaderVertexSource =
    "attribute highp vec3 \ta_position;  \n"
    "uniform highp mat4 u_ModelViewProjectionMatrix;  \n"
    "void main()  \n"
    "{  \n"
    "\tgl_Position = u_ModelViewProjectionMatrix * vec4(a_position, 1.0);  \n"
    "}  \n";

static const char *const kDNSShaderFragmentSource =
    "precision lowp float;  \n"
    "void main()  \n"
    "{  \n"
    "       gl_FragColor = vec4(0.5, 0.5, 0.5, 1.0); \n"
    "}  \n";

namespace AbyssEngine {
    int DNSShader::ShaderIndex;

    DNSShader::DNSShader() {
        DNSShader::ShaderIndex = ShaderBaseStruct::shaderIndexIntern;
        this->name = String("DNSShader", false);
    }

    void DNSShader::Init(Engine *) {
        this->program = this->LoadBindShader("data/shader/DNSShader.vs", "data/shader/DNSShader.fs");
        if (this->program == 0)
            this->program = this->ES2LoadProgram(kDNSShaderVertexSource, kDNSShaderFragmentSource);

        this->aPositionLoc = glGetAttribLocation(this->program, "a_position");
        this->aTexCoordLoc = glGetAttribLocation(this->program, "a_texCoord");
        this->aNormalLoc = glGetAttribLocation(this->program, "a_normal");
        this->aTangentLoc = glGetAttribLocation(this->program, "a_tangent");
        this->aBitangentLoc = glGetAttribLocation(this->program, "a_bitangent");

        this->uModelViewProjectionMatrixLoc = glGetUniformLocation(this->program, "u_ModelViewProjectionMatrix");
        this->uModelMatrixLoc = glGetUniformLocation(this->program, "u_ModelMatrix");
        this->uModelMatrixFullLoc = glGetUniformLocation(this->program, "u_ModelMatrixFull");
        this->uLightdirmodelLoc = glGetUniformLocation(this->program, "u_lightdirmodel");
        this->uEyeposmodelLoc = glGetUniformLocation(this->program, "u_eyeposmodel");
        this->uGlColorLoc = glGetUniformLocation(this->program, "glColor");
        this->uAmbientColorLoc = glGetUniformLocation(this->program, "u_AmbientColor");
        this->uDiffuseColorLoc = glGetUniformLocation(this->program, "u_DiffuseColor");
        this->uSpecularColorLoc = glGetUniformLocation(this->program, "u_SpecularColor");
        this->uSpecularPowerLoc = glGetUniformLocation(this->program, "u_SpecularPower");
        this->uTexBiasDiffuseLoc = glGetUniformLocation(this->program, "u_TexBiasDiffuse");
        this->uTexBiasNormalLoc = glGetUniformLocation(this->program, "u_TexBiasNormal");

        glUseProgram(this->program);
    }

    void DNSShader::SetInActive() {
        if (this->aPositionLoc >= 0)
            glDisableVertexAttribArray(this->aPositionLoc);
        if (this->aTexCoordLoc >= 0)
            glDisableVertexAttribArray(this->aTexCoordLoc);
        if (this->aNormalLoc >= 0)
            glDisableVertexAttribArray(this->aNormalLoc);
        if (this->aTangentLoc >= 0)
            glDisableVertexAttribArray(this->aTangentLoc);
        if (this->aBitangentLoc >= 0)
            glDisableVertexAttribArray(this->aBitangentLoc);
    }

    void DNSShader::UpdateMeshData(Mesh *mesh, Engine *engine) {
        if (this->uModelViewProjectionMatrixLoc >= 0)
            glUniformMatrix4fv(this->uModelViewProjectionMatrixLoc, 1, 0, engine->worldViewProjMatrix);
        if (this->uModelMatrixLoc >= 0)
            glUniformMatrix3fv(this->uModelMatrixLoc, 1, 0, engine->normalMatrix);
        if (this->uModelMatrixFullLoc >= 0)
            glUniformMatrix4fv(this->uModelMatrixFullLoc, 1, 0, engine->modelMatrixGL);
        if (this->uTexBiasDiffuseLoc >= 0)
            glUniform1f(this->uTexBiasDiffuseLoc, Engine::lodBiasDiffuse);
        if (this->uTexBiasNormalLoc >= 0)
            glUniform1f(this->uTexBiasNormalLoc, Engine::lodBiasNormal);

        if (this->dirty != 0) {
            glUniform3f(this->uLightdirmodelLoc, engine->lightDir.x, engine->lightDir.y,
                        engine->lightDir.z);
            if (this->uEyeposmodelLoc >= 0)
                glUniform3f(this->uEyeposmodelLoc, engine->lightColor.x, engine->lightColor.y,
                            engine->lightColor.z);
            if (this->uGlColorLoc >= 0)
                glUniform4fv(this->uGlColorLoc, 1, engine->glColor);
            if (this->uAmbientColorLoc >= 0)
                glUniform3fv(this->uAmbientColorLoc, 1, (float *) engine->ambientShaded[0]);
            if (this->uDiffuseColorLoc >= 0)
                glUniform3fv(this->uDiffuseColorLoc, 1, (float *) engine->diffuseShaded[0]);
            if (this->uSpecularColorLoc >= 0)
                glUniform3fv(this->uSpecularColorLoc, 1, (float *) engine->specularShaded[0]);
            if (this->uSpecularPowerLoc >= 0)
                glUniform1f(this->uSpecularPowerLoc, engine->materialShininess);
            this->dirty = 0;
        }

        if (this->aPositionLoc >= 0)
            glEnableVertexAttribArray(this->aPositionLoc);
        if (this->aTexCoordLoc >= 0)
            glEnableVertexAttribArray(this->aTexCoordLoc);
        if (this->aNormalLoc >= 0)
            glEnableVertexAttribArray(this->aNormalLoc);
        if (this->aTangentLoc >= 0)
            glEnableVertexAttribArray(this->aTangentLoc);
        if (this->aBitangentLoc >= 0)
            glEnableVertexAttribArray(this->aBitangentLoc);

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
