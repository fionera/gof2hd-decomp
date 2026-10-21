#include "engine/render/shaders/EnergyShield.h"
#include "engine/render/Engine.h"
#include "engine/render/Mesh.h"
#include <GLES2/gl2.h>

static const char *const kEnergyShieldVertexSource =
    "attribute highp    vec3 a_position;\n"
    "attribute mediump  vec2 a_texCoord;\n"
    "attribute highp    vec3 a_normal;\n"
    "varying mediump vec2 v_texCoord;  \n"
    "varying lowp vec3 v_eye_dir;  \n"
    "varying mediump vec3 v_normal;  \n"
    "varying mediump vec3 v_normalView;  \n"
    "uniform highp mat4 u_ModelViewProjectionMatrix;\n"
    "uniform highp vec3 u_lightdirmodel[2];\n"
    "uniform highp vec3 u_eyeposmodel;\n"
    "uniform mediump mat4 u_ModelViewMatrix;\n"
    "void main()  \n"
    "{  \n"
    "\thighp vec3 vertex = a_position;\n"
    "\tgl_Position = u_ModelViewProjectionMatrix * vec4(vertex, 1.0);\n"
    "\tv_texCoord  = a_texCoord;\n"
    "\tv_normalView = normalize((u_ModelViewProjectionMatrix * vec4(a_normal, 0.0)).xyz);\n"
    "\tv_normal = a_normal;\n"
    "   v_eye_dir = normalize((u_ModelViewProjectionMatrix * vec4(u_eyeposmodel - a_position, 0.0)).rgb); \n"
    "}\n";

static const char *const kEnergyShieldFragmentSource =
    "precision lowp float;\n"
    "varying mediump vec2 v_texCoord;\n"
    "varying lowp vec3 v_eye_dir;\n"
    "varying mediump vec3 v_normal;  \n"
    "varying mediump vec3 v_normalView;  \n"
    "uniform sampler2D   s_texture[2];\n"
    "uniform sampler2D   s_texture_base;\n"
    "uniform highp vec2 u_texelSize;\n"
    "void main()\n"
    "{\n"
    "\tmediump vec4 normalTex  = texture2D( s_texture[1], v_texCoord);\n"
    "\tmediump vec3 normalTexX = normalTex.rgb*2.0 -1.0;\n"
    "\tfloat rim_intensity = (smoothstep(0.0, 1.0, 1.0 - dot( v_eye_dir,v_normalView )));\n"
    "\tlowp vec3 rim_effect = smoothstep(0.0, 1.0, 1.0 - dot( v_eye_dir,v_normalView * normalTexX)) * vec3(0.0, 0.6, 1.0);\n"
    "   mediump vec2 coord;\n"
    "   coord = (gl_FragCoord.xy + ((normalTexX.xy * 0.4 + v_normalView.xy*rim_intensity)*150.0)) * u_texelSize;\n"
    "\tgl_FragColor.rgb = texture2D(s_texture_base, coord).rgb + rim_effect;\n"
    "}\n";

namespace AbyssEngine {
    int EnergyShield::ShaderIndex;

    void EnergyShield::UpdateMeshData(Mesh *mesh, Engine *engine) {
        ::Engine *host = (::Engine *) engine;
        if (this->uMvpMatrix >= 0)
            glUniformMatrix4fv(this->uMvpMatrix, 1, 0, host->worldViewProjMatrix);
        if (this->uModelViewMatrix >= 0)
            glUniformMatrix4fv(this->uModelViewMatrix, 1, 0, host->worldViewMatrixGL);

        if (this->dirty != 0) {
            if (this->uEyePosModel >= 0)
                glUniform3f(this->uEyePosModel, host->lightColor.x, host->lightColor.y,
                            host->lightColor.z);
            if (this->uGlColor >= 0)
                glUniform4fv(this->uGlColor, 1, host->glColor);
            if (this->uRimColor >= 0)
                glUniform3fv(this->uRimColor, 1, (float *) &host->rimColor);
            int loc = this->uTexelSize;
            if (loc >= 0) {
                float w = (float) (int) ((::Engine *) engine)->GetDisplayWidth();
                float h = (float) (int) ((::Engine *) engine)->GetDisplayHeight();
                glUniform2f(loc, 1.0f / w, 1.0f / h);
            }
            glActiveTexture(0x84c7);
            ((::Engine *) engine)->ActivateRefractFBO();
            glUniform1f(this->uTime, mesh->shaderAnimValue1);
            glUniform1f(this->uVertTime, mesh->shaderAnimValue1 / 3.0f);
            this->dirty = 0;
        }

        if (this->aPosition >= 0)
            glEnableVertexAttribArray(this->aPosition);
        if (this->aTexCoord >= 0)
            glEnableVertexAttribArray(this->aTexCoord);
        if (this->aNormal >= 0)
            glEnableVertexAttribArray(this->aNormal);

        if (mesh->uploaded == 0) {
            if (this->aPosition >= 0)
                glVertexAttribPointer(this->aPosition, 3, 0x1406, 0, 0, mesh->positions);
            if (this->aTexCoord >= 0)
                glVertexAttribPointer(this->aTexCoord, 2, 0x1406, 0, 0, mesh->texCoords);
            if (this->aNormal >= 0)
                glVertexAttribPointer(this->aNormal, 3, 0x1406, 0, 0, mesh->normals);
        } else {
            glBindBuffer(0x8892, mesh->positionVBO);
            glVertexAttribPointer(this->aPosition, 3, 0x1406, 0, 0, 0);
            glBindBuffer(0x8892, mesh->texCoordVBO);
            glVertexAttribPointer(this->aTexCoord, 2, 0x1406, 0, 0, 0);
            glBindBuffer(0x8892, mesh->normalVBO);
            glVertexAttribPointer(this->aNormal, 3, 0x1406, 0, 0, 0);
        }
    }

    void EnergyShield::SetInActive() {
        if (this->aPosition >= 0)
            glDisableVertexAttribArray(this->aPosition);
        if (this->aTexCoord >= 0)
            glDisableVertexAttribArray(this->aTexCoord);
        if (this->aNormal >= 0)
            glDisableVertexAttribArray(this->aNormal);
    }

    void EnergyShield::Init(Engine *engine) {
        int program = this->ES2LoadProgram(kEnergyShieldVertexSource, kEnergyShieldFragmentSource);
        this->program = program;

        this->aPosition = glGetAttribLocation(program, "a_position");
        this->aTexCoord = glGetAttribLocation(this->program, "a_texCoord");

        this->aNormal = glGetAttribLocation(this->program, "a_normal");
        this->uMvpMatrix = glGetUniformLocation(this->program, "u_ModelViewProjectionMatrix");
        this->uModelViewMatrix = glGetUniformLocation(this->program, "u_ModelViewMatrix");
        this->uEyePosModel = glGetUniformLocation(this->program, "u_eyeposmodel");
        this->sTexture0 = glGetUniformLocation(this->program, "s_texture[0]");
        this->sTexture1 = glGetUniformLocation(this->program, "s_texture[1]");
        this->uGlColor = glGetUniformLocation(this->program, "glColor");
        this->uRimColor = glGetUniformLocation(this->program, "u_RimColor");
        this->uTexelSize = glGetUniformLocation(this->program, "u_texelSize");

        glActiveTexture(0x84c7);
        ((::Engine *) engine)->ActivateRefractFBO();

        this->uVertTime = glGetUniformLocation(this->program, "u_vertTime");
        this->uTime = glGetUniformLocation(this->program, "u_time");
        this->sTextureBase = glGetUniformLocation(this->program, "s_texture_base");

        glUseProgram(this->program);

        for (int i = 0; i != 2; i = i + 1) {
            if ((&this->sTexture0)[i] >= 0)
                glUniform1i((&this->sTexture0)[i], i);
        }
        glUniform1i(this->sTextureBase, 7);
    }

    EnergyShield::EnergyShield() {
        EnergyShield::ShaderIndex = ShaderBaseStruct::shaderIndexIntern;
        this->name = String("EnergyShield", false);
    }
}
